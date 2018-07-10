#include <iostream>
using namespace std;

int inv(int e, int p)
{
	int x1, x2, x3, y1, y2, y3;
	int z1, z2, z3;
	x1 = 1; x2 = 0; x3 = p;
	y1 = 0; y2 = 1; y3 = e;
	int q;
	for (;;)
	{
		q = x3 / y3;
		z1 = x1; z2 = x2; z3 = x3;
		x1 = y1; x2 = y2; x3 = y3;
		y1 = z1 - y1*q;
		y2 = z2 - y2*q;
		y3 = z3 - y3*q;
		if (y3 == 1)return y2>0 ? y2 : y2 + p;
	}
}

int main()
{
	int e, p;
	cin >> e >> p;
	cout << inv(e, p)<< endl;
	return 0;
}