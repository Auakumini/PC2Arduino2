#include "X10Mess.h"

#include <iostream>
using namespace std;
	//remove these two...

X10Mess::X10Mess()
	: lastWrittenBitSet_(4)
{
	X10Contents_.push_back(0b00001110);
}

void X10Mess::addTwoBits(bool conditional) {
	// Adds 10 if conditional is true, else 01, to the end of the X10Payload vector

	if (lastWrittenBitSet_ == 4) {						// If counter is too low...
		X10Contents_.push_back(NULL);						// Make space on vector (null so we can OR 0's onto it directly)
		lastWrittenBitSet_ = 0;								// Reset counter
	}

	if (conditional == true) {
		X10Contents_.back() |= (0b10000000 >> (lastWrittenBitSet_ * 2) );		// We do not need to "force" our 0's through any ther logic - we pushed a NULL onto the 
			//force 0? no need - we are writing to a null
	} else {
		X10Contents_.back() |= (0b01000000 >> (lastWrittenBitSet_ * 2) );
	}
	lastWrittenBitSet_++;
}

void X10Mess::addVibbleChar(uint8_t newchar)
{
//																	Our protocol
	// ' '	32														27
	// .	46														28
	// ,	44														29
	// a-z	65-90		0b0100 0001			0b0101 0101				
	// A-Z	97-122		0b0110 0001			0b0111 1010				1-25

	// *** we can do without the lower case stuff, redo and simplify if that remains the case...
		//check to see if upper case, check one bit?

	if (												// Sort out simple cases:
		newchar >= 123 ||								// Too high for any char or special
		(newchar >= 91 && newchar <= 96)				// Between high and lower case
		) return;

	switch (newchar)									// Handle special chars
	{
	case ' ':
		addVibbleNum(27);
		break;
	case ',':
		addVibbleNum(28);
		break;
	case '.':
		addVibbleNum(29);
		break;

		if (												// Sort out remaining simple cases:
			newchar <= 64									// 
			) return;
	}
	addVibbleNum(newchar);
}

void X10Mess::addVibbleNum(uint8_t newchar)
{
	for (uint8_t i = 0; i < 5;i++) {						//We only read the first five bits of the byte
		//read from bit, AND it out, then bitshift?
		addTwoBits(newchar & 0b00000001);
		newchar >>= 1;
	}
}

void X10Mess::addFooter()
{
	// Adds stop-byte (NULL), at the same time ensuring the X10 message ends with 6 0's
	//just check if the last byte is null, if not we add one
	if (X10Contents_.back() != NULL) {
		X10Contents_.push_back(NULL);
	}
}

void X10Mess::getX10ContentsPtr(uint8_t*& X10ContentsPtr) 
{
	//debug statements
		uint8_t* test1 = X10Contents_.data();
		uint8_t test2 = *test1;
			//seem about right, gives 01 10 10 10...

	X10ContentsPtr = X10Contents_.data();
	return;
}
