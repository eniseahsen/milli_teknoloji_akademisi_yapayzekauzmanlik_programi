#include <stdio.h>
#include <omp.h>  // OpenMP API

int main() {
    int i;
    int sum = 0;

    // Parallel for döngüsü. Her thread 'i' deðerlerini paylaþýr,
    // 'sum' deðiþkeni reduction ile toplanýr.
    #pragma omp parallel for reduction(+:sum)
    for (i = 0; i < 10; i++) {
        sum += i;
        printf("Thread %d is adding %d | sum = %d\n",
               omp_get_thread_num(), i, sum);
    }

    printf("Final sum = %d\n", sum);
    return 0;
}
