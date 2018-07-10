#include "BigInteger.h"

BigInteger::BigInteger(void)   //Ĭ�ϵĹ��캯��
: dataLength(0), data(0)
{
	data = new unsigned int[maxLength];
	memset(data, 0, maxLength * sizeof(unsigned int));
	dataLength = 1;
}

BigInteger::BigInteger(__int64 value)   //��һ��64λ��ֵ����ʼ������
{
	data = new unsigned int[maxLength];
	memset(data, 0, maxLength * sizeof(unsigned int));   //������
	__int64 tempVal = value;

	dataLength = 0;
	while (value != 0 && dataLength < maxLength)
	{
		data[dataLength] = (unsigned int)(value & 0xFFFFFFFF);   //ȡ��λ
		value = value >> 32;   //��λ
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

BigInteger::BigInteger(unsigned __int64 value)   //��һ���޷��ŵ�64λ��������ʼ������
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

	if (dataLength == 0)   //��ֹ�����value=0
		dataLength = 1;
}

BigInteger::BigInteger(const BigInteger &bi)   //�ô�����ʼ������
{
	data = new unsigned int[maxLength];
	dataLength = bi.dataLength;

	for (int i = 0; i < maxLength; i++)   //���ǵ��и��������,����ÿһλ��Ҫ����
		data[i] = bi.data[i];
}

BigInteger::~BigInteger(void)
{
	if (data != NULL)
	{
		delete[]data;
	}
}

BigInteger::BigInteger(string value, int radix)   //����ת������,���ַ���ת���ɶ�Ӧ���ƵĴ���
{   //һ�㲻������
	BigInteger multiplier((__int64)1);
	BigInteger result;
	transform(value.begin(), value.end(), value.begin(), toupper);   //��Сд��ĸת����Ϊ��д

	int limit = 0;

	if (value[0] == '-')
		limit = 1;

	for (int i = value.size() - 1; i >= limit; i--)
	{
		int posVal = (int)value[i];

		if (posVal >= '0' && posVal <= '9')  //���ַ�ת��������
			posVal -= '0';
		else if (posVal >= 'A' && posVal <= 'Z')
			posVal = (posVal - 'A') + 10;
		else
			posVal = 9999999;       // arbitrary large �������ַ�


		if (posVal >= radix)   //���ܴ����ض��Ľ���,������ֹ
		{
			assert(false);
		}
		else
		{
			result = result + (multiplier * BigInteger((__int64)posVal));

			if ((i - 1) >= limit)   //û�е���β��
				multiplier = multiplier * BigInteger((__int64)radix);
		}
	}

	if (value[0] == '-')   //��������ٴ���
		result = -result;

	if (value[0] == '-')     //����Ϊ����,���õ��Ľ��Ϊ����,���������
	{
		if ((result.data[maxLength - 1] & 0x80000000) == 0)
			assert(false);
	}
	else    //����˵,����Ϊ����,�õ��Ľ��Ϊ����,Ҳ���������
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

BigInteger::BigInteger(byte inData[], int inLen)   //��һ��char���͵���������ʼ������
{
	dataLength = inLen >> 2;   //һ��unsigned intռ32λ,��һ��unsigned charֻռ8λ
	//���dataLengthӦ����������inLen/4,��һ������

	int leftOver = inLen & 0x3;
	//ȡ�����λ����ֵ,ΪʲôҪ�������أ�ʵ������Ϊ��̽��len�ǲ���4�ı���,��ȷ��dataLength�ĳ���
	if (leftOver != 0)    //���������Ļ�,dataLengthҪ��1
		dataLength++;


	if (dataLength > maxLength)
		assert(false);

	data = new unsigned int[maxLength];
	memset(data, 0, maxLength * sizeof(unsigned int));

	for (int i = inLen - 1, j = 0; i >= 3; i -= 4, j++)
	{
		data[j] = (unsigned int)((inData[i - 3] << 24) + (inData[i - 2] << 16) + (inData[i - 1] << 8) + inData[i]);
		//����֪��:һ��unsigned intռ32λ,��һ��unsigned charֻռ8λ,����ĸ�unsigned char�������һ��unsigned int
		//���ȡinData[i - 3]Ϊǰ32-25λ,inData[i - 2]Ϊǰ24-17~~~
		//i % 4 = 0 or 1 or 2 or 3 ��0��ʾǡ�ñ�ʾ��
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

BigInteger::BigInteger(unsigned int inData[], int inLen)   //��һ��unsigned int�������ʼ������
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

BigInteger BigInteger::operator *(BigInteger bi2)   //�˷�������
{
	BigInteger bi1(*this);
	int lastPos = maxLength - 1;
	bool bi1Neg = false, bi2Neg = false;

	//���ȶ���������ȡ����ֵ
	try
	{
		if ((this->data[lastPos] & 0x80000000) != 0)     //bi1Ϊ����
		{
			bi1Neg = true;
			bi1 = -bi1;
		}
		if ((bi2.data[lastPos] & 0x80000000) != 0)     //bi2Ϊ����
		{
			bi2Neg = true; bi2 = -bi2;
		}
	}
	catch (...) {}

	BigInteger result;

	//����ֵ���
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
				result.data[k] = (unsigned __int64)(val & 0xFFFFFFFF);   //ȡ��λ
				mcarry = (val >> 32);   //��λ
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

	// overflow check (result is -ve)������
	if ((result.data[lastPos] & 0x80000000) != 0)  //���Ϊ����
	{
		if (bi1Neg != bi2Neg && result.data[lastPos] == 0x80000000)    //���������Ų�ͬ
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

	//���������Ų�ͬ,���Ϊ����
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

	if ((this->data[lastPos] & 0x80000000) != 0)     //bi1Ϊ����
		bi1Neg = true;
	if ((bi2.data[lastPos] & 0x80000000) != 0)     //bi2Ϊ����
		bi2Neg = true;

	if (bi1Neg == false && bi2Neg == false)   //bi1��bi2��������
	{
		result.dataLength = (this->dataLength > bi2.dataLength) ? this->dataLength : bi2.dataLength;

		__int64 carry = 0;
		for (int i = 0; i < result.dataLength; i++)   //�ӵ�λ��ʼ,��λ���
		{
			__int64 sum = (__int64)this->data[i] + (__int64)bi2.data[i] + carry;
			carry = sum >> 32;   //��λ
			result.data[i] = (unsigned int)(sum & 0xFFFFFFFF);  //ȡ��λ���
		}

		if (carry != 0 && result.dataLength < maxLength)
		{
			result.data[result.dataLength] = (unsigned int)(carry);
			result.dataLength++;
		}

		while (result.dataLength > 1 && result.data[result.dataLength - 1] == 0)
			result.dataLength--;


		//������
		if ((this->data[lastPos] & 0x80000000) == (bi2.data[lastPos] & 0x80000000) &&
			(result.data[lastPos] & 0x80000000) != (this->data[lastPos] & 0x80000000))
		{
			assert(false);
		}
		return result;
	}
	//�ؼ�����,����ȫ��Ҫת����Ϊ��������
	if (bi1Neg == false && bi2Neg == true)   //bi1��,bi2��
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

	if (bi1Neg == true && bi2Neg == false)  //bi1��,bi2��
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

	if (bi1Neg == true && bi2Neg == true)  //bi1��,bi2��
	{
		result = -((-bi1) + (-bi2));
		return result;
	}
}

BigInteger BigInteger::operator -()
{
	//��λȡ����+1
	if (this->dataLength == 1 && this->data[0] == 0)
		return *this;

	BigInteger result(*this);

	for (int i = 0; i < maxLength; i++)
		result.data[i] = (unsigned int)(~(this->data[i]));   //ȡ��

	__int64 val, carry = 1;
	int index = 0;

	while (carry != 0 && index < maxLength)  //+1��
	{
		val = (__int64)(result.data[index]);
		val++;   //����ֵ����1��1,��ǰ��Ľ�λ���Ҳֻ��1��1,���val++������

		result.data[index] = (unsigned int)(val & 0xFFFFFFFF);   //ȡ��λ����
		carry = val >> 32;   //��λ

		index++;
	}

	if ((this->data[maxLength - 1] & 0x80000000) == (result.data[maxLength - 1] & 0x80000000))
		result.dataLength = maxLength;

	while (result.dataLength > 1 && result.data[result.dataLength - 1] == 0)
		result.dataLength--;

	return result;
}

BigInteger BigInteger::modPow(BigInteger exp, BigInteger n)   //��this^exp mod n
{
	if ((exp.data[maxLength - 1] & 0x80000000) != 0)   //ָ���Ǹ���
		return BigInteger((__int64)0);

	BigInteger resultNum((__int64)1);
	BigInteger tempNum;
	bool thisNegative = false;

	if ((this->data[maxLength - 1] & 0x80000000) != 0)   //�����Ǹ���
	{
		tempNum = -(*this) % n;
		thisNegative = true;
	}
	else
		tempNum = (*this) % n;  //��֤(tempNum * tempNum) < b^(2k)

	if ((n.data[maxLength - 1] & 0x80000000) != 0)   //nΪ��
		n = -n;

	//���� constant = b^(2k) / m
	//constant��Ҫ���ں����Baeert Reduction�㷨
	BigInteger constant;

	int i = n.dataLength << 1;
	constant.data[i] = 0x00000001;
	constant.dataLength = i + 1;

	constant = constant / n;
	int totalBits = exp.bitCount();
	int count = 0;

	//ƽ���˷��㷨
	for (int pos = 0; pos < exp.dataLength; pos++)
	{
		unsigned int mask = 0x01;

		for (int index = 0; index < 32; index++)
		{
			if ((exp.data[pos] & mask) != 0)  //ĳһ��bit��Ϊ0
				resultNum = BarrettReduction(resultNum * tempNum, n, constant);
			//resultNum = resultNum * tempNum mod n

			mask <<= 1; //��������

			tempNum = BarrettReduction(tempNum * tempNum, n, constant);
			//tempNum = tempNum * tempNum mod n

			if (tempNum.dataLength == 1 && tempNum.data[0] == 1)
			{
				if (thisNegative && (exp.data[0] & 0x1) != 0)    //ָ��Ϊ����
					return -resultNum;
				return resultNum;
			}
			count++;
			if (count == totalBits)
				break;
		}
	}

	if (thisNegative && (exp.data[0] & 0x1) != 0)    //����Ϊ����,ָ��Ϊ����
		return -resultNum;

	return resultNum;
}

int BigInteger::bitCount()   //�����ֽ���
{
	while (dataLength > 1 && data[dataLength - 1] == 0)
		dataLength--;

	unsigned int value = data[dataLength - 1];
	unsigned int mask = 0x80000000;
	int bits = 32;

	while (bits > 0 && (value & mask) == 0)   //�������λ��bit
	{
		bits--;
		mask >>= 1;
	}
	bits += ((dataLength - 1) << 5);   //���µ�λ����32bit
	//����5λ,�൱�ڳ���32,��2^5
	return bits;
}

BigInteger BigInteger::BarrettReduction(BigInteger x, BigInteger n, BigInteger constant)
{

	//�㷨,Baeert Reduction�㷨,�ڼ�����ģ�ĳ�������ʱ��������
	//ԭ������
	//Z mod N=Z-[Z/N]*N=Z-{[Z/b^(n-1)]*[b^2n/N]/b^(n+1)}*N=Z-q*N
	//q=[Z/b^(n-1)]*[b^2n/N]/b^(n+1)
	//����,[]��ʾȡ������,A^B��ʾA��B����

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
			singleByteDivide(bi1, bi2, quotient, remainder);   //bi2ֻռһ��λ��ʱ,��singleByteDivide����
		else
			multiByteDivide(bi1, bi2, quotient, remainder);   //bi2ռ���λ��ʱ,��multiByteDivide����

		if (dividendNeg)
			return -remainder;

		return remainder;
	}
}

void BigInteger::singleByteDivide(BigInteger &bi1, BigInteger &bi2,
	BigInteger &outQuotient, BigInteger &outRemainder)
{//outQuotient��,outRemainder����

	unsigned int result[maxLength];   //�����洢���
	memset(result, 0, sizeof(unsigned int)* maxLength);
	int resultPos = 0;

	for (int i = 0; i < maxLength; i++)   //��bi1������outRemainder
		outRemainder.data[i] = bi1.data[i];
	outRemainder.dataLength = bi1.dataLength;

	while (outRemainder.dataLength > 1 && outRemainder.data[outRemainder.dataLength - 1] == 0)
		outRemainder.dataLength--;

	unsigned __int64 divisor = (unsigned __int64)bi2.data[0];
	int pos = outRemainder.dataLength - 1;
	unsigned __int64 dividend = (unsigned __int64)outRemainder.data[pos];   //ȡ���λ����ֵ


	if (dividend >= divisor)   //������>����
	{
		unsigned __int64 quotient = dividend / divisor;
		result[resultPos++] = (unsigned __int64)quotient;   //���

		outRemainder.data[pos] = (unsigned __int64)(dividend % divisor);   //����
	}
	pos--;

	while (pos >= 0)
	{
		dividend = ((unsigned __int64)outRemainder.data[pos + 1] << 32) + (unsigned __int64)outRemainder.data[pos];   //ǰһλ����������һλ��ֵ���
		unsigned __int64 quotient = dividend / divisor;   //�õ����
		result[resultPos++] = (unsigned int)quotient;   //���ȡ��λ

		outRemainder.data[pos + 1] = 0;   //ǰһλ����������
		outRemainder.data[pos--] = (unsigned int)(dividend % divisor);   //�õ���һλ������
	}

	outQuotient.dataLength = resultPos;   //�̵ĳ�����resultPos�ĳ���
	int j = 0;
	for (int i = outQuotient.dataLength - 1; i >= 0; i--, j++)  //���̷�ת���� 
		outQuotient.data[j] = result[i];
	for (; j < maxLength; j++)   //�̵�����λ��Ҫ��0
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
	memset(result, 0, sizeof(unsigned int)* maxLength);   //������� 

	int remainderLen = bi1.dataLength + 1;   //��������
	unsigned int *remainder = new unsigned int[remainderLen];
	memset(remainder, 0, sizeof(unsigned int)* remainderLen);   //��������

	unsigned int mask = 0x80000000;
	unsigned int val = bi2.data[bi2.dataLength - 1];
	int shift = 0, resultPos = 0;

	while (mask != 0 && (val & mask) == 0)
	{
		shift++; mask >>= 1;
	}
	//���λ�Ӹߵ����ҳ�shift��0λ
	for (int i = 0; i < bi1.dataLength; i++)
		remainder[i] = bi1.data[i];   //��bi1���Ƶ�remainder֮��
	this->shiftLeft(remainder, remainderLen, shift);   //remainder����shiftλ
	bi2 = bi2 << shift;   //������shiftλ,����λ����
	//��������������������ͬ�ı���,���Խ������

	int j = remainderLen - bi2.dataLength;   //j��ʾ���������ȵĲ�ֵ,Ҳ��Ҫ����Ĵ���
	int pos = remainderLen - 1;   //posָʾ���������λ��λ��,����pos=bi1.dataLength

	//���µĲ��貢û�б����˼,��Ҫ����������
	unsigned __int64 firstDivisorByte = bi2.data[bi2.dataLength - 1];   //��һ������
	unsigned __int64 secondDivisorByte = bi2.data[bi2.dataLength - 2];  //�ڶ������� 

	int divisorLen = bi2.dataLength + 1;   //�����ĳ���
	unsigned int *dividendPart = new unsigned int[divisorLen];   //����Ϊ�����Ĳ���
	memset(dividendPart, 0, sizeof(unsigned int)* divisorLen);

	while (j > 0)
	{
		unsigned __int64 dividend = ((unsigned __int64)remainder[pos] << 32) + (unsigned __int64)remainder[pos - 1];   //ȡ�����ĸ���λ

		unsigned __int64 q_hat = dividend / firstDivisorByte;   //�õ�һ����
		unsigned __int64 r_hat = dividend % firstDivisorByte;   //�Լ�һ������

		bool done = false;   //��ʾû������
		while (!done)
		{
			done = true;

			if (q_hat == 0x100000000 || (q_hat * secondDivisorByte) > ((r_hat << 32) + remainder[pos - 2]))   //������Ҫ���������̵Ĵ�С 
				//(q_hat * secondDivisorByte) > ((r_hat << 32) + remainder[pos - 2]))�Ǻ����ϵ��̹���,��֮���Ϊ����
				//��q_hatҲ���ܳ���32bit
			{
				q_hat--;   //����Ļ�,����Сһ��,������һ��
				r_hat += firstDivisorByte;

				if (r_hat < 0x100000000)   //�������С��32bit,�ͼ���ѭ��
					done = false;
			}
		}

		for (int h = 0; h < divisorLen; h++)   //ȡ�������ĸ�λ����,��λ���ֳ������������һ��
			dividendPart[h] = remainder[pos - h];

		BigInteger kk(dividendPart, divisorLen);
		BigInteger ss = bi2 * BigInteger((__int64)q_hat);

		while (ss > kk)   //�����̵Ĵ�С
		{
			q_hat--;
			ss -= bi2;
		}
		BigInteger yy = kk - ss;   //�õ�����

		for (int h = 0; h < divisorLen; h++)  //��yy��λ��remainder��λƴ������,�õ�����
			remainder[pos - h] = yy.data[bi2.dataLength - h];   //ȡ������������

		result[resultPos++] = (unsigned int)q_hat;

		pos--;
		j--;
	}

	outQuotient.dataLength = resultPos;
	int y = 0;
	for (int x = outQuotient.dataLength - 1; x >= 0; x--, y++)   //���̷�ת����
		outQuotient.data[y] = result[x];
	for (; y < maxLength; y++)   //�̵�����λ��Ҫ��0
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

int BigInteger::shiftRight(unsigned int buffer[], int bufferLen, int shiftVal)   //���Ʋ���
{//�Լ���ͼ��ģ��һ����λ����,���ܺܿ�����������
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

BigInteger BigInteger::operator -(BigInteger bi2)   //����������
{
	BigInteger bi1(*this);
	BigInteger result;
	int lastPos = maxLength - 1;
	bool bi1Neg = false, bi2Neg = false;

	if ((this->data[lastPos] & 0x80000000) != 0)     // bi1 negative
		bi1Neg = true;
	if ((bi2.data[lastPos] & 0x80000000) != 0)     // bi1 negative
		bi2Neg = true;

	if (bi1Neg == false && bi2Neg == false)   //bi1,bi2��Ϊ����
	{
		if (bi1 < bi2)
		{
			result = -(bi2 - bi1);
			return result;
		}
		result.dataLength = (bi1.dataLength > bi2.dataLength) ? bi1.dataLength : bi2.dataLength;

		__int64 carryIn = 0;
		for (int i = 0; i < result.dataLength; i++)   //�ӵ�λ��ʼ��
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

	if (bi1Neg == true && bi2Neg == false)    //bi1��,bi2��
	{
		result = -(-bi1 + bi2);
		return result;
	}

	if (bi1Neg == false && bi2Neg == true)   //bi1��,bi2��
	{
		result = bi1 + (-bi2);
		return result;
	}

	if (bi1Neg == true && bi2Neg == true)   //bi1,bi2��Ϊ��
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

string BigInteger::DecToHex(unsigned int value, string format)   //10������ת����16������,����string��ʾ
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

string BigInteger::ToHexString()  //���ܣ���һ��������16���Ƶ�string��ʾ����
{
	string result = DecToHex(data[dataLength - 1], string("X"));

	for (int i = dataLength - 2; i >= 0; i--)
	{
		result += DecToHex(data[i], string("X8"));
	}

	return result;
}

ostream& operator<<(ostream& output, BigInteger &obj)//��16���������ֵ
{
	//if ((obj.data[obj.dataLength-1] & 0x80000000) != 0)     // bi1 negative
	for (int i = obj.dataLength - 1; i >= 0; i--)
		output << hex << obj.data[i];
	return output;
}


bool Miller_Robin(BigInteger &bi1)   //Miller_Robin�㷨
{
	BigInteger one((__int64)1), two((__int64)2), sum, a, b, temp;
	int k = 0, len = primeLength / 2;
	temp = sum = bi1 - one;
	while ((sum.data[0] & 0x00000001) == 0)   //ֻҪsum��Ϊ����,sum��һֱ������
	{
		sum.dataLength = sum.shiftRight(sum.data, maxLength, 1);   //����һλ
		k++;
	}
	//sum��ΪҪ�������,k����Ҫ���2�Ĵ���
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
	for (int i = 0; i < 303; i++)   //����һЩ�����������������ɸѡ
	{
		BigInteger prime((__int64)primesBelow2000[i]);
		if (obj % prime == zero)
			return false;
	}
	cout << "��һ�����Լ���ͨ���� �� �� ��" << endl;
	cout << "���ڽ���Miller_Robin���Լ��顭 �� �� ��" << endl;
	if (Miller_Robin(obj))   //����1��Miller_Robin����
		return true;  //ͨ���˾ͷ���result	  
	return false;//����result�Ǻ���,û��ͨ������
}

BigInteger GetPrime()
{
	BigInteger one((__int64)1), two((__int64)2), result;
	srand((unsigned)time(0));

	//�������һ��������
	for (int i = 0; i < primeLength; i++)
	{
		result.data[i] = (unsigned int)rand();
		if (result.data[i] != 0)
			result.dataLength = i + 1;
	}

	result.data[0] |= 0x00000001;   //��֤�������Ϊ����
	while (!IsPrime(result))   //���û��ͨ������,��+2,��������
	{
		result = result + two;

		cout << "����û��ͨ��,������һ�����ļ���,����֮�С� �� �� ��" << endl;
		cout << endl;
	}
	return result;

}




BigInteger extended_euclidean(BigInteger n, BigInteger m, BigInteger &x, BigInteger &y)   //��չ��ŷ������㷨  
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

BigInteger MultipInverse(BigInteger &bi1, BigInteger &n)   //��˷���Ԫ
{
	BigInteger x, y;
	extended_euclidean(bi1, n, x, y);
	if ((x.data[maxLength - 1] & 0x80000000) != 0)     // x negative
		x = x + n;
	// unsigned int i =  x.data[maxLength-1] & 0x80000000;
	// cout << i << endl;
	return x;
}