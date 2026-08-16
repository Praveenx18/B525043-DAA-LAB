#include <stdio.h>
#include <stdlib.h>

// Allocate an n x n matrix dynamically
int** allocMatrix(int n) {
    int **M = (int **)malloc(n * sizeof(int *));
    for (int i = 0; i < n; i++)
        M[i] = (int *)malloc(n * sizeof(int));
    return M;
}

void freeMatrix(int **M, int n) {
    for (int i = 0; i < n; i++) free(M[i]);
    free(M);
}

void add(int **A, int **B, int **C, int n) {
    for (int i = 0; i < n; i++)
        for (int j = 0; j < n; j++)
            C[i][j] = A[i][j] + B[i][j];
}

void subtract(int **A, int **B, int **C, int n) {
    for (int i = 0; i < n; i++)
        for (int j = 0; j < n; j++)
            C[i][j] = A[i][j] - B[i][j];
}

void strassen(int **A, int **B, int **C, int n) {
    if (n == 1) {
        C[0][0] = A[0][0] * B[0][0];
        return;
    }

    int k = n / 2;

    int **A11 = allocMatrix(k), **A12 = allocMatrix(k);
    int **A21 = allocMatrix(k), **A22 = allocMatrix(k);
    int **B11 = allocMatrix(k), **B12 = allocMatrix(k);
    int **B21 = allocMatrix(k), **B22 = allocMatrix(k);

    int **M1 = allocMatrix(k), **M2 = allocMatrix(k), **M3 = allocMatrix(k);
    int **M4 = allocMatrix(k), **M5 = allocMatrix(k);
    int **M6 = allocMatrix(k), **M7 = allocMatrix(k);

    int **T1 = allocMatrix(k), **T2 = allocMatrix(k);

    // Divide into submatrices
    for (int i = 0; i < k; i++) {
        for (int j = 0; j < k; j++) {
            A11[i][j] = A[i][j];
            A12[i][j] = A[i][j + k];
            A21[i][j] = A[i + k][j];
            A22[i][j] = A[i + k][j + k];
            B11[i][j] = B[i][j];
            B12[i][j] = B[i][j + k];
            B21[i][j] = B[i + k][j];
            B22[i][j] = B[i + k][j + k];
        }
    }

    add(A11, A22, T1, k); add(B11, B22, T2, k); strassen(T1, T2, M1, k);  // M1 = (A11+A22)(B11+B22)
    add(A21, A22, T1, k);                        strassen(T1, B11, M2, k); // M2 = (A21+A22)B11
    subtract(B12, B22, T2, k);                    strassen(A11, T2, M3, k); // M3 = A11(B12-B22)
    subtract(B21, B11, T2, k);                    strassen(A22, T2, M4, k); // M4 = A22(B21-B11)
    add(A11, A12, T1, k);                         strassen(T1, B22, M5, k); // M5 = (A11+A12)B22
    subtract(A21, A11, T1, k); add(B11, B12, T2, k); strassen(T1, T2, M6, k); // M6 = (A21-A11)(B11+B12)
    subtract(A12, A22, T1, k); add(B21, B22, T2, k); strassen(T1, T2, M7, k); // M7 = (A12-A22)(B21+B22)

    for (int i = 0; i < k; i++) {
        for (int j = 0; j < k; j++) {
            C[i][j]         = M1[i][j] + M4[i][j] - M5[i][j] + M7[i][j];
            C[i][j + k]     = M3[i][j] + M5[i][j];
            C[i + k][j]     = M2[i][j] + M4[i][j];
            C[i + k][j + k] = M1[i][j] - M2[i][j] + M3[i][j] + M6[i][j];
        }
    }

    int **all[] = {A11,A12,A21,A22,B11,B12,B21,B22,M1,M2,M3,M4,M5,M6,M7,T1,T2};
    for (int i = 0; i < 17; i++) freeMatrix(all[i], k);
}

int isPowerOfTwo(int n) { return n > 0 && (n & (n - 1)) == 0; }

int main() {
    int n;
    printf("Enter size of matrix (power of 2): ");
    scanf("%d", &n);

    if (!isPowerOfTwo(n)) {
        printf("Error: n must be a power of 2 for this implementation.\n");
        return 1;
    }

    int **A = allocMatrix(n), **B = allocMatrix(n), **C = allocMatrix(n);

    printf("Enter elements of Matrix A:\n");
    for (int i = 0; i < n; i++)
        for (int j = 0; j < n; j++)
            scanf("%d", &A[i][j]);

    printf("Enter elements of Matrix B:\n");
    for (int i = 0; i < n; i++)
        for (int j = 0; j < n; j++)
            scanf("%d", &B[i][j]);

    strassen(A, B, C, n);

    printf("\nResultant Matrix:\n");
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++)
            printf("%d ", C[i][j]);
        printf("\n");
    }

    freeMatrix(A, n); freeMatrix(B, n); freeMatrix(C, n);
    return 0;
}