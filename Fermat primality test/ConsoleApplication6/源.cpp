#include <iostream>
using namespace std;
#include <stdlib.h>
#include <time.h>

bool ispr(long long int a,long long int b){
	if (a <= 0 || b <= 0 || a == b){   // ������������С�ڻ����0
		return false;
	}
	else if (a == 1 || b == 1){  // �����������У�ֻ������һ����ֵΪ1������������Ϊ������
		return true;
	}
	else{
		//  ������������������Լ��
		while (1){
			int t = a%b;
			if (t == 0){
				break; 
			}
			else{
				a = b;
				b = t;
			}
		}
		if (b > 1){  // ������Լ������1����ʾ����������������
			return false;
		}
		else{         // ������Լ������1,��ʾ��������������
			return true;
		}
	}
}

long long int square(long long int x) { return x * x; }
long long int fast_mod(long long int x, long long int n, long long int m)
{
	if (n == 0)
		return 1;
	else if (n % 2 == 0)
		return square(fast_mod(x, n / 2, m)) % m;
	else
		return x * fast_mod(x, n - 1, m) % m;
}

bool isfpr(long long int n)
{
	long long int b;
	int i = 0;
	srand(time(NULL));
	while (i<100)
	{
		b = rand() % n;
		if (b<2 || b>n - 2){
			break;
		}
		else
		{
			if (ispr(n, b) == false){
				break;
			}
			else
			{
				i++;
				if (fast_mod(b,n-1,n)!=1){
					return false;
				}
			}
		}
	}
	return true;
}

int main()
{
	long long int n;
	while (true){
		cout << "�������Լ��飺";
		cin >> n;
		if (isfpr(n) == false){
			cout << "δͨ������ ����Ϊ����" << endl;
		}
		else{
			cout << "ͨ������ ����Ϊ������α����" << endl;
		}
	}
	return 0;
}