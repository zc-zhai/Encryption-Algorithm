#include <iostream>
#include "BigInteger.h"
#include "MdRSACrypto.h"
#include <string>

using namespace std;

int main()
{
	string message; // 要加密信息
	cout << "请输入要的加密信息" << endl;
	cin >> message;

	MdRSACrypto rsa;

	unsigned char *secData = NULL;
	int secDataLen = 0;

	rsa.Encrypt((unsigned char *)message.c_str(), message.size() + 1, secData, secDataLen);

	BigInteger secInt(secData, secDataLen);
	cout << "密文数据：" << secInt.ToHexString() << endl;

	unsigned char *decData = NULL;
	int decDataLen = 0;

	rsa.Decrypt(secData, secDataLen, decData, decDataLen);

	BigInteger decInt(decData, decDataLen);
	cout << "解密数据：" << decInt.ToHexString() << endl;

	string decMessage = (char *)decData;
	cout << "解密明文：" << decMessage << endl;

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