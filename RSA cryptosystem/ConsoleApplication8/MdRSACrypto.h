#pragma once
#include "BigInteger.h"
#include <cmath>
#include <string>

using namespace std;

class MdRSACrypto
{
	typedef unsigned char byte;
public:
	MdRSACrypto(void);
	void Encrypt(byte data[], int dataLen, byte *&secData, int &secDataLen);
	void Decrypt(byte data[], int dataLen, byte *&srcData, int &srcDataLen);
	void BigIntegerToBytes(BigInteger bi, byte *&secData, int &secDataLen);
	byte htoi(const char *str);
public:
	~MdRSACrypto(void);
public:
	BigInteger ParamD;
	BigInteger ParamE;
	BigInteger ParamN;
};
