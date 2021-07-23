// MathConcepInProg1.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <stdbool.h>

bool implies(bool x, bool y)
{
	if (x && !y)
		return false;
	return true;
}

bool con(bool x)
{
	if (x && !x)
		return true;
	return false;
}

bool con(bool x, bool y)
{
	if (implies(x, y))
		return true;
	return false;
}

bool con(bool x, bool y, bool z)
{
	if ((x && !y) || !z)
		return true;
	return false;
}

void print_result(bool x, bool (*con)(bool))
{
	if(con(x) == true)
		printf("%d1\n", x);
	else
		printf("%d0\n", x);
}

void print_result(bool x, bool y, bool (*con)(bool, bool))
{
	if (con(x, y) == true)
		printf("%d%d1\n", x, y);
	else
		printf("%d%d0\n", x, y);
}

void print_result(bool x, bool y, bool z, bool (*con)(bool, bool, bool))
{
	if (con(x, y, z) == true)
		printf("%d%d%d1\n", x, y, z);
	else
		printf("%d%d%d0\n", x, y, z);
}

int main()
{
	for (int i = 1; i >= 0; i--)
		for (int j = 1; j >= 0; j--)
		{
			print_result(i, j, con);
			for (int k = 1; k >= 0; k--)
			{
				/*if ((i && !j) || !k)
					printf("%d%d%d1\n", i, j, k);
				else
					printf("%d%d%d0\n", i, j, k);*/
				;
			}
		}
}
