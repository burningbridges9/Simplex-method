/*MADE BY RUSTAM TALIPOV*/
#include "stdafx.h"
#include "simplex.h"


simplex::simplex()
{
	isChangedExtremum = false;
}

simplex::simplex(int n_, int m_)
{
	n = n_;
	m = m_;
	c = vector<double>(n);
	L = vector<vector<double>>(m, vector<double>(n));
	b = vector<double>(m);
}

simplex::simplex(const simplex & other)
{
	if (this != &other)
	{
		n = other.n;
		m = other.m;
		c = other.c;
		L = other.L;
		b = other.b;
		signs = other.signs;
		extr = other.extr;

		delta = other.delta;
		deltaW = other.deltaW;
		F = other.F;
	}
}

bool simplex::checkVariableSigns()
{
	for (vector<string>::iterator it = variable_signs.begin(); it != variable_signs.end(); ++it)
	{
		if ((*it) != ">=")
		{
			return false;
		}
	}
	return true;
}

bool simplex::checkLSigns()
{
	for (vector<string>::iterator it =signs.begin(); it != signs.end(); ++it)
	{
		if ((*it) != "=")
		{
			return false;
		}
	}
	return true;
}

bool simplex::checkCanon()
{
	if (checkVariableSigns() == false)
	{
		return false;
	}
	if (checkLSigns() == false)
	{
		return false;
	}
	if (checkB() == false)
	{
		return false;
	}
	if (checkMin() == false)
	{
		return false;
	}
	return true;
}

bool simplex::checkB()
{
	for (vector<double>::iterator it = b.begin(); it != b.end(); it++)
	{
		if (*it < 0)
		{
			return false;
		}
	}
	return true;
}

bool simplex::checkMin()
{
	if (extr == "max")
		return false;
	else
		return true;
}

bool simplex::checkUnitaryMatrix()
{
	cout << "Ищем единичную матрицу размера :" << m << "x" << m << endl;
	for (vector<vector<double>>::iterator I = L.begin(); I != L.end(); ++I)
	{
		for (vector<double>::iterator J = (*I).begin(); J != (*I).end(); J++)
		{
			if (*J == 1)
			{
				// хз че делац
			}
		}
	}
	return false;
}

void simplex::makeCanon()
{
	if (checkVariableSigns() == false)//проверка на неотрицательность переменных
	{
		for (vector<string>::iterator it=variable_signs.begin(); it != variable_signs.end(); ++it)
		{
			if ((*it) != ">=")
			{
				int i = distance(variable_signs.begin(), it);//вычисл i
				edited_positions.push_back(i);//запоминаем изменившуюся переменную
				cout << "Найдена переменная с неопределенным знаком в позиции " << i + 1 << endl;
			}
		}
		for (vector<int>::iterator it = edited_positions.begin(); it != edited_positions.end(); ++it)
		{
			vector<string>::iterator ittmp1 = variable_signs.begin();
			variable_signs[*it] = ">=";
			
			advance(ittmp1, *it);// итератор по знакам переменных
			variable_signs.insert(ittmp1 + 1, ">=");//добавляем знак в вектор
			CW.resize(CW.size() + 1);

			//вставка в вектор С значения переменной с двумя штрихами
			vector<double>::iterator itC = c.begin();//устанавливаем на начало
			advance(itC, *it);//увеличиваем на i
			double c_i = *itC;//получили значение в позиции i
			advance(itC, 1);//увеличили итератор на 1
			c.insert(itC, -c_i);//вставили переменнцую с двумя штрихами
			cout << "Замена переменных: " << c_i << "[x_" << *it + 1 << "]" << " = " << c_i << "[x_" << *it + 1 << "]" << -c_i << "[x_" << *it + 2 << "]" << endl;
			//проверка последний элемент?
			for (vector<int>::iterator itValChange = it+1; itValChange != edited_positions.end(); ++itValChange)
			{
				(*itValChange) += 1;
			}
			//изменение матрицы ограничений
			for (vector<vector<double>>::iterator itI = L.begin(); itI != L.end(); ++itI)
			{
				vector<double>::iterator itJ = (*itI).begin();
				advance(itJ, *it);//находим элемент, который нужно раздвоить
				double L_ij = *itJ;//его значение
				advance(itJ, 1);//увеличиваем итератор на 1
				L[distance(L.begin(), itI)].insert(itJ, -L_ij);//вставляем в iую строку в позицию itJ+1 переменную с двумя штрихами
			}
			cout << (*this);
		}
		for (vector<int>::iterator it = edited_positions.begin(); it != edited_positions.end(); ++it)
		{
			cout << "edited positions: " << (*it) << endl;
		}
	}
	if (checkLSigns() == false)//проверка знаков(=, >=, <=) в ограничениях
	{
		for (vector<string>::iterator it = signs.begin(); it != signs.end(); ++it)
		{
			int idx = distance(signs.begin(), it); // индекс рассматриваемого знака
			if (*it == "=")
			{
				continue;
			}
			if (*it == ">=")
			{
				//должны добавить дополнительную переменную -1
				
				//нужно вставить в матрицу ограничений в iую строку в последнюю позицию
				
				vector<vector<double>>::iterator itI = L.begin();
				advance(itI, idx);//находим нужную строку
				vector<double>::iterator itJ = (*itI).end();
				cout << "Введем дополнительную переменную: [x_" << idx + 2 << "] = " << -1 << endl;
				L[distance(L.begin(), itI)].push_back(-1.0);//вставили в конец нужной строки -1, теперь нужно вставить в остальные строки 0
				//vector<double>::iterator itJedited = (*itI).end();//запомним конец вектора, куда вставили -1, чтобы потом туда не вставлять 0
				
				c.push_back(0);//нужно вставить в массив С значения 0 для доп переменных
				CW.resize(CW.size() + 1);

				variable_signs.push_back(">=");//нужно вставить в массив знаков переменных >=
				*it = "=";//изменим знак в векторе знаков ограничений
				cout << (*this);
				for (vector<vector<double>>::iterator itI0 = L.begin(); itI0 != L.end(); itI0++)
				{
					//vector<double>::iterator itJ0 = (*itI0).end();
					if (itI != itI0) //если не указывает на один и тот же элемент, то вставляем 0
					{
						L[distance(L.begin(), itI0)].push_back(0.0);
					}
				}
				cout << (*this);
			}
			if (*it == "<=")
			{
				//должны добавить дополнительную переменную -1
				//нужно вставить в матрицу ограничений в iую строку в последнюю позицию
				vector<vector<double>>::iterator itI = L.begin();
				advance(itI, idx);//находим нужную строку
				vector<double>::iterator itJ = (*itI).end();
				cout << "Введем дополнительную переменную: [x_" << idx + 2 << "] = " << -1 << endl;
				L[distance(L.begin(), itI)].push_back(1.0);//вставили в конец нужной строки 1, теперь нужно вставить в остальные строки 0
				c.push_back(0);//нужно вставить в массив С значения 0 для доп переменных
				CW.resize(CW.size() + 1);
				
				variable_signs.push_back(">=");//нужно вставить в массив знаков переменных >=
				*it = "=";//изменим знак в векторе знаков ограничений
				cout << (*this);
				//vector<double>::iterator itJedited = (*itI).end();//запомним конец вектора, куда вставили -1, чтобы потом туда не вставлять 0
				for (vector<vector<double>>::iterator itI0 = L.begin(); itI0 != L.end(); itI0++)
				{
					vector<double>::iterator itJ0 = (*itI0).end();
					if (itI != itI0) //если не указывает на один и тот же элемент, то вставляем 0
					{
						L[distance(L.begin(), itI0)].push_back(0.0);
					}
				}
				cout << (*this);
			}
		}
	}
	if (checkB() == false)
	{
		for (vector<double>::iterator it = b.begin(); it != b.end(); it++)
		{
			int index=distance(b.begin(), it);
			if (*it < 0)
			{
				*it *= -1;
				vector<vector<double>>::iterator itI = L.begin();
				advance(itI, index);//находим нужную строку, где нужно умножить на -1
				for (vector<double>::iterator itJ = (*itI).begin(); itJ != (*itI).end(); itJ++)
				{
					*itJ *= -1;
				}
				cout << (*this);
			}
			else
			{
				continue;
			}
		}
	}
	if (checkMin() == false)
	{
		extr = "min";
		for (vector<double>::iterator it = c.begin(); it != c.end(); ++it)
		{
			*it *= -1;
		}
		isChangedExtremum = true;
		cout << (*this);
	}
}

void simplex::makeBasis()
{
	for (vector<vector<double>>::iterator I = L.begin(); I != L.end(); ++I)
	{
		vector<double>::iterator itJ = (*I).end();
		cout << "Введем искусственную переменную: [x_" << distance(L.begin(),L.end()) +1<< "] = " << 1 << endl;
		L[distance(L.begin(), I)].push_back(1.0);//вставили в конец нужной строки -1, теперь нужно вставить в остальные строки 0
		//запомним конец вектора, куда вставили -1, чтобы потом туда не вставлять 0
		//НУЖНО ЗАПОМНИТЬ СТРОКУ КУДА ЗАСУНУЛИ -1!!!!!!!!!!
		c.push_back(0);//нужно вставить в массив С значения 0 для доп переменных
		CW.push_back(1);
		/*
		cout << CW.size();
		for (int i = 0; i != CW.size(); i++)
		{
			cout << "CW:" << CW[i]<<endl;
		}*/
		variable_signs.push_back(">=");//нужно вставить в массив знаков переменных >=
		cout << (*this);
		for (vector<vector<double>>::iterator itI0 = L.begin(); itI0 != L.end(); itI0++)
		{
			//vector<double>::iterator itJ0 = (*itI0).end();
			if (I != itI0) //если не указывает на один и тот же элемент, то вставляем 0
			{
				L[distance(L.begin(), itI0)].push_back(0.0);
			}
		}
		cout << (*this)<<endl;
	}
	cout << "Есть единичный базис" << endl;
}

void simplex::solveBySimplexMethod()
{
	cout << "Старт решения симплекс-методом:" << endl;
	//объявляем матрицу Mx(N+4): M - количество ограничений, N - количество переменных, 
	//1 - столбец под номеры векторов, 1 - столбец под С, 1 - столбец под Cw, 1 - столбец под B^-1*P0
	vector<vector<double>> Table;
	Table = vector<vector<double>>(m, vector<double>(c.size() + 4));
	
	double idx; // помним, что индексация с нуля -- прибавить один
	//начальное заполнение таблицы
	//здесь идет вставка коэфов CW
	int index_for_rows = 0;
	for (vector<double>::iterator i = CW.begin(); i != CW.end(); ++i)
	{
		if (*i != 0)
		{
			idx = distance(CW.begin(), i);
			Table[index_for_rows][0] = idx+1;
			Table[index_for_rows][2] = (*i);
			index_for_rows++;
		}
	}
	//здесь идет вставка коэфов C
	index_for_rows = 0;
	for (vector<double>::iterator i = c.begin(); i != c.end(); ++i)
	{	// ищем последние элементы в векторе С для искусственного базиса
		if (distance(c.begin(), i)== Table[index_for_rows][0]) 
		{
			Table[index_for_rows][1] = (*i);
			index_for_rows++;
		}
	}
	//здесь идет вставка B^-1*P0 вектора b
	index_for_rows = 0;
	for (vector<double>::iterator i = b.begin(); i != b.end(); ++i)
	{	
		Table[index_for_rows][3] = (*i);
		index_for_rows++;
	}
	//здесь идет вставка B^-1*P(i) матрицы L
	vector<vector<double>>::iterator IT = Table.begin();
	for (vector<vector<double>>::iterator IL = L.begin(); IL != L.end(); ++IL)
	{
		vector<double>::iterator JT = (*IT).begin() + 4;
		for (vector<double>::iterator JL = (*IL).begin(); JL != (*IL).end(); ++JL)
		{
			(*JT) = (*JL);
			++JT;
		}
		++IT;
	}

	
	
	//подсчет оценок для опорного базиса
	delta = vector<double>(c.size());
	deltaW = vector<double>(c.size());

	vector<double>::iterator iC = c.begin();
	vector<double>::iterator iCw = CW.begin();
	vector<double>::iterator id = delta.begin();
	vector<double>::iterator idw = deltaW.begin();

	IT = Table.begin();
	int col = 0;
	for (vector<double>::iterator JT = (*IT).begin() + 4; JT != (*IT).end(); ++JT)
	{
		
		for (vector<vector<double>>::iterator ITinner = Table.begin(); ITinner != Table.end(); ++ITinner)
		{
			vector<double>::iterator JTC = (*IT).begin() + 1;
			vector<double>::iterator JTCw = (*IT).begin() + 2;
			vector<double>::iterator JTinner = (*ITinner).begin() + 4+col;
			(*id) += (*JTC)* (*JTinner);
			(*idw) += (*JTCw)* (*JTinner);

			
		}
		(*id) -= (*iC);
		(*idw) -= (*iCw);
		++iC;
		++iCw;
		++id;
		++idw;
		++col;
	}

	cout << "Коэффициенты С:" << endl;
	for (vector<double>::iterator i = c.begin(); i != c.end(); ++i)
		cout << "|  " << (*i) << "  |";
	cout << endl << "Коэффициенты Сw:" << endl;
	for (vector<double>::iterator i = CW.begin(); i != CW.end(); ++i)
		cout << "|  " << (*i) << "  |";
	cout << endl << "Таблица:" << endl << "B         |  C-базисное  |  Cw-базисное  |  B^-1*P_0  |";
	for (vector<double>::iterator i = c.begin(); i != c.end(); ++i)
		cout << "    B^-1*P_" << distance(c.begin(), i) + 1 << "  |";
	cout << endl;
	for (vector<vector<double>>::iterator IT = Table.begin(); IT != Table.end(); ++IT)
	{
		for (vector<double>::iterator JT = (*IT).begin(); JT != (*IT).end(); ++JT)
		{
			cout << *JT << "              ";
		}
		cout << endl;
	}
	cout << "Оценки:" << endl << "                                                              ";
	for (vector<double>::iterator i = delta.begin(); i != delta.end(); ++i)
	{
		cout << (*i) << "            ";
	}
	cout << endl;
	cout << "Оценки w:" << endl << "                                                            ";
	for (vector<double>::iterator i = deltaW.begin(); i != deltaW.end(); ++i)
	{
		cout << (*i) << "            ";
	}
	cout << endl;
	F = 0; //подсчет ЗЛФ
	double Fw = 0;
	for (int i = 0; i != Table.size(); i++)
	{
		Fw += Table[i][2] * Table[i][3];
		F += Table[i][1] * Table[i][3];
	}
	cout << " ЗЛФ = " << F << endl;
	cout << " ЗЛФ при w= " << Fw << endl;

	//проверка на оптимальность плана
	bool isEndFlag = 1;
	bool isEndFlagW = 1;
	for (vector<double>::iterator i = delta.begin(); i != delta.end(); ++i)
	{
		if ((*i) <= 0)
			continue;
		else
			isEndFlag = 0;
	}                                                         
	for (vector<double>::iterator i = deltaW.begin(); i != deltaW.end(); ++i)
	{
		if ((*i) <= 0)
			continue;
		else
			isEndFlagW = 0;
	}

	while ( isEndFlag != true || isEndFlagW != true )
	{
		if (isEndFlagW == false)
		{
			//поиск минимального
			double min=100000000000000000; //
			bool flagMinChanged = 0;
			int minIndex;// на выходе получим индекс минимального элемента
			int indexForTableCol; // в каком столбце положительная оценка
			int indexPositive; // номер нового вектора в базисе
			for (vector<double>::iterator i = deltaW.begin(); i != deltaW.end(); ++i) //сперва проверим для W
			{
				if ((*i) > 0)
				{
					indexPositive = distance(deltaW.begin(), i); //  находим номер вектора, входящий в новый базис

					indexForTableCol = 4 + indexPositive;
					
					for (int j = 0; j != Table.size(); j++)
					{
						if (Table[j][indexForTableCol] <= 0)
						{
							continue;
						}
						else
						{
							double theta = (Table[j][3] * 1.0) / Table[j][indexForTableCol];
							if (j == 0)
							{
								flagMinChanged = true;
								min = theta;
								minIndex = j;
							}
							else
							{
								if (theta <= min)
								{
									flagMinChanged = true;
									min = theta;
									minIndex = j;
								}
							}
						}
					}
					if (flagMinChanged == false)
					{
						cerr << "min не достигается в области ограничений D"<<endl;
					}
					break; // нашли первый попапвшийся положительный элемент
				}
				else 
					continue;
			}
			//Выведем вектор из базиса
			Table[minIndex][0] = indexPositive + 1; // перепишем индекс
			double Ctemp = c[indexPositive];
			double CWtemp = CW[indexPositive];
			Table[minIndex][1] = Ctemp; // перепишем с
			Table[minIndex][2] = CWtemp; // перепишем cw
			//Гауссовские преобразования:
			IT = Table.begin()+minIndex;
			// преобразования в самой строке
			double tempDenominator = Table[minIndex][indexForTableCol]; //чтобы не испортить делитель
			for (vector<double>::iterator JT = (*IT).begin()+3; JT!= (*IT).end(); ++JT)
			{
				(*JT) = (1.0*(*JT)) / tempDenominator; // получаем единицу на нужном месте
			}

			//проход по столбцу и запись чисел, на которые нужно будет умножить строку, чтобы вычитать из других
			vector < double	> multipliers(Table.size());
			double t;
			for (int i = 0; i != Table.size(); ++i)
			{
				multipliers[i] = Table[i][indexForTableCol];
				t = multipliers[i];
			}
			//int i = 0;
			int k = 0;
			double t1;
			for (vector<vector<double>>::iterator IT = Table.begin() ; IT != Table.end(); ++IT)
			{	
				if (distance(Table.begin(), IT) == minIndex) // если находимся на той строке, которую вычитаем от всех
				{
					double t = multipliers[k];
					++k;
					continue;
				}
				else
				{
					// какие то костыли хз зачем так делал
					//if (distance(Table.begin(), IT)==0) 
					//	++i;
					//// какие то костыли хз зачем так делал
					//if (distance(Table.begin(), IT) == 0)
					//	++i;
					int j = 0 + 3;
					for (vector<double>::iterator JT = (*IT).begin() + 3; JT != (*IT).end(); ++JT)
					{
						t = multipliers[k];
						t1 = Table[minIndex][j];
						(*JT) = (*JT) - Table[minIndex][j] * multipliers[k];
						
						++j;
					}
					//++i;
					++k;
				}
			}

			//подсчет оценок для  базиса
			delta.clear();
			deltaW.clear();
			delta = vector<double>(c.size());
			deltaW = vector<double>(c.size());

			vector<double>::iterator iC = c.begin();
			vector<double>::iterator iCw = CW.begin();
			vector<double>::iterator id = delta.begin();
			vector<double>::iterator idw = deltaW.begin();

			cout << "Коэффициенты С:" << endl;
			for (vector<double>::iterator i = c.begin(); i != c.end(); ++i)
				cout << "|  " << (*i) << "  |";
			cout << endl << "Коэффициенты Сw:" << endl;
			for (vector<double>::iterator i = CW.begin(); i != CW.end(); ++i)
				cout << "|  " << (*i) << "  |";
			cout << endl << "Таблица:" << endl << "B         |  C-базисное  |  Cw-базисное  |  B^-1*P_0  |";
			for (vector<double>::iterator i = c.begin(); i != c.end(); ++i)
				cout << "    B^-1*P_" << distance(c.begin(), i) + 1 << "  |";
			cout << endl;
			for (vector<vector<double>>::iterator IT = Table.begin(); IT != Table.end(); ++IT)
			{
				for (vector<double>::iterator JT = (*IT).begin(); JT != (*IT).end(); ++JT)
				{
					cout << *JT << "              ";
				}
				cout << endl;
			}

			IT = Table.begin();
			int col = 0;
			for (vector<double>::iterator JT = (*IT).begin() + 4; JT != (*IT).end(); ++JT)
			{

				for (vector<vector<double>>::iterator ITinner = Table.begin(); ITinner != Table.end(); ++ITinner)
				{
					vector<double>::iterator JTC = (*ITinner).begin() + 1;
					vector<double>::iterator JTCw = (*ITinner).begin() + 2;
					vector<double>::iterator JTinner = (*ITinner).begin() + 4 + col;
					(*id) += (*JTC)* (*JTinner);
					(*idw) += (*JTCw)* (*JTinner);


				}
				(*id) -= (*iC);
				(*idw) -= (*iCw);
				++iC;
				++iCw;
				++id;
				++idw;
				++col;
			}

			cout << "Коэффициенты С:" << endl;
			for (vector<double>::iterator i = c.begin(); i != c.end(); ++i)
				cout << "|  " << (*i) << "  |";
			cout << endl << "Коэффициенты Сw:" << endl;
			for (vector<double>::iterator i = CW.begin(); i != CW.end(); ++i)
				cout << "|  " << (*i) << "  |";
			cout << endl << "Таблица:" << endl << "B         |  C-базисное  |  Cw-базисное  |  B^-1*P_0  |";
			for (vector<double>::iterator i = c.begin(); i != c.end(); ++i)
				cout << "    B^-1*P_" << distance(c.begin(), i) + 1 << "  |";
			cout << endl;
			for (vector<vector<double>>::iterator IT = Table.begin(); IT != Table.end(); ++IT)
			{
				for (vector<double>::iterator JT = (*IT).begin(); JT != (*IT).end(); ++JT)
				{
					cout << *JT << "              ";
				}
				cout << endl;
			}
			cout << "Оценки:" << endl << "                                                              ";
			for (vector<double>::iterator i = delta.begin(); i != delta.end(); ++i)
			{
				cout << (*i) << "            ";
			}
			cout << endl;
			cout << "Оценки w:" << endl << "                                                            ";
			for (vector<double>::iterator i = deltaW.begin(); i != deltaW.end(); ++i)
			{
				cout << (*i) << "            ";
			}
			cout << endl;
			F = 0; //подсчет ЗЛФ
			Fw = 0;
			for (int i = 0; i != Table.size(); i++)
			{
				Fw += Table[i][2] * Table[i][3];
				F += Table[i][1] * Table[i][3];
			}
			cout << " ЗЛФ = " << F << endl;
			cout << " ЗЛФ при w= " << Fw << endl;

			//проверка на оптимальность плана
			isEndFlag = 1;
			isEndFlagW = 1;

			//проверка, присутствуют ли искусственные векторы в базисе

			vector<int> fake_indexes;
			int C = 0;
			for (vector<double>::iterator i = CW.begin(); i != CW.end(); ++i)
			{
				if ((*i) != 0)
				{
					fake_indexes.push_back(distance(CW.begin(), i)+1);

					C++;
				}
			}

			for (vector<int>::iterator i = fake_indexes.begin(); i != fake_indexes.end(); ++i)
			{
				cout << "fi = " << (*i) << "  ,";
			}
			cout << endl;
			bool isNot = 0;
			
			for (vector<vector<double>>::iterator it = Table.begin(); it != Table.end(); ++it)
			{
				vector<double>::iterator index = (*it).begin();
				for (vector<int>::iterator f_index = fake_indexes.begin(); f_index != fake_indexes.end(); ++f_index)
				{
					if ((*index) == (*f_index))
					{
						isNot = 0;
						break;
					}
					if (( (*index) != (*f_index) ) && ( distance(fake_indexes.begin(), f_index) ))
					{
						isNot = 1;
					}
				}
				if (isNot == 0)
				{
					break;
				}
			}

			if (isNot == 0)
			{
				for (vector<double>::iterator i = delta.begin(); i != delta.end(); ++i)
				{
					if ((*i) <= 0)
						continue;
					else
						isEndFlag = 0;
				}
				for (vector<double>::iterator i = deltaW.begin(); i != deltaW.end(); ++i)
				{
					if ((*i) <= 0)
					{
						continue;
					}
					
					else
					{
						isEndFlagW = 0;
					}
				}

				bool flagIsNeg = 1;
				for (vector<double>::iterator i = deltaW.begin(); i != deltaW.end(); ++i)
				{
					if ((*i) > 0)
					{
						flagIsNeg = 0;
						break;
					}
					int a = distance(deltaW.begin(), i);
					int b = deltaW.size();
					if ((flagIsNeg == 1) && (distance(deltaW.begin(), i) == deltaW.size()-1 ))
					{
						cerr << "Решения нет. Искусственный вектор еще в базисе, а оценки для него отрицаетльны"<<endl;
						system("pause");
						exit(1);
					}
				}

			}
			else
			{
				for (vector<double>::iterator i = delta.begin(); i != delta.end()-C; ++i)
				{
					if ((*i) <= 0)
						continue;
					else
						isEndFlag = 0;
				}
			}
			continue; // если еще есть оценки в deltaW
		}
		if (isEndFlag == false)
		{
			//поиск минимального
			double min=10000000000000 ; //
			bool flagMinChanged = 0;
			int minIndex;// на выходе получим индекс минимального элемента
			int indexForTableCol; // в каком столбце положительная оценка
			int indexPositive; // номер нового вектора в базисе
			for (vector<double>::iterator i = delta.begin(); i != delta.end(); ++i) //перва проверим для W
			{
				if ((*i) > 0)
				{
					indexPositive = distance(delta.begin(), i); //  находим номер вектора, входящий в новый базис

					indexForTableCol = 4 + indexPositive;

					for (int j = 0; j != Table.size(); j++)
					{
						if (Table[j][indexForTableCol] <= 0)
						{
							continue;
						}
						else
						{
							double theta = (Table[j][3] * 1.0) / Table[j][indexForTableCol];
							if (j == 0)
							{
								flagMinChanged = true;
								min = theta;
								minIndex = j;//запоминаем строковый индекс минимального тета
							}
							else
							{
								if (theta < min)
								{
									flagMinChanged = true;
									min = theta;
									minIndex = j;
								}
							}
						}
					}
					if (flagMinChanged == false)
					{
						cerr << "min не достигается в области ограничений D" << endl;
						system("pause");
						exit(1);
					}
					break; // нашли первый попапвшийся положительный элемент
				}
				else
					continue;
			}
			//Выведем вектор из базиса
			Table[minIndex][0] = indexPositive + 1; // перепишем индекс
			Table[minIndex][1] = c[indexPositive]; // перепишем с
			Table[minIndex][2] = CW[indexPositive]; // перепишем cw
			//Гауссовские преобразования:
			IT = Table.begin() + minIndex;
			// преобразования в самой строке
			double tempDenominator = Table[minIndex][indexForTableCol];
			for (vector<double>::iterator JT = (*IT).begin() + 3; JT != (*IT).end(); ++JT)
			{
				(*JT) = (1.0*(*JT)) / tempDenominator; // получаем единицу на нужном месте
			}

			//проход по столбцу и запись чисел, на которые нужно будет умножить строку, чтобы вычитать из других
			vector < double	> multipliers(Table.size() );
			for (int i = 0; i != Table.size(); ++i)
			{
				multipliers[i] = Table[i][indexForTableCol];
			}
			//int i = 0; 
			int k = 0;
			for (vector<vector<double>>::iterator IT = Table.begin() ; IT != Table.end(); ++IT)
			{
				if (distance(Table.begin(), IT) == minIndex)
				{
					++k;
					continue;
				}
				else
				{
					/*if (distance(Table.begin(), IT) == 0)
						++i;*/
					int j = 0 + 3;
					for (vector<double>::iterator JT = (*IT).begin() + 3; JT != (*IT).end(); ++JT)
					{
						(*JT) = (*JT) - Table[minIndex][j] * multipliers[k];
						
						++j;
					}
					//++i;
					++k;
				}
			}

			//подсчет оценок для  базиса
			delta = vector<double>(c.size());
			deltaW = vector<double>(c.size());

			vector<double>::iterator iC = c.begin();
			vector<double>::iterator iCw = CW.begin();
			vector<double>::iterator id = delta.begin();
			vector<double>::iterator idw = deltaW.begin();

			IT = Table.begin();
			int col = 0;
			for (vector<double>::iterator JT = (*IT).begin() + 4; JT != (*IT).end(); ++JT)
			{

				for (vector<vector<double>>::iterator ITinner = Table.begin(); ITinner != Table.end(); ++ITinner)
				{
					vector<double>::iterator JTC = (*ITinner).begin() + 1;
					vector<double>::iterator JTCw = (*ITinner).begin() + 2;
					vector<double>::iterator JTinner = (*ITinner).begin() + 4 + col;
					(*id) += (*JTC)* (*JTinner);
					(*idw) += (*JTCw)* (*JTinner);


				}
				(*id) -= (*iC);
				(*idw) -= (*iCw);
				++iC;
				++iCw;
				++id;
				++idw;
				++col;
			}

			cout << "Коэффициенты С:" << endl;
			for (vector<double>::iterator i = c.begin(); i != c.end(); ++i)
				cout << "|  " << (*i) << "  |";
			cout << endl << "Коэффициенты Сw:" << endl;
			for (vector<double>::iterator i = CW.begin(); i != CW.end(); ++i)
				cout << "|  " << (*i) << "  |";
			cout << endl << "Таблица:" << endl << "B         |  C-базисное  |  Cw-базисное  |  B^-1*P_0  |";
			for (vector<double>::iterator i = c.begin(); i != c.end(); ++i)
				cout << "    B^-1*P_" << distance(c.begin(), i) + 1 << "  |";
			cout << endl;
			for (vector<vector<double>>::iterator IT = Table.begin(); IT != Table.end(); ++IT)
			{
				for (vector<double>::iterator JT = (*IT).begin(); JT != (*IT).end(); ++JT)
				{
					cout << *JT << "              ";
				}
				cout << endl;
			}
			cout << "Оценки:" << endl << "                                                              ";
			for (vector<double>::iterator i = delta.begin(); i != delta.end(); ++i)
			{
				cout << (*i) << "            ";
			}
			cout << endl;
			cout << "Оценки w:" << endl << "                                                            ";
			for (vector<double>::iterator i = deltaW.begin(); i != deltaW.end(); ++i)
			{
				cout << (*i) << "            ";
			}
			cout << endl;
			F = 0; //подсчет ЗЛФ
			Fw = 0;
			for (int i = 0; i != Table.size(); i++)
			{
				Fw += Table[i][2] * Table[i][3];
				F += Table[i][1] * Table[i][3];
			}
			cout << " ЗЛФ = " << F << endl;
			cout << " ЗЛФ при w= " << Fw << endl;

			//проверка на оптимальность плана
			isEndFlag = 1;
			isEndFlagW = 1;
			int C = 0;
			for (vector<double>::iterator iCW = CW.begin(); iCW != CW.end(); ++iCW)
			{
				if ((*iCW) != 0)
				{
					C++;
				}
			}
			for (vector<double>::iterator i = delta.begin(); i != delta.end()-C; ++i)
			{
				if ((*i) <= 0)
					continue;
				else
					isEndFlag = 0;
			}
			if (isEndFlag == 1)
			{
				break;
			}

		}
	}

	if (edited_positions.empty() != true)
	{
		double counter = 0; //здесь убираем не нужные доп и искусственные переменные
		for (int i = 0; i != c.size(); i++)
		{
			if (c[i] != 0)
				counter++;
		}

		xi = vector<double>(counter);
		for (vector<vector<double>>::iterator IT = Table.begin(); IT != Table.end(); ++IT)
		{
			vector<double>::iterator JT = (*IT).begin();
			int findedIndex = (*JT) - 1;
			vector<double>::iterator JTP0 = (*IT).begin() + 3;
			if (findedIndex < counter)
				xi[findedIndex] = (*JTP0);
		}
		for (vector<int>::iterator it = edited_positions.begin(); it != edited_positions.end(); ++it)
		{
			vector<double>::iterator itxi = xi.begin();
			vector<double>::iterator itc = c.begin();

			int findedIndex1 = distance(edited_positions.begin(), it);
			int findedIndex2 = findedIndex1 + 1;
			xi[findedIndex1] -= xi[findedIndex2];

			itxi += findedIndex2;
			itc += findedIndex2;
			xi.erase(itxi);
			c.erase(itc);

			for (vector<int>::iterator it1 = edited_positions.begin() + distance(edited_positions.begin(), it); it1 != edited_positions.end(); ++it1)
			{
				(*it1) -= 1;
			}
		}

		if (isChangedExtremum == true)
		{
			F *= -1;
			cout << endl << "F* = " << F << endl;
		}
		else
			cout << endl << "F* = " << F << endl;

		
		cout << endl << "Вектор найденных значений переменных" << endl;
		for (int i = 0; i != xi.size(); i++)
		{
			cout << "xi[" << i + 1 << "] = " << xi[i] << "  ";
		}
		cout << endl;
	}
	else
	{
		if (isChangedExtremum == true)
		{
			F *= -1;
			cout << endl << "F* = " << F << endl;
		}
		else
			cout <<endl<< "F* = " << F << endl;
		//запись в вектор перменных кси
		
		double counter = 0; //здесь убираем не нужные доп и искусственные переменные
		
		for (int i = 0; i != c.size(); i++)
		{
			if (c[i] != 0)
				counter++;
		}
		xi = vector<double>(counter);
		for (vector<vector<double>>::iterator IT = Table.begin(); IT != Table.end(); ++IT)
		{
			vector<double>::iterator JT = (*IT).begin();
			int findedIndex = (*JT) - 1;
			vector<double>::iterator JTP0 = (*IT).begin() + 3;
			if (findedIndex < counter)
				xi[findedIndex] = (*JTP0);
		}
		cout << endl << "Вектор найденных значений переменных" << endl;
		for (int i = 0; i != xi.size(); i++)
		{
			cout << "xi[" << i + 1 << "] = " << xi[i] << "  ";
		}
		cout << endl;
	}
	
}

const simplex & simplex::operator=(const simplex & other)
{
	if (this != &other)
	{
		n=other.n;
		m = other.m;
		L = other.L;
		b = other.b;
		signs = other.signs;
		extr = other.extr;

		delta = other.delta;
		deltaW = other.deltaW;
		F = other.F;
	}
	return (*this);
}

void simplex::setLSize(int N, int M)
{
	
	L = vector<vector<double>>(M, vector<double>(N));
}

void simplex::setCSize(int N)
{
	c = vector<double>(N);
}

void simplex::setBSize(int M)
{
	b = vector<double>(M);
}

void simplex::setSignsSize(int M)
{
	signs = vector<string>(M);
}

void simplex::setVariable_signsSize(int M)
{
	variable_signs = vector<string>(M);
}

void simplex::setCWSize(int N)
{
	CW = vector<double>(N);
	for (int i = 0; i != CW.size(); i++)
	{
		CW[i] = 0;
	}
}


simplex::~simplex()
{
}

ostream & operator<<(ostream & output, simplex & obj)
{
	output << "************************************************************************************************************************" << endl;
	output << "Задача линейного программирования:" << endl<<endl;
	for (vector<double>::iterator it = obj.c.begin(); it != obj.c.end(); ++it)
	{
		int i = distance(obj.c.begin(), it);
		if (it != obj.c.end() - 1)
		{
			if (*it > 0)
			{
				output << (*it) << "[x_" << i + 1 << "]" << "+";
			}
			else
			{

				output <<"("<< (*it)<<")" << "[x_" << i + 1 << "]" << "+";
			}
		}
		else
		{
			if (*it > 0)
			{
				output << (*it) << "[x_" << i + 1 << "] " << " -> " << obj.extr << endl;
			}
			else
				output <<"("<< (*it)<<")" << "[x_" << i + 1 << "] " << " ->  " << obj.extr << endl;
		}
	}
	output << endl;
	vector<double>::iterator itB = obj.b.begin();
	vector<string>::iterator itSigns = obj.signs.begin();
	vector<vector<double>>::iterator itI = obj.L.begin();
	while ((itI != obj.L.end()) && (itSigns != obj.signs.end()) && (itB != obj.b.end()))
	{
		for (vector<double>::iterator itJ = (*itI).begin(); itJ != (*itI).end(); ++itJ)
		{
			if (itJ != (*itI).end() - 1)
			{
				if (*itJ >= 0)
					output << (*itJ) << "[x_"<<distance((*itI).begin(), itJ)+1<<"]  +" << "  ";
				else
					output << "(" << (*itJ) << ")" << "[x_" << distance((*itI).begin(), itJ) + 1 << "]  +" << "  ";
			}
			else
			{
				if (*itJ >= 0)
					output << (*itJ) << "[x_" << distance((*itI).begin(), itJ) + 1 << "]   ";
				else
					output << "(" << (*itJ) << ")" << "[x_" << distance((*itI).begin(), itJ) + 1 << "]   ";
			}
		}
		output << (*itSigns)<<"  ";
		output << (*itB)<<endl;
		itI++;
		itB++;
		itSigns++;
	}

	cout << endl;
	for (vector<string>::iterator itVS = obj.variable_signs.begin(); itVS != obj.variable_signs.end(); itVS++)
	{
		output << "x_" << distance(obj.variable_signs.begin(), itVS + 1) << " " << *itVS<<"  " << 0<<endl;
	}
	cout << endl;
	if (obj.checkCanon() == 1)
		output << "В каноническом виде" << endl;
	else
		output << "Не в каноническом виде" << endl;
	return output;
}

istream & operator >> (istream & input, simplex & obj)
{
	cout << "Введите количество переменных n = ";
	input >> obj.n;
	cout << endl<< "Введите количество ограничений m = ";
	input >> obj.m;
	obj.setCSize(obj.n);
	cout << endl << "Введите max/min(f) " << endl;
	input >> obj.extr;
	cout << endl << "Введите коэффициенты при целевой функции f = " << endl;
	for (vector<double>::iterator it=obj.c.begin();it!=obj.c.end();++it)
		input >> (*it);
	obj.setLSize(obj.n, obj.m);
	obj.setBSize(obj.m);
	obj.setSignsSize(obj.m);
	obj.setVariable_signsSize(obj.n);
	obj.setCWSize(obj.n);
	cout << endl << "Введите систему ограничений L (>|<|=) b " << endl;
	vector<double>::iterator itB = obj.b.begin();
	vector<string>::iterator itSigns = obj.signs.begin();
	vector<vector<double>>::iterator itI = obj.L.begin();
	while((itI != obj.L.end())&&(itSigns != obj.signs.end())&&(itB != obj.b.end()))
	{
		for (vector<double>::iterator itJ = (*itI).begin(); itJ != (*itI).end(); ++itJ)
		{
			input >> (*itJ);
		}
		input >> (*itSigns);
		input >> (*itB);
		itI++;
		itB++;
		itSigns++;
	}
	cout << endl << "Введите знаки переменных в формате (x_i >= | any 0):" << endl;
	for (vector<string>::iterator itVS = obj.variable_signs.begin(); itVS != obj.variable_signs.end(); itVS++)
	{
		cout << "x_" << distance(obj.variable_signs.begin(), itVS + 1) << " ";
		input >> *itVS;
	}
	return input;
}
/*MADE BY RUSTAM TALIPOV*/