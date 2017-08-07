/* For loop of openMP. */

#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

#define CHUNK_SIZE 10
#define N 2001

int main (int argc, char *argv[])
{
	int i;
	float a[N], b[N], c[N];

	/* Initializations */
	for (i = 0; i < N; i++){
		a[i] = i * 1.0;
		b[i] = a[i] * 10 + i * 1.0;
	}

	#pragma omp parallel shared(a, b, c) private(i)
	{
		for (i = 0; i < N; i++) {
				c[i] = a[i] + b[i];
		}
	}
	printf("c[%d] = %f\n", N - 1, c[N-1]);

	return 0;
}
