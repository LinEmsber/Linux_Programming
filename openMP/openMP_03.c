#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

int main()
{
	int i;
	int r = 0, c = 0, a = 0;
	int iter = 500000;

	printf("OpenMP threads = %d\n", omp_get_max_threads() );

	#pragma omp parallel reduction(+:r) shared(c, a)
	{
		#pragma omp for
		for ( i = 0; i < iter; i++ ) {
			r++;
			#pragma omp critical
			c++;
			#pragma omp atomic
			a++;
		}
	}
	printf("reduce   = %d\n"
		"critical = %d\n"
		"atomic   = %d\n", r, c, a);

	return 0;
}
