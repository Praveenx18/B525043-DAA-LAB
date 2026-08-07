# Merging K Sorted Arrays

A C program comparing two algorithms for merging `k` sorted arrays (each of size `n`) into a single sorted array of `kn` elements.

## Problem

Given `k` sorted arrays, each with `n` elements, combine them into one sorted array containing all `kn` elements.

## Methods Implemented

### Method 1 — Sequential Merging
Merge the first two arrays, then merge that result with the third array, then the fourth, and so on until all `k` arrays are combined.

- **Worst-case running time:** `O(k²n)`
- Each merge step touches an increasingly large "running" array, so the total work is proportional to `n(1 + 2 + ... + k)`.

### Method 2 — Pairwise (Divide & Conquer) Merging
Split the `k` arrays into `k/2` pairs, merge each pair (producing `k/2` arrays of size `2n`), then repeat on the resulting arrays until only one array remains.

- **Running time:** `O(kn log k)`
- There are `O(log k)` rounds, and each round does `O(kn)` total work across all merges.

Method 2 is asymptotically faster than Method 1 for large `k`.

## Files

| File | Description |
|---|---|
| `merge_k.c` | Source code: merge subroutine, both algorithms, random test generation, correctness validation, and timing benchmark |

## Build

```bash
gcc -O2 -o merge_k merge_k.c
```

## Run

```bash
./merge_k
```

## What the Program Does

1. **Validation** — Generates random sorted arrays, runs both algorithms, and checks the output against a "ground truth" produced by flattening all arrays and sorting them with `qsort`. Runs multiple trials, including an odd value of `k` to test the leftover-array edge case in Method 2.
2. **Benchmark** — Times both algorithms on several `(k, n)` configurations and prints the elapsed time for each, so you can observe how Method 1 degrades relative to Method 2 as `k` grows.

### Sample output shape

```
Validation passed for k=8, n=15 (20 trials)
Validation passed for k=7, n=20 (20 trials)

Benchmark (sequential vs pairwise):
k=   10, n= 1000 | Sequential:   0.0003s | Pairwise:   0.0002s
k=  100, n= 1000 | Sequential:   0.0210s | Pairwise:   0.0031s
k=  500, n=  500 | Sequential:   0.2650s | Pairwise:   0.0142s
k= 1000, n=  200 | Sequential:   0.4980s | Pairwise:   0.0198s
```

(Exact numbers vary by machine, but the gap between the two methods should widen as `k` increases.)

## Function Reference

| Function | Purpose |
|---|---|
| `merge(a, na, b, nb)` | Merges two sorted arrays into a newly allocated sorted array |
| `merge_k_sequential(arrays, k, n, *out_len)` | Implements Method 1 |
| `merge_k_pairwise(arrays, k, n, *out_len)` | Implements Method 2 |
| `make_test_arrays(k, n)` | Generates `k` random sorted arrays of size `n` |
| `validate(k, n, trials)` | Correctness check against a sorted reference array |
| `benchmark(k, n)` | Times and prints both methods for given `k`, `n` |

## Notes

- Memory allocated during merging is freed as soon as it's no longer needed to avoid leaks during repeated runs.
- The array-generation step uses `rand()`; seed it differently (`srand`) if you need reproducible test data.