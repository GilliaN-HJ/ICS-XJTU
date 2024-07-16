#include "poly.h"
#include <time.h>
#include <math.h>

/*
void poly(const double a[], double x, long degree, double *result) {
    long i;
    double r = a[degree];
    for (i = degree - 1; i >= 0; i--) {
        r = a[i] + r * x;
    }
    *result = r;
}
*/

void poly_optim(const double a[], double x, long degree, double *result) {
    // your code here

    double poly[10]; 
    //init poly
    poly[0] = 1.0;
    for (long j = 1; j < 10; j++) {
        poly[j] = poly[j - 1] * x;
    } 
    
    double rest = 0.0;
    double r1 = a[degree];
    double r2 = a[degree - 1];
    double r3 = a[degree - 2];
    double r4 = a[degree - 3];
    double r5 = a[degree - 4];
    double r6 = a[degree - 5];
    double r7 = a[degree - 6];

    long i;
    double tp = poly[7];
    for (i = degree - 7; i >= 7; i -= 7) {
        r1 = a[i - 0] + r1 * tp;
        r2 = a[i - 1] + r2 * tp;
        r3 = a[i - 2] + r3 * tp;
        r4 = a[i - 3] + r4 * tp;
        r5 = a[i - 4] + r5 * tp;
        r6 = a[i - 5] + r6 * tp;
        r7 = a[i - 6] + r7 * tp;

    }

    tp = poly[i];
    r7 *= poly[1] * tp;
    r6 *= poly[2] * tp;
    r5 *= poly[3] * tp;
    r4 *= poly[4] * tp;
    r3 *= poly[5] * tp;
    r2 *= poly[6] * tp;
    r1 *= poly[7] * tp;
   
    //rest
    for (; i >= 0; i--) {
        rest = a[i] + rest * x;
    } 

    *result = r1 + r2 + r3 + r4 + r5 + r6 + r7 + rest;

}

void measure_time(poly_func_t poly, const double a[], double x, long degree,
                  double *time) {
    // your code here
    int N = 100;
    double result = 0;
    double * pr = &result;
    double count[N] ;
    double sum = 0.0;

    // timespec
    struct timespec begin ;
    struct timespec end;
    
    *time = 0.0;

    for (int i = 0; i < N; i++){
        // TO AVOID Cache miss
        poly(a, x, degree, pr);

        // count
        clock_gettime(CLOCK_REALTIME, &begin);
        poly(a, x, degree, pr);
        clock_gettime(CLOCK_REALTIME, &end);

        count[i] = end.tv_nsec - begin.tv_nsec + (end.tv_sec - begin.tv_sec) * 1e9;

    }

    // averary
    for (int i = 0;i<N;i++){
        sum += count[i];
    }

    *time = sum/(double) N;
}
