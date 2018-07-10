#include <iostream>
using namespace std;

long long int square(long long int x) { return x * x; }
long long int fast_mod(long long int x,long long int n,long long int m)
{
	if (n == 0)
		return 1;
	else if (n % 2 == 0)
		return square(fast_mod(x, n / 2, m)) % m;
	else
		return x * fast_mod(x, n - 1, m) % m;
}

int main()
{
	long long int x, n, m;
	cin >> x >> n >> m;
	cout << fast_mod(x, n, m) << endl;
	return 0;
}
