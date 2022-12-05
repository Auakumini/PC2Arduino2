#include <vector>
#pragma once

using namespace std;

class X10Mess
{
public:
	X10Mess();
	//~X10Mess();
	void addTwoBits(bool conditional);
	void addVibbleChar(uint8_t newchar);
	void addVibbleNum(uint8_t newchar);
	void addFooter();
	void getX10ContentsPtr(uint8_t *& X10ContentsPtr);
private:
	vector<uint8_t> X10Contents_;
	uint8_t lastWrittenBitSet_;
};
