#include <iostream>
#include "BigInteger.h"
#include "MdRSACrypto.h"
#include <string>

using namespace std;

int main()
{
	string message; // Ҫ������Ϣ
	cout << "������Ҫ�ļ�����Ϣ" << endl;
	cin >> message;

	MdRSACrypto rsa;

	unsigned char *secData = NULL;
	int secDataLen = 0;

	rsa.Encrypt((unsigned char *)message.c_str(), message.size() + 1, secData, secDataLen);

	BigInteger secInt(secData, secDataLen);
	cout << "�������ݣ�" << secInt.ToHexString() << endl;

	unsigned char *decData = NULL;
	int decDataLen = 0;

	rsa.Decrypt(secData, secDataLen, decData, decDataLen);

	BigInteger decInt(decData, decDataLen);
	cout << "�������ݣ�" << decInt.ToHexString() << endl;

	string decMessage = (char *)decData;
	cout << "�������ģ�" << decMessage << endl;

	if (secData != NULL)
	{
		delete[]secData;
	}

	if (decData != NULL)
	{
		delete[]decData;
	}
	system("pause");
	return 0;
}