#include <vector>
#include <stdio.h>
#include <conio.h>
#include <Windows.h>
#include <cilk/cilk.h>
#include <cilk/reducer.h>
#include <cilk/reducer_opadd.h>

/// подынтегральная функция
double func(double x)
{
	return 4 / sqrt(4 - x * x);
}

/// Последовательное вычисление интеграла
/// по формуле трапеций
double integral(int N)
{
	double h = 1 / (double)N;
	double sum = 0.0;
	for(int i = 0; i < N; ++i)
	{
		sum += func((i + 1 / 2) * h);
	}
	return sum * h;
}

/// Параллельное вычисление интеграла
/// по формуле трапеций
double parallel_integral(int N)
{
	double h = 1.0 / (double)N;
	cilk::reducer_opadd<double> sum(0.0);
	cilk_for(auto i = 0; i < N; i++)
	{
		sum += func((i + 1 / 2) * h);
	}
	return sum.get_value() * h;
}

int main()
{
	int N = 10000000;

	DWORD start = GetTickCount();
	double I = integral(N);
	DWORD time = GetTickCount() - start;
	
	start = GetTickCount();
	double Ip = parallel_integral(N);
	DWORD parallel_time = GetTickCount() - start;

	printf("\n serial time    - %d ms\n parallel time  - %d ms\n", time, parallel_time);
	printf("\n serial value   - %f \n parallel value - %f\n ", I, Ip);
	_getch();

	FILE *fs;
	if ((fs = fopen("serial.txt", "w")) == NULL)
	{
		printf("Cannot open file.\n");
		return 0;
	}

	std::vector<int> ns = { 50000, 200000, 1000000, 3000000, 10000000, 30000000 };
	for (int i = 0; i < ns.size(); ++i)
	{
		DWORD start = GetTickCount();
		double I = integral(ns[i]);
		DWORD time = GetTickCount() - start;
		fprintf(fs, "%d ", time);
	}
	fclose(fs);

	FILE *fp;
	if ((fp = fopen("parallel.txt", "w")) == NULL)
	{
		printf("Cannot open file.\n");
		return 0;
	}
	for (int i = 0; i < ns.size(); ++i)
	{
		DWORD start = GetTickCount();
		double I = parallel_integral(ns[i]);
		DWORD time = GetTickCount() - start;
		fprintf(fp, "%d ", time);
	}
	fclose(fp);

	return 0;
}
