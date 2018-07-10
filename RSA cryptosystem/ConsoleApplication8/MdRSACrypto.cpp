#include "MdRSACrypto.h"

MdRSACrypto::MdRSACrypto(void)
: ParamN(string("9925267379821871425510845582445759503537677132811205937337767819243983130290431496536905675097172626108809849063099490383615206095849973923022381920579193"), 10)
, ParamD(string("5644942460824217305807314808246519826955131459745825064823270702643487645914327490405459900282526319197371417079551956432087864023339947421172316448189099"), 10)
, ParamE(string("131363"), 10) // Ĭ�� N E D 512λ����,���ȼ������Ҫ������
{
}

MdRSACrypto::~MdRSACrypto(void)
{
}

void MdRSACrypto::Encrypt(byte data[], int dataLen, byte *&secData, int &secDataLen)
{
	//���ܺ���
	//��һ�����Ƚ��ַ�ת����Ϊ���ݵõ��������� decryptData
	BigInteger decryptData(data, dataLen);
	//�ڶ���,���� �������� encryptData = decryptData^ParamE mod ParamN
	BigInteger encryptData = decryptData.modPow(ParamE, ParamN);
	//����������ת����Ϊ�ַ���
	BigIntegerToBytes(encryptData, secData, secDataLen);
}

void MdRSACrypto::Decrypt(byte data[], int dataLen, byte *&srcData, int &srcDataLen)
{
	//���ܺ���
	//��һ�����õ��������� encryptData
	BigInteger encryptData(data, dataLen);
	//�ڶ������õ��������� decryptData = encryptData^ParamD mod ParamN
	BigInteger decryptData = encryptData.modPow(ParamD, ParamN);
	//����������ת����Ϊ�ַ���
	BigIntegerToBytes(decryptData, srcData, srcDataLen);
}

void MdRSACrypto::BigIntegerToBytes(BigInteger bi, byte *&secData, int &secDataLen)
{
	string str = bi.ToHexString(); //�Ƚ�������16���Ʊ�ʾת����Ϊ�ַ���
	secDataLen = ceil(str.size() / 2.0);
	secData = new byte[secDataLen];
	memset(secData, 0, sizeof(byte)* secDataLen);
	int mod = str.size() % 2;
	if (mod == 0)
		secData[0] = htoi(str.substr(0, 2).c_str());
	else
		secData[0] = htoi(str.substr(0, 1).c_str());

	/*====================================================================
	substr ����
	basic_string::substr
	basic_string substr(size_type _Off = 0,size_type _Count = npos) const;
	���ܣ���һ���ַ�������һ����ָ��λ�ÿ�ʼ��������ָ�����ȵ����ַ�����
	����
	_Off
	��������ַ�������ʼλ�á��ַ����е�һ���ַ�������Ϊ 0,Ĭ��ֵΪ0.
	_Count
	���Ƶ��ַ���Ŀ
	����ֵ
	һ�����ַ���������ָ����λ�ÿ�ʼ
	��ע
	��� length Ϊ 0 ������������һ�����ַ��������û��ָ���ò����������ַ������������ַ����Ľ�β
	==================================================================*/

	for (int i = 1; i < secDataLen; i++)
	{
		//����16���ƴ������ַ��������ַ���8λ,���ÿ�δ��������ַ�
		secData[i] = htoi(str.substr((i * 2 - mod), 2).c_str());

	}
}

MdRSACrypto::byte MdRSACrypto::htoi(const char *str)
{//ת������
	//����str�����ڱ�ʾ������16���ַ���,���ÿһ���ַ�ֻ����ȡ
	//0 1 2 3 4 5 6 7 8 9 a b c d e f ���� 0 1 2 3 4 5 6 7 8 9 A B C D E F
	byte dec = 0, t;
	while (t = *str++)
	{
		dec <<= 4;
		if (t < 58)  //t��ʾ0 - 9 ʱ
			t -= 48; // 57 - 48 = 9
		if (t > 64 && t < 71)  //t��ʾ A - F ʱ
			t -= 55;  // 70 - 55 = 15 
		if (t > 96 && t < 103)  //t��ʾ a -  f ʱ
			t -= 87;   // 102 - 87 = 15
		//�õ���t<= 15 ,���ռ4λ
		dec |= t; //��λ����,ʵ����ʵ����ǰһ����t(4λ)�ͺ�һ����t(4λ)��ƴ��
	}
	return dec;//��������һ��8λ���ַ���Ҳ���ǳɹ��صõ��������ַ����������ַ�
}