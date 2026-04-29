#include "integrate.h"

#include <cstdio>

// Parallel composite trapezoid rule for A1.
// We use dynamic scheduling with a moderate chunk size to balance the spike
// region in f(x) while keeping scheduling overhead low.

double integrate_parallel(double a, double b, long n)
{
    const double h = (b - a) / static_cast<double>(n);
    double sum = 0.5 * (f(a) + f(b));

#pragma omp parallel for default(none) shared(a, n) reduction(+:sum) schedule(dynamic, 64)
    for (long i = 1; i < n; ++i) {
        const double x = a + (static_cast<double>(i) * h);
        sum += f(x);
    }
    return sum * h;
}

int main()
{
    constexpr double A = 0.0;
    constexpr double B = 1.0;
    constexpr long N = 100'000'000;

    const double result = integrate_parallel(A, B, N);
    std::printf("integral = %.12f\n", result);
    return 0;
}
