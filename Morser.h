#include <string>
#include <vector>
#include <stdbool.h>
#include "BBIndex.h"
#include "X10Mess.h"

using namespace std;

//clean up this goddamn mess
//	allAttrPtr is really the next step...

const vector<uint8_t> X10Header = { 0, 0, 0, 0, 1, 1, 1, 0};				// 6+4 bits
	//needs a footer first, since we need to be able to interrupt an already sending message
const vector<uint8_t> X10Footer = {0, 0, 0, 0, 0, 0};						// 6 bits
//change this to conform better to x10 protocol - see local file x10 stuff
	//0, 1, 1, 0, 1, 0, 0, 1,														// House code A
	//0, 1, 1, 0, 1, 0, 0, 1, 0, 1,												// Key code, adressing unit 1
//remember to change offset code if this is changed?


static const char* CHAR_TO_MORSE_BIN[128] = {
	//Remember this is copied from online source, then changed
	//Comment also on why they end in 0's, and why the space morse-char has the length it has
	// Recognizes ".", ",", "?"
	NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
	NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
	NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
	"1010", NULL, NULL, NULL, NULL, NULL, NULL, NULL,
	"0000000", "11101011101011101110", "1011101010111010", NULL, NULL, NULL, NULL, "10111011101110111010",
	"1110101110111010", "11101011101110101110", NULL, NULL, "11101110101011101110", "1110101010101110", "101110101110101110", "11101010111010",
	"11101110111011101110", "101110111011101110", "1010111011101110", "10101011101110", "101010101110", "1010101010", "111010101010", "11101110101010",
	"1110111011101010", "111011101110111010", "111011101110101010", NULL, NULL, "11101010101110", NULL, "1010111011101010",
	"101110111010111010", "101110", "1110101010", "111010111010", "11101010", "10", "1010111010", "1110111010",
	"10101010", "1010", "10111011101110", "1110101110", "1011101010", "11101110", "111010", "111011101110",
	"101110111010", "11101110101110", "10111010", "101010", "1110", "10101110", "1010101110", "1011101110",
	"111010101110", "11101011101110", "111011101010", NULL, NULL, NULL, NULL, "101011101110101110",
	NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
	NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
	NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
	NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
};

static const uint8_t CHAR_LEN_IN_MORSE[128] = {
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 4, 0, 0, 0, 0, 0, 0, 0, 7, 20, 16, 0, 0, 0, 0,
	20, 16, 20, 0, 0, 20, 16, 18, 14, 20, 18, 16, 14, 12, 10, 12, 14, 16, 18, 18, 0, 0, 14, 0, 16, 18, 6, 10, 12, 8, 2, 10,
	10, 8, 4, 14, 10, 10, 8, 6, 12, 12, 14, 8, 6, 4, 8, 10, 10, 12, 14, 12, 0, 0, 0, 0, 18, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };

const char* char_to_morse(char);
	//might not be needed...


class Morser {
public:
	// Constructors
		Morser();
	// Setters
		void setMessage(string);
			//should probably return bool, false if contains illegal chars? or just fix?
		void setSpeed(uint8_t newspeed);
		void setOnIntensity(uint8_t onInt);
		void setOffIntensity(uint8_t offInt);
	// Getters
		string getMessage() const;
		uint16_t getSpeed() const;
		uint8_t getOnIntensity_() const;
		uint8_t getOffIntensity_() const;

		void getMessageInMorse(const uint8_t* &messPtrParam);
		uint32_t GetMessageInMorseLength() const;

		void getAllAttrPtr(uint8_t *& allAttrPtr);
			//old version - uint8_t* const getAllAttrPtr();

		
	//***
		// return what? promised ptr's, but vectors are just a better fit - vector references?

		//uint8_t const * getMorseBinPtr();
		//	deprecated - using 
		//		clean up this and functions using it - test that they are not in use first...
		//	...
			//generates, calls private method genAllAttr(), should return a read-only pointer
				//https://stackoverflow.com/questions/16452218/return-a-read-only-pointer-from-the-int-pointer-in-c
		bool allAttrUpToDate_;
		X10Mess myX10Mess_;
		//should be private, only public for testing
private:
	// Notes
		//we only make vibbles when sending
	uint8_t speed_;
	uint8_t onIntensity_;
	uint8_t offIntensity_;
	string messOrg_;
	vector<uint8_t> messVibbles_;					// 

	// ***************************
		// Which of the below are necessary as attributes, and which should be moved to local, method variables?
		// Outside of methods - "permanent" - we need:
			uint32_t messMorseBinLen_;
				//useful for only needing to go through the org mess once
				//nope - not needed on pc

	uint8_t * messMorseBin_;
	BBIndex<8> messMorseBinIndex_;

	//these go into "5 5-bit vibbles" = 25 bits = 7 bits left over if we need some bools or something???

	unsigned int messageInMorsePtr_;
	unsigned int messageInMorsePtrMax_;

	void genMorseBin();
	void genAllAttr();									// Generates proper X10 command in allAttr_
		//this and related variables could be more cleanly a method under a new object (composition)


			//should be dynamically allocated
		//uint8_t * AllAttrPtr_;							// Points to start of allAttrVibbles_
			// Probably does need to be an attributes itself
};