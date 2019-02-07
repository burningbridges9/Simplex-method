/*MADE BY RUSTAM TALIPOV*/
#pragma once
#include <iostream>
#include <vector>
#include <algorithm>
#include <string>
using namespace std;
class simplex
{
	int n;//���������� �����������
	int m;// ���������� ����������
	vector<double> c; // ������������ C ��� ������� �������
	vector<vector<double>> L; //������� �����������
	vector<double> b;//������ ������ �����
	vector<string> signs; // ����� � ������� �����������
	string extr;//max ? min ������� �������
	bool isChangedExtremum;// ������������ ���?
	vector<string> variable_signs; //����� (>=, <) ����������
	vector<double> xi; //���� ����������

	vector<int> edited_positions;
	vector<double> CW; // ������������ � � �����
	vector<double> delta;//������ (��� w)
	vector<double> deltaW;// ������ � ������ �������������� ������
	double F; //���

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