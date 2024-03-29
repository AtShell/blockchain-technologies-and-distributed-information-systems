﻿#include <omp.h>
#include <random>
#include <iostream>
using namespace std;

bool search_reverse_matrix(vector <vector<double>>& matrix)
{
	int size = matrix.size();
	vector <vector<double>> E(size, vector<double>(size));

	//Заполнение единичной матрицы
	for (int i = 0; i < size; i++)
	{
		for (int j = 0; j < size; j++)
		{
			if (i == j) E[i][j] = 1.0;
			else E[i][j] = 0.0;
		}
	}
	for (int k = 0; k < size; k++)
	{
		if (abs(matrix[k][k]) < 1e-8)
		{
			bool changed = false;

			for (int i = k + 1; i < size; i++)
			{
				if (abs(matrix[i][k]) > 1e-8)
				{
					swap(matrix[k], matrix[i]);
					swap(E[k], E[i]);
					changed = true;
					break;
				}
			}

			if (!changed)
				return false;
		}

		double div = matrix[k][k];

#pragma omp parallel
		{
#pragma omp for
			for (int j = 0; j < size; j++)
			{
				matrix[k][j] /= div;
				E[k][j] /= div;
			}
		}

#pragma omp parallel
		{
#pragma omp for
			for (int i = k + 1; i < size; i++)
			{
				double multi = matrix[i][k];


				for (int j = 0; j < size; j++)
				{
					matrix[i][j] -= multi * matrix[k][j];
					E[i][j] -= multi * E[k][j];
				}
			}
		}
	}

	for (int k = size - 1; k > 0; k--)
	{
#pragma omp parallel
		{
#pragma omp for
			for (int i = k - 1; i > -1; i--)
			{
				double multi = matrix[i][k];

				for (int j = 0; j < size; j++)
				{
					matrix[i][j] -= multi * matrix[k][j];
					E[i][j] -= multi * E[k][j];
				}
			}
		}
	}
	matrix = E;
	return true;
}

double random(const int min, const int max)
{
	if (min == max)
		return min;
	return min + rand() % (max - min);
}

void print(vector<vector<double>>matrix, int equations_amount, vector<double>B) {
	// Вывод системы уравнений
	cout << "\nСистема уравнений:\n";
	for (int i = 0; i < equations_amount; i++)
	{
		for (int j = 0; j < equations_amount; j++)
		{

			if (j != 0 && matrix[i][j] >= 0)
				cout << " +";
			cout << " " << matrix[i][j] << "x" << j + 1;
		}
		cout << " = " << B[i] << endl;
	}
}
void printa(int equations_amount, vector<double>X) {
	// Вывод окончательного результата
	cout << "\nРешение системы уравнений:";
	for (int i = 0; i < equations_amount; i++)
		cout << "\nx" << i + 1 << " = " << X[i];
}

int main()
{
	setlocale(LC_ALL, "RUS");
	int equations_amount;
	int prints;
	cout << "Введите количество уравнений: ";
	cin >> equations_amount;
	cout << "Выводить систему и результат?\n 1-Да    2-Нет\n";
	cin >> prints;

	vector<vector<double>> matrix(equations_amount, vector<double>(equations_amount));
	vector<double> B(equations_amount);

	// Заполняем матрицу коэффициентов и B
	for (int i = 0; i < equations_amount; i++)
	{
		for (int j = 0; j < equations_amount; j++)
			matrix[i][j] = random(0, 100);
		B[i] = random(0, 100);
	}
	if (prints == 1)
		print(matrix, equations_amount, B);

	double t = clock();
	if (!search_reverse_matrix(matrix))
	{
		cout << "\nНевозможно найти обратную матрицу!\n";
		exit(1);
	}

	vector<double> X(equations_amount);
#pragma omp parallel
	{
#pragma omp for
		for (int i = 0; i < equations_amount; i++)
		{
			X[i] = 0;
			for (int j = 0; j < equations_amount; j++)
				X[i] += matrix[i][j] * B[j];
		}
	}
	if (prints == 1)
		printa(equations_amount, X);

	t = (clock() - t) / 1000;
	cout << "\n\nВремя, затраченное на вычисление: " << t << "с.\n";
	return 0;
}

