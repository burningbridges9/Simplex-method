/*MADE BY RUSTAM TALIPOV*/
#pragma once
#include <iostream>
#include <vector>
#include <algorithm>
#include <string>
using namespace std;
class simplex
{
	int n;//количество ограничений
	int m;// количество переменных
	vector<double> c; // коэффициенты C при целевой функции
	vector<vector<double>> L; //матрица ограничений
	vector<double> b;//вектор правой части
	vector<string> signs; // знаки в матрице ограничений
	string extr;//max ? min целевой функции
	bool isChangedExtremum;// канонический вид?
	vector<string> variable_signs; //знаки (>=, <) переменных
	vector<double> xi; //сами переменные

	vector<int> edited_positions;
	vector<double> CW; // коэффициенты С с омега
	vector<double> delta;//оценки (без w)
	vector<double> deltaW;// оценка в случае искусственного базиса
	double F; //ЗЛФ

public:
	simplex();
	simplex(int n_, int m_);
	simplex(const simplex & other);
	bool checkCanon();
	bool checkVariableSigns();
	bool checkLSigns();
	bool checkB();
	bool checkMin();
	bool checkUnitaryMatrix();
	void makeCanon();
	void makeBasis();
	void solveBySimplexMethod();
	const simplex& operator= (const simplex & other);
	void setLSize(int N, int M);
	void setCSize(int N);
	void setBSize(int M);
	void setSignsSize(int M);
	void setVariable_signsSize(int M);
	void setCWSize(int N);
	friend ostream &operator<< (ostream &output, simplex &obj);
	friend istream &operator >> (istream &input, simplex &obj);
	~simplex();
};

/*MADE BY RUSTAM TALIPOV*/