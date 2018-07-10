#pragma once
#include <cstring>
#include <string>
#include <algorithm>
#include <assert.h>
#include <ctime>
#include <iostream>

using namespace std;
const int maxLength = 512;
/*产生素数所占的位数*/
const int primeLength = 30;
const int primesBelow2000[303] = {
	2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37, 41, 43, 47, 53, 59, 61, 67, 71, 73, 79, 83, 89, 97,
	101, 103, 107, 109, 113, 127, 131, 137, 139, 149, 151, 157, 163, 167, 173, 179, 181, 191, 193, 197, 199,
	211, 223, 227, 229, 233, 239, 241, 251, 257, 263, 269, 271, 277, 281, 283, 293,
	307, 311, 313, 317, 331, 337, 347, 349, 353, 359, 367, 373, 379, 383, 389, 397,
	401, 409, 419, 421, 431, 433, 439, 443, 449, 457, 461, 463, 467, 479, 487, 491, 499,
	503, 509, 521, 523, 541, 547, 557, 563, 569, 571, 577, 587, 593, 599,
	601, 607, 613, 617, 619, 631, 641, 643, 647, 653, 659, 661, 673, 677, 683, 691,
	701, 709, 719, 727, 733, 739, 743, 751, 757, 761, 769, 773, 787, 797,
	809, 811, 821, 823, 827, 829, 839, 853, 857, 859, 863, 877, 881, 883, 887,
	907, 911, 919, 929, 937, 941, 947, 953, 967, 971, 977, 983, 991, 997,
	1009, 1013, 1019, 1021, 1031, 1033, 1039, 1049, 1051, 1061, 1063, 1069, 1087, 1091, 1093, 1097,
	1103, 1109, 1117, 1123, 1129, 1151, 1153, 1163, 1171, 1181, 1187, 1193,
	1201, 1213, 1217, 1223, 1229, 1231, 1237, 1249, 1259, 1277, 1279, 1283, 1289, 1291, 1297,
	1301, 1303, 1307, 1319, 1321, 1327, 1361, 1367, 1373, 1381, 1399,
	1409, 1423, 1427, 1429, 1433, 1439, 1447, 1451, 1453, 1459, 1471, 1481, 1483, 1487, 1489, 1493, 1499,
	1511, 1523, 1531, 1543, 1549, 1553, 1559, 1567, 1571, 1579, 1583, 1597,
	1601, 1607, 1609, 1613, 1619, 1621, 1627, 1637, 1657, 1663, 1667, 1669, 1693, 1697, 1699,
	1709, 1721, 1723, 1733, 1741, 1747, 1753, 1759, 1777, 1783, 1787, 1789,
	1801, 1811, 1823, 1831, 1847, 1861, 1867, 1871, 1873, 1877, 1879, 1889,
	1901, 1907, 1913, 1931, 1933, 1949, 1951, 1973, 1979, 1987, 1993, 1997, 1999 };

class BigInteger
{
	typedef unsigned char byte;
public:
	/*构造函数*/
	BigInteger(void);
	BigInteger(__int64 value);
	BigInteger(unsigned __int64 value);
	BigInteger(const BigInteger &bi);
	BigInteger(string value, int radix);
	BigInteger(byte inData[], int inLen);
	BigInteger(unsigned int inData[], int inLen);
	/*负号的重载*/
	BigInteger operator -();

	/*加减乘除四则运算的重载*/
	BigInteger operator +(BigInteger &bi2);
	BigInteger operator -(BigInteger bi2);
	BigInteger operator /(BigInteger bi2);
	BigInteger operator *(BigInteger bi2);
	void singleByteDivide(BigInteger &bi1, BigInteger &bi2, BigInteger &outQuotient, BigInteger &outRemainder);
	void multiByteDivide(BigInteger &bi1, BigInteger &bi2, BigInteger &outQuotient, BigInteger &outRemainder);
	/*%的重载*/
	BigInteger operator %(BigInteger bi2);

	BigInteger operator +=(BigInteger bi2);
	BigInteger operator -=(BigInteger bi2);
	BigInteger operator =(const BigInteger &bi2);

	int bitCount();
	/*=================================================
	modpow函数
	作用：求this^exp mod n(this,exp,n都是大整数)，并返回这个BigInteger类型的大整数
	=================================================*/
	BigInteger modPow(BigInteger exp, BigInteger n);


	friend ostream& operator<<(ostream& output, BigInteger &bi1);

	/*=================================================
	GetPrime函数
	作用：返回一个BigInteger类型的大素数
	=================================================*/
	friend BigInteger GetPrime();

	/*=================================================
	Miller_Robin算法
	作用：判断大数 bi1是否为素数 是返回true,否返回false
	=================================================*/
	friend bool Miller_Robin(BigInteger &bi1);

	/*扩展的欧几里德算法*/
	friend BigInteger extended_euclidean(BigInteger n, BigInteger m, BigInteger &x, BigInteger &y);

	/*=================================================
	MultInverse函数
	作用：求大整数bi1在mod 大整数 n 下的逆元，并返回这个BigInteger类型的逆元
	=================================================*/
	friend BigInteger MultipInverse(BigInteger &bi1, BigInteger &n);

	/*=================================================
	Gcd函数
	作用：求大整数bi1和大整数bi2的最大公约数，并返回这个BigInteger类型的最大公约数
	=================================================*/
	friend BigInteger Gcd(BigInteger &bi1, BigInteger &bi2);

	/*=================================================
	Gcd函数
	作用：判断大整数obj是否为素数，是则返回true,否返回false
	=================================================*/
	friend bool IsPrime(BigInteger &obj);

	BigInteger BarrettReduction(BigInteger x, BigInteger n, BigInteger constant);

	/*比较运算的重载*/
	bool operator >=(BigInteger bi2)
	{
		return ((*this) == bi2 || (*this) > bi2);
	}

	bool operator >(BigInteger bi2);
	bool operator ==(BigInteger bi2);
	bool operator !=(BigInteger bi2);

	/*移位操作*/
	int shiftRight(unsigned int buffer[], int bufLen, int shiftVal);
	BigInteger operator <<(int shiftVal);
	int shiftLeft(unsigned int buffer[], int bufLen, int shiftVal);
	bool operator <(BigInteger bi2);

	string DecToHex(unsigned int value, string format);
	string ToHexString();

public:
	~BigInteger(void);

public:
	int dataLength;
	// number of actual chars used
	unsigned int *data;
};
