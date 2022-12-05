#include <stdint.h>
#include <string>
#include "Morser.h"
#include <iostream>

using namespace std;

Morser::Morser()
	: messOrg_(""), onIntensity_(0), offIntensity_(0), speed_(0), allAttrUpToDate_(true)
{
	//allAttr_.clear();
	messVibbles_.clear();

	//	allAttrPtr_ = nullptr;					// unnecessary, gen as needed? return using data()

	//These should be unnecessary, and moved to genallatr
		messageInMorsePtr_ = 0;
		messageInMorsePtrMax_ = 0;
		BBIndex<8> messageInMorseIndex_(0,0);
		uint8_t* messageInMorse_ = nullptr;
}

void Morser::setSpeed(uint8_t newspeed) {
	// Speed is higher, the higher the speed - default use to to convert to milliseconds wait time using eg.
		// uint16_t loopSpeed = 8000 / TestSender_.getSpeed();
	// Ie. dits of, given a speed_ of: 8 secs (1), 1 sec (8), 0.5 sec (16), 0.258 sec (31)
	if (speed_ == newspeed) { return; }

	if (newspeed > 31) {
		speed_ = 31;
	}
	else {
		speed_ = newspeed;
	}
	allAttrUpToDate_ = false;
}

void Morser::setOnIntensity(uint8_t onInt) {
	if (onIntensity_ == onInt) { return; }

	if (onInt > 31) {
		onIntensity_ = 31;
	}
	else {
		onIntensity_ = onInt;
	}
	allAttrUpToDate_ = false;
}

void Morser::setOffIntensity(uint8_t offInt) {
	if (offIntensity_ == offInt) { return; }

	if (offInt > 31) {
		offIntensity_= 31;
	}
	else {
		offIntensity_ = offInt;
	}
	allAttrUpToDate_ = false;
}

void Morser::setMessage(string newmessage) {
	if (messOrg_ == newmessage) { return; }

	messOrg_ = newmessage;
	messMorseBinLen_ = 0;
	for (uint64_t i = 0; i < messOrg_.length(); i++) {
		if ((messOrg_[i] >= 97) && (messOrg_[i] <= 122)) {
			messOrg_[i] += ('A' - 'a');
		}
	}
	messOrg_ += " ";

	allAttrUpToDate_ = false;

		//clean up the input that goes into messOrg_ - wide chars or something for זרו?
			//or throw error if it contains them?
			//should contain spaces, dots and commas, etc.
}

void Morser::getAllAttrPtr(uint8_t*& allAttrPtr) {
	//test for const - where do we make sure noone can write using the pointer they get (seems OK, but test)

	if (allAttrUpToDate_ == false) {
		genAllAttr();
	}
	
	myX10Mess_.getX10ContentsPtr(allAttrPtr);
	return;
	//change to x10payload.something...


	//was..
	// return allAttr_.data();
		//should give a pointer to allattr internal array
			//considered bad form - but needed for C compatibility, is the menu running C?
}

void Morser::genAllAttr()
{

	// Add header

	//nope - not a true header at all
		//make an add literal function
	//		or go back to header + payload + footer functionality
	//			maybe myX10Mess_ could be "born" with the header, and it could take up one byte
	//				then, make sure the pushback is activated (as is?) and it should remain
	

	//remember to fix footer too


	// Add payload - nonmessage

	myX10Mess_.addVibbleNum(speed_);
	myX10Mess_.addVibbleNum(onIntensity_);
	myX10Mess_.addVibbleNum(offIntensity_);

	// Add payload - message
	
	for (auto it = messOrg_.begin(); it != messOrg_.end(); it++) {
		myX10Mess_.addVibbleChar(*it);
	}

	//Add footer - ensure NULL stop (and 6 trailing 0's)
		//*** sloppy, but hard to do better...

	myX10Mess_.addFooter();										//Add footer

	allAttrUpToDate_ = true;

}




void Morser::genMorseBin()
{

	messVibbles_.clear();
	BBIndex<5> VibbleIndex(0, 0);

	//should split this up
		// first, we take in the new message, save it in messOrg (as uppercase)
			// and create the length we need in morseBin
		// then send it on to genMorseBin automatically?
			// maybe not quite what it says in the seqDia ???


	// Set correct messageInMorseLength, and raise to Upper Case

	messMorseBinLen_ = 0;
	for (uint8_t i = 0; i <= messOrg_.length(); i++) {
		if ((messOrg_[i] >= 97) && (messOrg_[i] <= 122)) {
			messOrg_[i] += ('A' - 'a');
			//hopefully, this gives us spaces too...
				//check זרו somehow
			//maybe make an else that for this if, that checks those things
		}
		messMorseBinLen_ += (int)CHAR_LEN_IN_MORSE[messOrg_[i]];
			//***test here
			//also, do we need it as an attribute, or can we just do it on the fly?
	}

	//include X10 start and stop here or at the end?
//		newMorseBin not the right name then...
//			copy this code first, for genFromX10...





//this should be a new function - genMo


// Now we know the length - Allocate space on the heap for enough bits, and initialize it to 0
	uint8_t* newMessageInMorse = new uint8_t[(messMorseBinLen_ / 8)];
	//+1 to length - nope, stop bit comes later
	for (uint64_t i = 0; i < messMorseBinLen_; i++) {
		newMessageInMorse[i] = 0b00000000;
	}


	// Looping over each letter in newmessage

	BBIndex<8> messageInMorseIndex = { 0,0 };				// Index for where we are writing in the output message
	BBIndex<8> nextMessageInMorseIndex = { 0,0 };			// next index to increase towards
	const char* currentLetterInMorse = nullptr;

	for (uint32_t i = 0; i < messOrg_.length(); i++) {

		// Loop over the current letter - AND, the index of our output message (by both Byte and Bit)
			//correct to loop until and including nextmess...?
		currentLetterInMorse = CHAR_TO_MORSE_BIN[(int)messOrg_[i]];
		nextMessageInMorseIndex = messageInMorseIndex + BBIndex<8>(0, (int)CHAR_LEN_IN_MORSE[messOrg_[i]]);

		uint64_t currentLetterInMorseIndex = 0;

		for (messageInMorseIndex;
			messageInMorseIndex != nextMessageInMorseIndex;
			//			messageInMorseIndex = messageInMorseIndex + BBIndex(0, 1)) {
			messageInMorseIndex++) {

			// If currentLetterInMorse dictates a 1 in this place, the current byte according to our index is OR'ed
			// ...with itself and a byte with only the (single) bit set corresponding to BitIndex
			if (currentLetterInMorse[currentLetterInMorseIndex] == '1') {
				newMessageInMorse[messageInMorseIndex.ByteIndex] =
					newMessageInMorse[messageInMorseIndex.ByteIndex] | ((uint16_t)1 << messageInMorseIndex.BitIndex);
			}
			currentLetterInMorseIndex++;
		}
	}

	// Move pointer of messageinmorse to newmessage

	delete[] messMorseBin_;
	messMorseBin_ = newMessageInMorse;

	//remember a stop bit?

}









//getters

string Morser::getMessage() const
{
	return messOrg_;
}

void Morser::getMessageInMorse(const uint8_t* &messPtrParam)
{
	//notes
	//		uint8_t * messageInMorse_;
	//this moved a pointer correctly:
	//			messPtr = messPtr2;

	// throwing a reference  around (and not throwing it any further) is useless here

	genMorseBin();
	messPtrParam = messMorseBin_;
}

uint32_t Morser::GetMessageInMorseLength() const
{
	return messMorseBinLen_;
	//should probably be deleted, no need for this attr
}


uint16_t Morser::getSpeed() const
{
	return speed_;
}

uint8_t Morser::getOnIntensity_() const
{
	return onIntensity_;
}

uint8_t Morser::getOffIntensity_() const
{
	return offIntensity_;
}







//internal, ie. private,  functions


const char* char_to_morse (char c)
{
	//Remember this is copied from online source, 

	if ( (c >= 97) && (c <= 122) ) {
		c += ('A' - 'a');
	}
	return CHAR_TO_MORSE_BIN[(int) c];
}






/*
		uint8_t X10Command_[] = {
		1, 1, 1, 0,																	// Start code
		0, 1, 1, 0, 1, 0, 0, 1,														// House code A (0110)
		0, 1, 1, 0, 1, 0, 0, 1, 0, 1,												// Key code, adressing unit 1
		1, 1, 1, 0,	0, 1, 1, 0, 1, 0, 0, 1, 0, 1, 1, 0, 1, 0, 0, 1, 0, 1,			// Repeat above
		0, 0, 0, 0, 0, 0,															// Wait six zero-crossings

		1, 1, 1, 0,																	// Start code
		0, 1, 1, 0, 1, 0, 0, 1,														// House code A
		0, 0, 0, 0, 0, 0, 0, 0,	1, 0,												// Key code, command (blank)
		1, 1, 1, 0,	0, 1, 1, 0, 1, 0, 0, 1,	0, 0, 0, 0, 0, 0, 0, 0,	1, 0,			// Repeat above
		0, 0, 0, 0, 0, 0,															// Wait six zero-crossings
	}
	// X10Command_ har 100 entries
	// Nye kommandoer skrives ind i index: 61-71, samt 83-93

*/