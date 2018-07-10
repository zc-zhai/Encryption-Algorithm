#include <iostream>
using namespace std;
#include <stdlib.h>
#include <time.h>

bool ispr(long long int a,long long int b){
	if (a <= 0 || b <= 0 || a == b){   // 互质整数不能小于或等于0
		return false;
	}
	else if (a == 1 || b == 1){  // 两个正整数中，只有其中一个数值为1，两个正整数为互质数
		return true;
	}
	else{
		//  求出两个正整数的最大公约数
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
		if (b > 1){  // 如果最大公约数大于1，表示两个正整数不互质
			return false;
		}
		else{         // 如果最大公约数等于1,表示两个正整数互质
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
		cout << "费马素性检验：";
		cin >> n;
		if (isfpr(n) == false){
			cout << "未通过检验 该数为合数" << endl;
		}
		else{
			cout << "通过检验 该数为素数或伪素数" << endl;
		}
	}
	return 0;
}