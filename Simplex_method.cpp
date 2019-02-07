#include "stdafx.h"
#include <iostream>
#include <vector>
#include <algorithm>
#include <string>
#include "simplex.h"
using namespace std;

int main()
{
	setlocale(LC_ALL, "Russian");
	simplex A;
	cin >> A;
	cout << endl << A;
	if (A.checkCanon() == false)
	{
		A.makeCanon();
	}
	A.makeBasis();
	A.solveBySimplexMethod();
	system("pause");
    return 0;
}

