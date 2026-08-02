/*
 * coin_toss_simulation.c
 *
 * Part 1: Simulate a FAIR coin many times and show that the
 *         proportion of HEADs converges to about 0.5.
 *
 * Part 2: Extend the simulation to a BIASED coin (user-chosen
 *         probability of HEAD) and compare the two side by side,
 *         at several different trial counts (to show convergence).
 */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

/* Simulate n tosses of a coin with P(HEAD) = p_head.
 * Returns the number of heads observed. */
long simulate_tosses(long n, double p_head) {
    long heads = 0;
    for (long i = 0; i < n; i++) {
        /* rand() gives [0, RAND_MAX]; convert to a uniform double in [0,1) */
        double r = (double) rand() / ((double) RAND_MAX + 1.0);
        if (r < p_head) {
            heads++;
        }
    }
    return heads;
}

void run_experiment(const char *label, long n, double p_head) {
    long heads = simulate_tosses(n, p_head);
    double observed_prob = (double) heads / (double) n;

    printf("%-20s | Tosses: %10ld | True P(H)=%.2f | Heads: %10ld | "
           "Observed P(H) = %.5f\n",
           label, n, p_head, heads, observed_prob);
}

int main(void) {
    srand((unsigned int) time(NULL));

    long trial_sizes[] = {100, 1000, 10000, 100000, 1000000, 10000000};
    int num_sizes = sizeof(trial_sizes) / sizeof(trial_sizes[0]);

    double fair_p = 0.5;    /* fair coin   */
    double biased_p = 0.75; /* biased coin: 75% chance of heads */

    printf("=================================================================\n");
    printf(" PART 1: FAIR COIN - convergence toward P(HEAD) = 0.5\n");
    printf("=================================================================\n");
    for (int i = 0; i < num_sizes; i++) {
        run_experiment("Fair coin", trial_sizes[i], fair_p);
    }

    printf("\n=================================================================\n");
    printf(" PART 2: FAIR vs BIASED coin comparison\n");
    printf("=================================================================\n");
    for (int i = 0; i < num_sizes; i++) {
        run_experiment("Fair coin",   trial_sizes[i], fair_p);
        run_experiment("Biased coin", trial_sizes[i], biased_p);
        printf("-----------------------------------------------------------------\n");
    }

    return 0;
}
