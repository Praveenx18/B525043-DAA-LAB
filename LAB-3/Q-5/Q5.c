#include <stdio.h>
#include <stdlib.h>

/* Function to create an n x n matrix */
int** createMatrix(int n)
{
    int i;
    int **matrix;

    matrix = (int**)malloc(n * sizeof(int*));

    for (i = 0; i < n; i++)
    {
        matrix[i] = (int*)malloc(n * sizeof(int));
    }

    return matrix;
}

/* Function to free matrix memory */
void freeMatrix(int **matrix, int n)
{
    int i;

    for (i = 0; i < n; i++)
    {
        free(matrix[i]);
    }

    free(matrix);
}

/* Function to add two matrices */
void addMatrix(int **A, int **B, int **C, int n)
{
    int i, j;

    for (i = 0; i < n; i++)
    {
        for (j = 0; j < n; j++)
        {
            C[i][j] = A[i][j] + B[i][j];
        }
    }
}

/* Function to subtract two matrices */
void subtractMatrix(int **A, int **B, int **C, int n)
{
    int i, j;

    for (i = 0; i < n; i++)
    {
        for (j = 0; j < n; j++)
        {
            C[i][j] = A[i][j] - B[i][j];
        }
    }
}

/*
   Divide and Conquer multiplication
   for special-pattern matrices
*/
void specialMultiply(int **A, int **B, int **C, int n)
{
    int i, j;
    int half;

    int **A1;
    int **A2;
    int **B1;
    int **B2;

    int **Aplus;
    int **Aminus;
    int **Bplus;
    int **Bminus;

    int **P;
    int **Q;

    /* Base case */
    if (n == 1)
    {
        C[0][0] = A[0][0] * B[0][0];
        return;
    }

    half = n / 2;

    /* Create smaller matrices */
    A1 = createMatrix(half);
    A2 = createMatrix(half);
    B1 = createMatrix(half);
    B2 = createMatrix(half);

    Aplus = createMatrix(half);
    Aminus = createMatrix(half);
    Bplus = createMatrix(half);
    Bminus = createMatrix(half);

    P = createMatrix(half);
    Q = createMatrix(half);

    /* Divide A and B into four blocks */
    for (i = 0; i < half; i++)
    {
        for (j = 0; j < half; j++)
        {
            A1[i][j] = A[i][j];
            A2[i][j] = A[i][j + half];

            B1[i][j] = B[i][j];
            B2[i][j] = B[i][j + half];
        }
    }

    /* Calculate A1 + A2 and A1 - A2 */
    addMatrix(A1, A2, Aplus, half);
    subtractMatrix(A1, A2, Aminus, half);

    /* Calculate B1 + B2 and B1 - B2 */
    addMatrix(B1, B2, Bplus, half);
    subtractMatrix(B1, B2, Bminus, half);

    /*
       Only two recursive multiplications are required
    */
    specialMultiply(Aplus, Bplus, P, half);
    specialMultiply(Aminus, Bminus, Q, half);

    /*
       Construct C1 = (P + Q) / 2
       Construct C2 = (P - Q) / 2
    */
    for (i = 0; i < half; i++)
    {
        for (j = 0; j < half; j++)
        {
            C[i][j] = (P[i][j] + Q[i][j]) / 2;

            C[i][j + half] = (P[i][j] - Q[i][j]) / 2;

            C[i + half][j] = C[i][j + half];

            C[i + half][j + half] = C[i][j];
        }
    }

    /* Free allocated memory */
    freeMatrix(A1, half);
    freeMatrix(A2, half);
    freeMatrix(B1, half);
    freeMatrix(B2, half);

    freeMatrix(Aplus, half);
    freeMatrix(Aminus, half);
    freeMatrix(Bplus, half);
    freeMatrix(Bminus, half);

    freeMatrix(P, half);
    freeMatrix(Q, half);
}

/* Function to print a matrix */
void printMatrix(int **A, int n)
{
    int i, j;

    for (i = 0; i < n; i++)
    {
        for (j = 0; j < n; j++)
        {
            printf("%d ", A[i][j]);
        }

        printf("\n");
    }
}

int main()
{
    int n;
    int i, j;

    int **A;
    int **B;
    int **C;

    printf("Enter size of matrix: ");
    scanf("%d", &n);

    /* n must be a power of 2 */
    if (n < 1 || (n & (n - 1)) != 0)
    {
        printf("Matrix size must be a power of 2.\n");
        return 0;
    }

    A = createMatrix(n);
    B = createMatrix(n);
    C = createMatrix(n);

    printf("Enter first matrix:\n");

    for (i = 0; i < n; i++)
    {
        for (j = 0; j < n; j++)
        {
            scanf("%d", &A[i][j]);
        }
    }

    printf("Enter second matrix:\n");

    for (i = 0; i < n; i++)
    {
        for (j = 0; j < n; j++)
        {
            scanf("%d", &B[i][j]);
        }
    }

    specialMultiply(A, B, C, n);

    printf("\nResultant matrix:\n");
    printMatrix(C, n);

    freeMatrix(A, n);
    freeMatrix(B, n);
    freeMatrix(C, n);

    return 0;
}