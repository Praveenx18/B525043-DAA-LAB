/*
 * simple_growth.c
 * ---------------------------------------------------
 * Prints the value of each function for a chosen n,
 * IN THE ORDER OF INCREASING GROWTH (already worked out
 * on paper).
 *
 * NOTE: 2^(32n) and 3^n get too big for a double very
 * fast, so for just those two we print log2(value)
 * instead of the value itself (clearly labeled).
 *
 * Compile:  gcc -o simple_growth simple_growth.c -lm
 * Run:      ./simple_growth
 * ---------------------------------------------------
 */
/*
 Comparing exponents/growth classes
 (I verified this with the C program below),
 the increasing order of growth is:

1/𝑛<log 𝑛<12 sqrt(𝑛)  < 50𝑛^0.5<𝑛^0.51<𝑛log𝑛<𝑛^2−324<100𝑛^2+6𝑛 < 2𝑛^3 < 𝑛^log𝑛  < 3^𝑛  <  (2^32)*𝑛
*/

 /*A function with a technically smaller growth rate can still be
 larger in absolute value for a long stretch of small-to-moderately-large n.
 For example n^0.51 vs 50·n^0.5, the crossover happens at
n = 50^100 ≈ 1.27 × 10^170. Before that point, 50·n^0.5 is numerically larger
 This is a good illustration of what "sufficiently large n"
really means in Big-O: it's a mathematical guarantee about
the eventual trend (the 0.01 extra exponent will always win in the end,
no matter how big the constant 50 is),
but "eventually" can be a genuinely enormous number,
far beyond any n you'd ever compute with in practice.*/


#include <stdio.h>
#include <math.h>

int main() {
    double n = 10000;   // try changing this and re-running

    double log2n = log2(n);   // log base 2 of n, used a few times below

    printf("Values at n = %.0f, in increasing order of growth:\n\n", n);

    printf("1/n              = %f\n",  1.0 / n);
    printf("log2(n)          = %f\n",  log2n);
    printf("12*sqrt(n)       = %f\n",  12 * sqrt(n));
    printf("50*n^0.5         = %f\n",  50 * pow(n, 0.5));
    printf("n^0.51           = %f\n",  pow(n, 0.51));
    printf("n*log2(n)        = %f\n",  n * log2n);
    printf("n^2 - 324        = %f\n",  n * n - 324);
    printf("100n^2 + 6n      = %f\n",  100 * n * n + 6 * n);
    printf("2n^3             = %f\n",  2 * n * n * n);
    printf("n^(log2 n)       = %f\n",  pow(n, log2n));

    // These two get astronomically large, so print log2(value) instead
    printf("3^n   -> log2 of it       = %f\n", n * log2(3.0));
    printf("2^(32n) -> log2 of it     = %f\n", 32 * n);

    return 0;
}
