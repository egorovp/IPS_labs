#include <iostream>
#include <omp.h>
#include <conio.h>

long long num = 100000000;
double step;

double par(void)
{
	int num_of_threads = 3;
	long long inc = 0;
	long long i = 0;
	double x = 0.0;
	double pi;
	double S = 0.0;
	step = 1.0 / (double)num;
	omp_lock_t inc_lock;
	omp_init_lock(&inc_lock);
	double t = omp_get_wtime();
#pragma omp parallel for num_threads(3) private(x) reduction(+ : S)
	for (i = 0; i < num; i++)
	{
		x = (i + 0.5)*step;
		S = S + 4.0 / (1.0 + x*x);
		omp_set_lock(&inc_lock);
		inc++;
		omp_unset_lock(&inc_lock);
	}
	t = omp_get_wtime() - t;
	omp_destroy_lock(&inc_lock);
	pi = step * S;
	printf("Par: pi = %.14f\n", pi);
	return t;
}

int main()
{
	printf("time: %f sec.\n\n", par());
	getch();
	return 0;
}