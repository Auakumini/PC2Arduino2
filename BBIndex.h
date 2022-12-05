#pragma once

//this could probably be done smarter - make the "add signed integer to bbindex" function first
//then run that from most other functions...
//just store the total, but return with the right modal logic
// %= ByteSize modulus assignment

//or, make a version that stores a total too, but also autogenerates the others
	//so all setting methods calls this private updater...

template <uint8_t ByteSize = 8>
struct BBIndex {
	uint8_t ByteSizeVar = ByteSize;
	uint64_t ByteIndex = 0;
	uint8_t BitIndex = 0;

	BBIndex(const uint64_t newByteIndex = 0, uint64_t newBitIndex = 0)
		: ByteIndex(newByteIndex), BitIndex(newBitIndex)
	{
	}

	bool operator == (const BBIndex& rhs) const {
		return (ByteIndex == rhs.ByteIndex) && (BitIndex == rhs.BitIndex) && (ByteSize == rhs.ByteSizeVar);
	}

	bool operator != (const BBIndex& rhs) const {
		//return (ByteIndex != rhs.ByteIndex) || (BitIndex != rhs.BitIndex);
		//*** should just use == operator... "~"?
		return !(*this == rhs);
	}

	bool operator < (const BBIndex& rhs) const {
		if (ByteSize != rhs.ByteSizeVar) { return false; };
		if (ByteIndex > rhs.ByteIndex) { return false; };
		if ((ByteIndex == rhs.ByteIndex) && (BitIndex > rhs.BitIndex)) { return false; };
		return true;
	}

	bool operator > (const BBIndex& rhs) const {
		if (ByteSize != rhs.ByteSize) { return false; };
		if (ByteIndex < rhs.ByteIndex) { return false; };
		if ((ByteIndex == rhs.ByteIndex) && (BitIndex < rhs.BitIndex)) { return false; };
		return true;
	}

	BBIndex operator+(const BBIndex& rhs) const {
		uint64_t BitIndexTotal = BitIndex + rhs.BitIndex;
		return BBIndex(
			ByteIndex + rhs.ByteIndex + (BitIndexTotal / ByteSize),
			(BitIndexTotal % ByteSize));
	}

	BBIndex& operator++() {
		if (BitIndex == (ByteSize - 1)) { ByteIndex++; BitIndex = 0; }
		else { BitIndex++; }
		return *this;
	}

	BBIndex operator++(int)
	{
		BBIndex old = *this;
		operator++();
		return old;
	}

	BBIndex& operator--() {
		if (BitIndex == 0) { ByteIndex--; BitIndex = ByteSize - 1; }
		else { BitIndex--; }
		return *this;
	}

	BBIndex operator--(int)
	{
		BBIndex old = *this;
		operator--();
		return old;
	}

};