#include "MdRSACrypto.h"

MdRSACrypto::MdRSACrypto(void)
: ParamN(string("9925267379821871425510845582445759503537677132811205937337767819243983130290431496536905675097172626108809849063099490383615206095849973923022381920579193"), 10)
, ParamD(string("5644942460824217305807314808246519826955131459745825064823270702643487645914327490405459900282526319197371417079551956432087864023339947421172316448189099"), 10)
, ParamE(string("131363"), 10) // 默认 N E D 512位加密,事先计算出需要的数据
{
}

MdRSACrypto::~MdRSACrypto(void)
{
}

void MdRSACrypto::Encrypt(byte data[], int dataLen, byte *&secData, int &secDataLen)
{
	//加密函数
	//第一步，先将字符转换成为数据得到明文数据 decryptData
	BigInteger decryptData(data, dataLen);
	//第二步,加密 密文数据 encryptData = decryptData^ParamE mod ParamN
	BigInteger encryptData = decryptData.modPow(ParamE, ParamN);
	//将密文数据转换成为字符串
	BigIntegerToBytes(encryptData, secData, secDataLen);
}

void MdRSACrypto::Decrypt(byte data[], int dataLen, byte *&srcData, int &srcDataLen)
{
	//解密函数
	//第一步，得到密文数据 encryptData
	BigInteger encryptData(data, dataLen);
	//第二步，得到明文数据 decryptData = encryptData^ParamD mod ParamN
	BigInteger decryptData = encryptData.modPow(ParamD, ParamN);
	//将明文数据转换成为字符串
	BigIntegerToBytes(decryptData, srcData, srcDataLen);
}

void MdRSACrypto::BigIntegerToBytes(BigInteger bi, byte *&secData, int &secDataLen)
{
	string str = bi.ToHexString(); //先将大数的16进制表示转换成为字符串
	secDataLen = ceil(str.size() / 2.0);
	secData = new byte[secDataLen];
	memset(secData, 0, sizeof(byte)* secDataLen);
	int mod = str.size() % 2;
	if (mod == 0)
		secData[0] = htoi(str.substr(0, 2).c_str());
	else
		secData[0] = htoi(str.substr(0, 1).c_str());

	/*====================================================================
	substr 方法
	basic_string::substr
	basic_string substr(size_type _Off = 0,size_type _Count = npos) const;
	功能：从一个字符串复制一个从指定位置开始，并具有指定长度的子字符串。
	参数
	_Off
	所需的子字符串的起始位置。字符串中第一个字符的索引为 0,默认值为0.
	_Count
	复制的字符数目
	返回值
	一个子字符串，从其指定的位置开始
	备注
	如果 length 为 0 或负数，将返回一个空字符串。如果没有指定该参数，则子字符串将延续到字符串的结尾
	==================================================================*/

	for (int i = 1; i < secDataLen; i++)
	{
		//由于16进制大数的字符串两个字符才8位,因此每次处理两个字符
		secData[i] = htoi(str.substr((i * 2 - mod), 2).c_str());

	}
}

MdRSACrypto::byte MdRSACrypto::htoi(const char *str)
{//转换函数
	//由于str复制于表示大数的16进字符串,因此每一个字符只可能取
	//0 1 2 3 4 5 6 7 8 9 a b c d e f 或者 0 1 2 3 4 5 6 7 8 9 A B C D E F
	byte dec = 0, t;
	while (t = *str++)
	{
		dec <<= 4;
		if (t < 58)  //t表示0 - 9 时
			t -= 48; // 57 - 48 = 9
		if (t > 64 && t < 71)  //t表示 A - F 时
			t -= 55;  // 70 - 55 = 15 
		if (t > 96 && t < 103)  //t表示 a -  f 时
			t -= 87;   // 102 - 87 = 15
		//得到的t<= 15 ,最多占4位
		dec |= t; //按位求与,实际上实现了前一部分t(4位)和后一部分t(4位)的拼接
	}
	return dec;//最后组成了一个8位的字符，也就是成功地得到了明文字符或者密文字符
}