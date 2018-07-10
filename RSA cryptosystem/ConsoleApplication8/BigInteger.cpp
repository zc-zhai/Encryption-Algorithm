#include "BigInteger.h"

BigInteger::BigInteger(void)   //默认的构造函数
: dataLength(0), data(0)
{
	data = new unsigned int[maxLength];
	memset(data, 0, maxLength * sizeof(unsigned int));
	dataLength = 1;
}

BigInteger::BigInteger(__int64 value)   //用一个64位的值来初始化大数
{
	data = new unsigned int[maxLength];
	memset(data, 0, maxLength * sizeof(unsigned int));   //先清零
	__int64 tempVal = value;

	dataLength = 0;
	while (value != 0 && dataLength < maxLength)
	{
		data[dataLength] = (unsigned int)(value & 0xFFFFFFFF);   //取低位
		value = value >> 32;   //进位
		dataLength++;
	}

	if (tempVal > 0)         // overflow check for +ve value
	{
		if (value != 0 || (data[maxLength - 1] & 0x80000000) != 0)
			assert(false);
	}
	else if (tempVal < 0)    // underflow check for -ve value
	{
		if (value != -1 || (data[dataLength - 1] & 0x80000000) == 0)
			assert(false);
	}

	if (dataLength == 0)
		dataLength = 1;
}

BigInteger::BigInteger(unsigned __int64 value)   //用一个无符号的64位整数来初始化大数
{
	data = new unsigned int[maxLength];
	memset(data, 0, maxLength * sizeof(unsigned int));

	dataLength = 0;
	while (value != 0 && dataLength < maxLength)
	{
		data[dataLength] = (unsigned int)(value & 0xFFFFFFFF);
		value >>= 32;
		dataLength++;
	}

	if (value != 0 || (data[maxLength - 1] & 0x80000000) != 0)
		assert(false);

	if (dataLength == 0)   //防止输入的value=0
		dataLength = 1;
}

BigInteger::BigInteger(const BigInteger &bi)   //用大数初始化大数
{
	data = new unsigned int[maxLength];
	dataLength = bi.dataLength;

	for (int i = 0; i < maxLength; i++)   //考虑到有负数的情况,所以每一位都要复制
		data[i] = bi.data[i];
}

BigInteger::~BigInteger(void)
{
	if (data != NULL)
	{
		delete[]data;
	}
}

BigInteger::BigInteger(string value, int radix)   //输入转换函数,将字符串转换成对应进制的大数
{   //一般不处理负数
	BigInteger multiplier((__int64)1);
	BigInteger result;
	transform(value.begin(), value.end(), value.begin(), toupper);   //将小写字母转换成为大写

	int limit = 0;

	if (value[0] == '-')
		limit = 1;

	for (int i = value.size() - 1; i >= limit; i--)
	{
		int posVal = (int)value[i];

		if (posVal >= '0' && posVal <= '9')  //将字符转换成数字
			posVal -= '0';
		else if (posVal >= 'A' && posVal <= 'Z')
			posVal = (posVal - 'A') + 10;
		else
			posVal = 9999999;       // arbitrary large 输入别的字符


		if (posVal >= radix)   //不能大于特定的进制,否则终止
		{
			assert(false);
		}
		else
		{
			result = result + (multiplier * BigInteger((__int64)posVal));

			if ((i - 1) >= limit)   //没有到达尾部
				multiplier = multiplier * BigInteger((__int64)radix);
		}
	}

	if (value[0] == '-')   //符号最后再处理
		result = -result;

	if (value[0] == '-')     //输入为负数,但得到的结果为正数,可能溢出了
	{
		if ((result.data[maxLength - 1] & 0x80000000) == 0)
			assert(false);
	}
	else    //或者说,输入为正数,得到的结果为负数,也可能溢出了
	{
		if ((result.data[maxLength - 1] & 0x80000000) != 0)
			assert(false);
	}


	data = new unsigned int[maxLength];
	//memset(data, 0, maxLength * sizeof(unsigned int));
	for (int i = 0; i < maxLength; i++)
		data[i] = result.data[i];

	dataLength = result.dataLength;
}

BigInteger::BigInteger(byte inData[], int inLen)   //用一个char类型的数组来初始化大数
{
	dataLength = inLen >> 2;   //一个unsigned int占32位,而一个unsigned char只占8位
	//因此dataLength应该是至少是inLen/4,不一定整除

	int leftOver = inLen & 0x3;
	//取最低两位的数值,为什么要这样干呢？实际上是为了探测len是不是4的倍数,好确定dataLength的长度
	if (leftOver != 0)    //不能整除的话,dataLength要加1
		dataLength++;


	if (dataLength > maxLength)
		assert(false);

	data = new unsigned int[maxLength];
	memset(data, 0, maxLength * sizeof(unsigned int));

	for (int i = inLen - 1, j = 0; i >= 3; i -= 4, j++)
	{
		data[j] = (unsigned int)((inData[i - 3] << 24) + (inData[i - 2] << 16) + (inData[i - 1] << 8) + inData[i]);
		//我们知道:一个unsigned int占32位,而一个unsigned char只占8位,因此四个unsigned char才能组成一个unsigned int
		//因此取inData[i - 3]为前32-25位,inData[i - 2]为前24-17~~~
		//i % 4 = 0 or 1 or 2 or 3 余0表示恰好表示完
	}

	if (leftOver == 1)
		data[dataLength - 1] = (unsigned int)inData[0];
	else if (leftOver == 2)
		data[dataLength - 1] = (unsigned int)((inData[0] << 8) + inData[1]);
	else if (leftOver == 3)
		data[dataLength - 1] = (unsigned int)((inData[0] << 16) + (inData[1] << 8) + inData[2]);


	while (dataLength > 1 && data[dataLength - 1] == 0)
		dataLength--;
}

BigInteger::BigInteger(unsigned int inData[], int inLen)   //用一个unsigned int型数组初始化大数
{
	dataLength = inLen;

	if (dataLength > maxLength)
		assert(false);

	data = new unsigned int[maxLength];
	memset(data, 0, maxLength * sizeof(maxLength));

	for (int i = dataLength - 1, j = 0; i >= 0; i--, j++)
		data[j] = inData[i];

	while (dataLength > 1 && data[dataLength - 1] == 0)
		dataLength--;
}

BigInteger BigInteger::operator *(BigInteger bi2)   //乘法的重载
{
	BigInteger bi1(*this);
	int lastPos = maxLength - 1;
	bool bi1Neg = false, bi2Neg = false;

	//首先对两个乘数取绝对值
	try
	{
		if ((this->data[lastPos] & 0x80000000) != 0)     //bi1为负数
		{
			bi1Neg = true;
			bi1 = -bi1;
		}
		if ((bi2.data[lastPos] & 0x80000000) != 0)     //bi2为负数
		{
			bi2Neg = true; bi2 = -bi2;
		}
	}
	catch (...) {}

	BigInteger result;

	//绝对值相乘
	try
	{
		for (int i = 0; i < bi1.dataLength; i++)
		{
			if (bi1.data[i] == 0) continue;

			unsigned __int64 mcarry = 0;
			for (int j = 0, k = i; j < bi2.dataLength; j++, k++)
			{
				// k = i + j
				unsigned __int64 val = ((unsigned __int64)bi1.data[i] * (unsigned __int64)bi2.data[j]) + (unsigned __int64)result.data[k] + mcarry;
				result.data[k] = (unsigned __int64)(val & 0xFFFFFFFF);   //取低位
				mcarry = (val >> 32);   //进位
			}

			if (mcarry != 0)
				result.data[i + bi2.dataLength] = (unsigned int)mcarry;
		}
	}
	catch (...)
	{
		assert(false);
	}


	result.dataLength = bi1.dataLength + bi2.dataLength;
	if (result.dataLength > maxLength)
		result.dataLength = maxLength;

	while (result.dataLength > 1 && result.data[result.dataLength - 1] == 0)
		result.dataLength--;

	// overflow check (result is -ve)溢出检查
	if ((result.data[lastPos] & 0x80000000) != 0)  //结果为负数
	{
		if (bi1Neg != bi2Neg && result.data[lastPos] == 0x80000000)    //两乘数符号不同
		{
			// handle the special case where multiplication produces
			// a max negative number in 2's complement.

			if (result.dataLength == 1)
				return result;
			else
			{
				bool isMaxNeg = true;
				for (int i = 0; i < result.dataLength - 1 && isMaxNeg; i++)
				{
					if (result.data[i] != 0)
						isMaxNeg = false;
				}

				if (isMaxNeg)
					return result;
			}
		}

		assert(false);
	}

	//两乘数符号不同,结果为负数
	if (bi1Neg != bi2Neg)
		return -result;

	return result;
}

BigInteger BigInteger::operator =(const BigInteger &bi2)
{
	if (&bi2 == this)
	{
		return *this;
	}
	if (data != NULL)
	{
		delete[]data;
		data = NULL;
	}
	data = new unsigned int[maxLength];
	memset(data, 0, maxLength * sizeof(unsigned int));

	dataLength = bi2.dataLength;

	for (int i = 0; i < maxLength; i++)
		data[i] = bi2.data[i];
	return *this;
}

BigInteger BigInteger::operator +(BigInteger &bi2)
{
	int lastPos = maxLength - 1;
	bool bi1Neg = false, bi2Neg = false;
	BigInteger bi1(*this);
	BigInteger result;

	if ((this->data[lastPos] & 0x80000000) != 0)     //bi1为负数
		bi1Neg = true;
	if ((bi2.data[lastPos] & 0x80000000) != 0)     //bi2为负数
		bi2Neg = true;

	if (bi1Neg == false && bi2Neg == false)   //bi1与bi2都是正数
	{
		result.dataLength = (this->dataLength > bi2.dataLength) ? this->dataLength : bi2.dataLength;

		__int64 carry = 0;
		for (int i = 0; i < result.dataLength; i++)   //从低位开始,逐位相加
		{
			__int64 sum = (__int64)this->data[i] + (__int64)bi2.data[i] + carry;
			carry = sum >> 32;   //进位
			result.data[i] = (unsigned int)(sum & 0xFFFFFFFF);  //取低位结果
		}

		if (carry != 0 && result.dataLength < maxLength)
		{
			result.data[result.dataLength] = (unsigned int)(carry);
			result.dataLength++;
		}

		while (result.dataLength > 1 && result.data[result.dataLength - 1] == 0)
			result.dataLength--;


		//溢出检查
		if ((this->data[lastPos] & 0x80000000) == (bi2.data[lastPos] & 0x80000000) &&
			(result.data[lastPos] & 0x80000000) != (this->data[lastPos] & 0x80000000))
		{
			assert(false);
		}
		return result;
	}
	//关键在于,负数全部要转化成为正数来做
	if (bi1Neg == false && bi2Neg == true)   //bi1正,bi2负
	{
		BigInteger bi3 = -bi2;
		if (bi1 > bi3)
		{
			result = bi1 - bi3;
			return result;
		}
		else
		{
			result = -(bi3 - bi1);
			return result;
		}
	}

	if (bi1Neg == true && bi2Neg == false)  //bi1负,bi2正
	{
		BigInteger bi3 = -bi1;
		if (bi3 > bi2)
		{
			result = -(bi3 - bi2);
			return result;
		}
		else
		{
			result = bi2 - bi3;
			return result;
		}
	}

	if (bi1Neg == true && bi2Neg == true)  //bi1负,bi2负
	{
		result = -((-bi1) + (-bi2));
		return result;
	}
}

BigInteger BigInteger::operator -()
{
	//逐位取反并+1
	if (this->dataLength == 1 && this->data[0] == 0)
		return *this;

	BigInteger result(*this);

	for (int i = 0; i < maxLength; i++)
		result.data[i] = (unsigned int)(~(this->data[i]));   //取反

	__int64 val, carry = 1;
	int index = 0;

	while (carry != 0 && index < maxLength)  //+1；
	{
		val = (__int64)(result.data[index]);
		val++;   //由于值加了1个1,往前面的进位最多也只是1个1,因此val++就完了

		result.data[index] = (unsigned int)(val & 0xFFFFFFFF);   //取低位部分
		carry = val >> 32;   //进位

		index++;
	}

	if ((this->data[maxLength - 1] & 0x80000000) == (result.data[maxLength - 1] & 0x80000000))
		result.dataLength = maxLength;

	while (result.dataLength > 1 && result.data[result.dataLength - 1] == 0)
		result.dataLength--;

	return result;
}

BigInteger BigInteger::modPow(BigInteger exp, BigInteger n)   //求this^exp mod n
{
	if ((exp.data[maxLength - 1] & 0x80000000) != 0)   //指数是负数
		return BigInteger((__int64)0);

	BigInteger resultNum((__int64)1);
	BigInteger tempNum;
	bool thisNegative = false;

	if ((this->data[maxLength - 1] & 0x80000000) != 0)   //底数是负数
	{
		tempNum = -(*this) % n;
		thisNegative = true;
	}
	else
		tempNum = (*this) % n;  //保证(tempNum * tempNum) < b^(2k)

	if ((n.data[maxLength - 1] & 0x80000000) != 0)   //n为负
		n = -n;

	//计算 constant = b^(2k) / m
	//constant主要用于后面的Baeert Reduction算法
	BigInteger constant;

	int i = n.dataLength << 1;
	constant.data[i] = 0x00000001;
	constant.dataLength = i + 1;

	constant = constant / n;
	int totalBits = exp.bitCount();
	int count = 0;

	//平方乘法算法
	for (int pos = 0; pos < exp.dataLength; pos++)
	{
		unsigned int mask = 0x01;

		for (int index = 0; index < 32; index++)
		{
			if ((exp.data[pos] & mask) != 0)  //某一个bit不为0
				resultNum = BarrettReduction(resultNum * tempNum, n, constant);
			//resultNum = resultNum * tempNum mod n

			mask <<= 1; //不断左移

			tempNum = BarrettReduction(tempNum * tempNum, n, constant);
			//tempNum = tempNum * tempNum mod n

			if (tempNum.dataLength == 1 && tempNum.data[0] == 1)
			{
				if (thisNegative && (exp.data[0] & 0x1) != 0)    //指数为奇数
					return -resultNum;
				return resultNum;
			}
			count++;
			if (count == totalBits)
				break;
		}
	}

	if (thisNegative && (exp.data[0] & 0x1) != 0)    //底数为负数,指数为奇数
		return -resultNum;

	return resultNum;
}

int BigInteger::bitCount()   //计算字节数
{
	while (dataLength > 1 && data[dataLength - 1] == 0)
		dataLength--;

	unsigned int value = data[dataLength - 1];
	unsigned int mask = 0x80000000;
	int bits = 32;

	while (bits > 0 && (value & mask) == 0)   //计算最高位的bit
	{
		bits--;
		mask >>= 1;
	}
	bits += ((dataLength - 1) << 5);   //余下的位都有32bit
	//左移5位,相当于乘以32,即2^5
	return bits;
}

BigInteger BigInteger::BarrettReduction(BigInteger x, BigInteger n, BigInteger constant)
{

	//算法,Baeert Reduction算法,在计算大规模的除法运算时很有优势
	//原理如下
	//Z mod N=Z-[Z/N]*N=Z-{[Z/b^(n-1)]*[b^2n/N]/b^(n+1)}*N=Z-q*N
	//q=[Z/b^(n-1)]*[b^2n/N]/b^(n+1)
	//其中,[]表示取整运算,A^B表示A的B次幂

	int k = n.dataLength,
		kPlusOne = k + 1,
		kMinusOne = k - 1;

	BigInteger q1;

	// q1 = x / b^(k-1)
	for (int i = kMinusOne, j = 0; i < x.dataLength; i++, j++)
		q1.data[j] = x.data[i];
	q1.dataLength = x.dataLength - kMinusOne;
	if (q1.dataLength <= 0)
		q1.dataLength = 1;


	BigInteger q2 = q1 * constant;
	BigInteger q3;

	// q3 = q2 / b^(k+1)
	for (int i = kPlusOne, j = 0; i < q2.dataLength; i++, j++)
		q3.data[j] = q2.data[i];
	q3.dataLength = q2.dataLength - kPlusOne;
	if (q3.dataLength <= 0)
		q3.dataLength = 1;


	// r1 = x mod b^(k+1)
	// i.e. keep the lowest (k+1) words
	BigInteger r1;
	int lengthToCopy = (x.dataLength > kPlusOne) ? kPlusOne : x.dataLength;
	for (int i = 0; i < lengthToCopy; i++)
		r1.data[i] = x.data[i];
	r1.dataLength = lengthToCopy;


	// r2 = (q3 * n) mod b^(k+1)
	// partial multiplication of q3 and n

	BigInteger r2;
	for (int i = 0; i < q3.dataLength; i++)
	{
		if (q3.data[i] == 0) continue;

		unsigned __int64 mcarry = 0;
		int t = i;
		for (int j = 0; j < n.dataLength && t < kPlusOne; j++, t++)
		{
			// t = i + j
			unsigned __int64 val = ((unsigned __int64)q3.data[i] * (unsigned __int64)n.data[j]) +
				(unsigned __int64)r2.data[t] + mcarry;

			r2.data[t] = (unsigned int)(val & 0xFFFFFFFF);
			mcarry = (val >> 32);
		}

		if (t < kPlusOne)
			r2.data[t] = (unsigned int)mcarry;
	}
	r2.dataLength = kPlusOne;
	while (r2.dataLength > 1 && r2.data[r2.dataLength - 1] == 0)
		r2.dataLength--;

	r1 -= r2;
	if ((r1.data[maxLength - 1] & 0x80000000) != 0)        // negative
	{
		BigInteger val;
		val.data[kPlusOne] = 0x00000001;
		val.dataLength = kPlusOne + 1;
		r1 += val;
	}

	while (r1 >= n)
		r1 -= n;

	return r1;
}

bool BigInteger::operator >(BigInteger bi2)
{
	int pos = maxLength - 1;
	BigInteger bi1(*this);

	// bi1 is negative, bi2 is positive
	if ((bi1.data[pos] & 0x80000000) != 0 && (bi2.data[pos] & 0x80000000) == 0)
		return false;

	// bi1 is positive, bi2 is negative
	else if ((bi1.data[pos] & 0x80000000) == 0 && (bi2.data[pos] & 0x80000000) != 0)
		return true;

	// same sign
	int len = (bi1.dataLength > bi2.dataLength) ? bi1.dataLength : bi2.dataLength;
	for (pos = len - 1; pos >= 0 && bi1.data[pos] == bi2.data[pos]; pos--);

	if (pos >= 0)
	{
		if (bi1.data[pos] > bi2.data[pos])
			return true;
		return false;
	}
	return false;
}

bool BigInteger::operator ==(BigInteger bi2)
{
	if (this->dataLength != bi2.dataLength)
		return false;

	for (int i = 0; i < this->dataLength; i++)
	{
		if (this->data[i] != bi2.data[i])
			return false;
	}
	return true;
}

bool BigInteger::operator !=(BigInteger bi2)
{
	if (this->dataLength != bi2.dataLength)
		return true;
	for (int i = 0; i < this->dataLength; i++)
	{
		if (this->data[i] != bi2.data[i])
			return true;
	}
	return false;
}

BigInteger BigInteger::operator %(BigInteger bi2)
{
	BigInteger bi1(*this);
	BigInteger quotient;
	BigInteger remainder(bi1);

	int lastPos = maxLength - 1;
	bool dividendNeg = false;

	if ((bi1.data[lastPos] & 0x80000000) != 0)     // bi1 negative
	{
		bi1 = -bi1;
		dividendNeg = true;
	}
	if ((bi2.data[lastPos] & 0x80000000) != 0)     // bi2 negative
		bi2 = -bi2;

	if (bi1 < bi2)
	{
		return remainder;
	}

	else
	{
		if (bi2.dataLength == 1)
			singleByteDivide(bi1, bi2, quotient, remainder);   //bi2只占一个位置时,用singleByteDivide更快
		else
			multiByteDivide(bi1, bi2, quotient, remainder);   //bi2占多个位置时,用multiByteDivide更快

		if (dividendNeg)
			return -remainder;

		return remainder;
	}
}

void BigInteger::singleByteDivide(BigInteger &bi1, BigInteger &bi2,
	BigInteger &outQuotient, BigInteger &outRemainder)
{//outQuotient商,outRemainder余数

	unsigned int result[maxLength];   //用来存储结果
	memset(result, 0, sizeof(unsigned int)* maxLength);
	int resultPos = 0;

	for (int i = 0; i < maxLength; i++)   //将bi1复制至outRemainder
		outRemainder.data[i] = bi1.data[i];
	outRemainder.dataLength = bi1.dataLength;

	while (outRemainder.dataLength > 1 && outRemainder.data[outRemainder.dataLength - 1] == 0)
		outRemainder.dataLength--;

	unsigned __int64 divisor = (unsigned __int64)bi2.data[0];
	int pos = outRemainder.dataLength - 1;
	unsigned __int64 dividend = (unsigned __int64)outRemainder.data[pos];   //取最高位的数值


	if (dividend >= divisor)   //被除数>除数
	{
		unsigned __int64 quotient = dividend / divisor;
		result[resultPos++] = (unsigned __int64)quotient;   //结果

		outRemainder.data[pos] = (unsigned __int64)(dividend % divisor);   //余数
	}
	pos--;

	while (pos >= 0)
	{
		dividend = ((unsigned __int64)outRemainder.data[pos + 1] << 32) + (unsigned __int64)outRemainder.data[pos];   //前一位的余数和这一位的值相加
		unsigned __int64 quotient = dividend / divisor;   //得到结果
		result[resultPos++] = (unsigned int)quotient;   //结果取低位

		outRemainder.data[pos + 1] = 0;   //前一位的余数清零
		outRemainder.data[pos--] = (unsigned int)(dividend % divisor);   //得到这一位的余数
	}

	outQuotient.dataLength = resultPos;   //商的长度是resultPos的长度
	int j = 0;
	for (int i = outQuotient.dataLength - 1; i >= 0; i--, j++)  //将商反转过来 
		outQuotient.data[j] = result[i];
	for (; j < maxLength; j++)   //商的其余位都要置0
		outQuotient.data[j] = 0;

	while (outQuotient.dataLength > 1 && outQuotient.data[outQuotient.dataLength - 1] == 0)
		outQuotient.dataLength--;

	if (outQuotient.dataLength == 0)
		outQuotient.dataLength = 1;

	while (outRemainder.dataLength > 1 && outRemainder.data[outRemainder.dataLength - 1] == 0)
		outRemainder.dataLength--;
}

void BigInteger::multiByteDivide(BigInteger &bi1, BigInteger &bi2,
	BigInteger &outQuotient, BigInteger &outRemainder)
{
	unsigned int result[maxLength];
	memset(result, 0, sizeof(unsigned int)* maxLength);   //结果置零 

	int remainderLen = bi1.dataLength + 1;   //余数长度
	unsigned int *remainder = new unsigned int[remainderLen];
	memset(remainder, 0, sizeof(unsigned int)* remainderLen);   //余数置零

	unsigned int mask = 0x80000000;
	unsigned int val = bi2.data[bi2.dataLength - 1];
	int shift = 0, resultPos = 0;

	while (mask != 0 && (val & mask) == 0)
	{
		shift++; mask >>= 1;
	}
	//最高位从高到低找出shift个0位
	for (int i = 0; i < bi1.dataLength; i++)
		remainder[i] = bi1.data[i];   //将bi1复制到remainder之中
	this->shiftLeft(remainder, remainderLen, shift);   //remainder左移shift位
	bi2 = bi2 << shift;   //向左移shift位,将空位填满
	//由于两个数都扩大了相同的倍数,所以结果不变

	int j = remainderLen - bi2.dataLength;   //j表示两个数长度的差值,也是要计算的次数
	int pos = remainderLen - 1;   //pos指示余数的最高位的位置,现在pos=bi1.dataLength

	//以下的步骤并没有别的意思,主要是用来试商
	unsigned __int64 firstDivisorByte = bi2.data[bi2.dataLength - 1];   //第一个除数
	unsigned __int64 secondDivisorByte = bi2.data[bi2.dataLength - 2];  //第二个除数 

	int divisorLen = bi2.dataLength + 1;   //除数的长度
	unsigned int *dividendPart = new unsigned int[divisorLen];   //起名为除数的部分
	memset(dividendPart, 0, sizeof(unsigned int)* divisorLen);

	while (j > 0)
	{
		unsigned __int64 dividend = ((unsigned __int64)remainder[pos] << 32) + (unsigned __int64)remainder[pos - 1];   //取余数的高两位

		unsigned __int64 q_hat = dividend / firstDivisorByte;   //得到一个商
		unsigned __int64 r_hat = dividend % firstDivisorByte;   //以及一个余数

		bool done = false;   //表示没有做完
		while (!done)
		{
			done = true;

			if (q_hat == 0x100000000 || (q_hat * secondDivisorByte) > ((r_hat << 32) + remainder[pos - 2]))   //这里主要用来调整商的大小 
				//(q_hat * secondDivisorByte) > ((r_hat << 32) + remainder[pos - 2]))是害怕上的商过大,减之后变为负数
				//商q_hat也不能超过32bit
			{
				q_hat--;   //否则的话,就商小一点,余数大一点
				r_hat += firstDivisorByte;

				if (r_hat < 0x100000000)   //如果余数小于32bit,就继续循环
					done = false;
			}
		}

		for (int h = 0; h < divisorLen; h++)   //取被除数的高位部分,高位部分长度与除数长度一致
			dividendPart[h] = remainder[pos - h];

		BigInteger kk(dividendPart, divisorLen);
		BigInteger ss = bi2 * BigInteger((__int64)q_hat);

		while (ss > kk)   //调节商的大小
		{
			q_hat--;
			ss -= bi2;
		}
		BigInteger yy = kk - ss;   //得到余数

		for (int h = 0; h < divisorLen; h++)  //将yy高位和remainder低位拼接起来,得到余数
			remainder[pos - h] = yy.data[bi2.dataLength - h];   //取得真正的余数

		result[resultPos++] = (unsigned int)q_hat;

		pos--;
		j--;
	}

	outQuotient.dataLength = resultPos;
	int y = 0;
	for (int x = outQuotient.dataLength - 1; x >= 0; x--, y++)   //将商反转过来
		outQuotient.data[y] = result[x];
	for (; y < maxLength; y++)   //商的其余位都要置0
		outQuotient.data[y] = 0;

	while (outQuotient.dataLength > 1 && outQuotient.data[outQuotient.dataLength - 1] == 0)
		outQuotient.dataLength--;

	if (outQuotient.dataLength == 0)
		outQuotient.dataLength = 1;

	outRemainder.dataLength = this->shiftRight(remainder, remainderLen, shift);

	for (y = 0; y < outRemainder.dataLength; y++)
		outRemainder.data[y] = remainder[y];
	for (; y < maxLength; y++)
		outRemainder.data[y] = 0;

	delete[]remainder;
	delete[]dividendPart;
}

int BigInteger::shiftRight(unsigned int buffer[], int bufferLen, int shiftVal)   //右移操作
{//自己用图画模拟一下移位操作,就能很快明白意义了
	int shiftAmount = 32;
	int invShift = 0;
	int bufLen = bufferLen;

	while (bufLen > 1 && buffer[bufLen - 1] == 0)
		bufLen--;

	for (int count = shiftVal; count > 0;)
	{
		if (count < shiftAmount)
		{
			shiftAmount = count;
			invShift = 32 - shiftAmount;
		}

		unsigned __int64 carry = 0;
		for (int i = bufLen - 1; i >= 0; i--)
		{
			unsigned __int64 val = ((unsigned __int64)buffer[i]) >> shiftAmount;
			val |= carry;

			carry = ((unsigned __int64)buffer[i]) << invShift;
			buffer[i] = (unsigned int)(val);
		}

		count -= shiftAmount;
	}

	while (bufLen > 1 && buffer[bufLen - 1] == 0)
		bufLen--;

	return bufLen;
}

BigInteger BigInteger::operator <<(int shiftVal)
{
	BigInteger result(*this);
	result.dataLength = shiftLeft(result.data, maxLength, shiftVal);

	return result;
}

int BigInteger::shiftLeft(unsigned int buffer[], int bufferLen, int shiftVal)
{
	int shiftAmount = 32;
	int bufLen = bufferLen;

	while (bufLen > 1 && buffer[bufLen - 1] == 0)
		bufLen--;

	for (int count = shiftVal; count > 0;)
	{
		if (count < shiftAmount)
			shiftAmount = count;

		unsigned __int64 carry = 0;
		for (int i = 0; i < bufLen; i++)
		{
			unsigned __int64 val = ((unsigned __int64)buffer[i]) << shiftAmount;
			val |= carry;

			buffer[i] = (unsigned int)(val & 0xFFFFFFFF);
			carry = val >> 32;
		}

		if (carry != 0)
		{
			if (bufLen + 1 <= bufferLen)
			{
				buffer[bufLen] = (unsigned int)carry;
				bufLen++;
			}
		}
		count -= shiftAmount;
	}
	return bufLen;
}

bool BigInteger::operator <(BigInteger bi2)
{
	BigInteger bi1(*this);
	int pos = maxLength - 1;

	// bi1 is negative, bi2 is positive
	if ((bi1.data[pos] & 0x80000000) != 0 && (bi2.data[pos] & 0x80000000) == 0)
		return true;

	// bi1 is positive, bi2 is negative
	else if ((bi1.data[pos] & 0x80000000) == 0 && (bi2.data[pos] & 0x80000000) != 0)
		return false;

	// same sign
	int len = (bi1.dataLength > bi2.dataLength) ? bi1.dataLength : bi2.dataLength;
	for (pos = len - 1; pos >= 0 && bi1.data[pos] == bi2.data[pos]; pos--);

	if (pos >= 0)
	{
		if (bi1.data[pos] < bi2.data[pos])
			return true;
		return false;
	}
	return false;
}

BigInteger BigInteger::operator +=(BigInteger bi2)
{
	*this = *this + bi2;
	return *this;
}

BigInteger BigInteger::operator /(BigInteger bi2)
{
	BigInteger bi1(*this);
	BigInteger quotient;
	BigInteger remainder;

	int lastPos = maxLength - 1;
	bool divisorNeg = false, dividendNeg = false;

	if ((bi1.data[lastPos] & 0x80000000) != 0)     // bi1 negative
	{
		bi1 = -bi1;
		dividendNeg = true;
	}
	if ((bi2.data[lastPos] & 0x80000000) != 0)     // bi2 negative
	{
		bi2 = -bi2;
		divisorNeg = true;
	}

	if (bi1 < bi2)
	{
		return quotient;
	}

	else
	{
		if (bi2.dataLength == 1)
			singleByteDivide(bi1, bi2, quotient, remainder);
		else
			multiByteDivide(bi1, bi2, quotient, remainder);

		if (dividendNeg != divisorNeg)
			return -quotient;

		return quotient;
	}
}

BigInteger BigInteger::operator -=(BigInteger bi2)
{
	*this = *this - bi2;
	return *this;
}

BigInteger BigInteger::operator -(BigInteger bi2)   //减法的重载
{
	BigInteger bi1(*this);
	BigInteger result;
	int lastPos = maxLength - 1;
	bool bi1Neg = false, bi2Neg = false;

	if ((this->data[lastPos] & 0x80000000) != 0)     // bi1 negative
		bi1Neg = true;
	if ((bi2.data[lastPos] & 0x80000000) != 0)     // bi1 negative
		bi2Neg = true;

	if (bi1Neg == false && bi2Neg == false)   //bi1,bi2都为正数
	{
		if (bi1 < bi2)
		{
			result = -(bi2 - bi1);
			return result;
		}
		result.dataLength = (bi1.dataLength > bi2.dataLength) ? bi1.dataLength : bi2.dataLength;

		__int64 carryIn = 0;
		for (int i = 0; i < result.dataLength; i++)   //从低位开始减
		{
			__int64 diff;

			diff = (__int64)bi1.data[i] - (__int64)bi2.data[i] - carryIn;
			result.data[i] = (unsigned int)(diff & 0xFFFFFFFF);

			if (diff < 0)
				carryIn = 1;
			else
				carryIn = 0;
		}

		if (carryIn != 0)
		{
			for (int i = result.dataLength; i < maxLength; i++)
				result.data[i] = 0xFFFFFFFF;
			result.dataLength = maxLength;
		}

		// fixed in v1.03 to give correct datalength for a - (-b)
		while (result.dataLength > 1 && result.data[result.dataLength - 1] == 0)
			result.dataLength--;

		// overflow check

		if ((bi1.data[lastPos] & 0x80000000) != (bi2.data[lastPos] & 0x80000000) &&
			(result.data[lastPos] & 0x80000000) != (bi1.data[lastPos] & 0x80000000))
		{
			assert(false);
		}

		return result;
	}

	if (bi1Neg == true && bi2Neg == false)    //bi1负,bi2正
	{
		result = -(-bi1 + bi2);
		return result;
	}

	if (bi1Neg == false && bi2Neg == true)   //bi1正,bi2负
	{
		result = bi1 + (-bi2);
		return result;
	}

	if (bi1Neg == true && bi2Neg == true)   //bi1,bi2皆为负
	{
		BigInteger bi3 = -bi1, bi4 = -bi2;
		if (bi3 > bi4)
		{
			result = -(bi3 - bi4);
			return result;
		}
		else
		{
			result = bi4 - bi3;
			return result;
		}
	}
}

string BigInteger::DecToHex(unsigned int value, string format)   //10进制数转换成16进制数,并用string表示
{
	string HexStr;
	int a[100];
	int i = 0;
	int m = 0;
	int mod = 0;
	char hex[16] = { '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F' };
	while (value > 0)
	{
		mod = value % 16;
		a[i++] = mod;
		value = value / 16;

	}

	for (i = i - 1; i >= 0; i--)
	{
		m = a[i];
		HexStr.push_back(hex[m]);
	}

	while (format == string("X8") && HexStr.size() < 8)
	{
		HexStr = "0" + HexStr;
	}

	return HexStr;
}

string BigInteger::ToHexString()  //功能：将一个大数用16进制的string表示出来
{
	string result = DecToHex(data[dataLength - 1], string("X"));

	for (int i = dataLength - 2; i >= 0; i--)
	{
		result += DecToHex(data[i], string("X8"));
	}

	return result;
}

ostream& operator<<(ostream& output, BigInteger &obj)//以16进制输出数值
{
	//if ((obj.data[obj.dataLength-1] & 0x80000000) != 0)     // bi1 negative
	for (int i = obj.dataLength - 1; i >= 0; i--)
		output << hex << obj.data[i];
	return output;
}


bool Miller_Robin(BigInteger &bi1)   //Miller_Robin算法
{
	BigInteger one((__int64)1), two((__int64)2), sum, a, b, temp;
	int k = 0, len = primeLength / 2;
	temp = sum = bi1 - one;
	while ((sum.data[0] & 0x00000001) == 0)   //只要sum不为奇数,sum就一直往右移
	{
		sum.dataLength = sum.shiftRight(sum.data, maxLength, 1);   //右移一位
		k++;
	}
	//sum即为要求的奇数,k即是要求的2的次数
	srand((unsigned)time(0));
	for (int i = 0; i < len; i++)
	{
		a.data[i] = (unsigned int)rand();
		if (a.data[i] != 0) a.dataLength = i + 1;
	}

	b = a.modPow(sum, bi1);  //b = a^m mod bi1
	if (b == one) return true;

	for (int i = 0; i < k; i++)
	{
		if (b == temp) return true;
		else b = b.modPow(two, bi1);  //b = b^2 mod bi1
	}
	return false;
}

bool IsPrime(BigInteger &obj)
{
	BigInteger zero;
	for (int i = 0; i < 303; i++)   //先用一些素数对这个整数进行筛选
	{
		BigInteger prime((__int64)primesBelow2000[i]);
		if (obj % prime == zero)
			return false;
	}
	cout << "第一轮素性检验通过… … … …" << endl;
	cout << "正在进行Miller_Robin素性检验… … … …" << endl;
	if (Miller_Robin(obj))   //进行1次Miller_Robin检验
		return true;  //通过了就返回result	  
	return false;//表明result是合数,没有通过检验
}

BigInteger GetPrime()
{
	BigInteger one((__int64)1), two((__int64)2), result;
	srand((unsigned)time(0));

	//随机产生一个大整数
	for (int i = 0; i < primeLength; i++)
	{
		result.data[i] = (unsigned int)rand();
		if (result.data[i] != 0)
			result.dataLength = i + 1;
	}

	result.data[0] |= 0x00000001;   //保证这个整数为奇数
	while (!IsPrime(result))   //如果没有通过检验,就+2,继续检验
	{
		result = result + two;

		cout << "检验没有通过,进行下一个数的检验,运行之中… … … …" << endl;
		cout << endl;
	}
	return result;

}




BigInteger extended_euclidean(BigInteger n, BigInteger m, BigInteger &x, BigInteger &y)   //扩展的欧几里德算法  
{
	BigInteger x1((__int64)1), x2, x3(n);
	BigInteger y1, y2((__int64)1), y3(m);
	BigInteger zero;
	while (x3 % y3 != zero)
	{
		BigInteger d = x3 / y3;
		BigInteger t1, t2, t3;
		t1 = x1 - d * y1;
		t2 = x2 - d * y2;
		t3 = x3 - d * y3;
		x1 = y1; x2 = y2; x3 = y3;
		y1 = t1; y2 = t2; y3 = t3;
	}
	x = y1; y = y2;
	return y3;
}

/*
BigInteger extended_euclidean(BigInteger n,BigInteger m,BigInteger &x,BigInteger &y)
{
BigInteger zero, one((__int64)1);
if(m == zero) { x = one; y = zero; return n; }
BigInteger g = extended_euclidean(m, n%m, x, y);
BigInteger t = x - n / m * y;
x = y;
y = t;
return g;
}
*/

BigInteger Gcd(BigInteger &bi1, BigInteger &bi2)
{
	BigInteger x, y;
	BigInteger g = extended_euclidean(bi1, bi2, x, y);
	return g;
}

BigInteger MultipInverse(BigInteger &bi1, BigInteger &n)   //求乘法逆元
{
	BigInteger x, y;
	extended_euclidean(bi1, n, x, y);
	if ((x.data[maxLength - 1] & 0x80000000) != 0)     // x negative
		x = x + n;
	// unsigned int i =  x.data[maxLength-1] & 0x80000000;
	// cout << i << endl;
	return x;
}