#include "matrix.h"
#include <stdio.h>
#include <errno.h>

Matrix new_matrix(int columns,int rows)
{
    int n = columns*rows;
    Matrix M = {columns, rows};
    //Matrix *p = &M;
    for (int i = 0; i < n; ++i)
    {
        M.m[i] = 0;
    }

    return M;
}

Matrix matrix_print(Matrix M)
{
    int c = M.columns;
    int r = M.rows;
    int n = 0;
    for (int i = 0; i < r; ++i)
    {
        printf("[");
        for (int j = 0; j < c-1; ++j)
        {
            printf("%f;",M.m[n]);
            ++n;
        }
        printf("%f",M.m[n]);
        printf("]\n");
    }

}

Matrix matrix_add(Matrix a,Matrix b)
{
    if(a.rows == b.rows && a.columns == b.columns)
    {
        Matrix M;
        Matrix *p = &M;
        int n = a.rows*a.columns;
        for (int i = 0; i < n; ++i)
        {
            p->m[i] = a.m[i] + b.m[i];
        }

        return M;
    }
    else
    {
        perror(" Wrong Matrix dimensions ");
    }

}

Matrix matrix_mult(Matrix a,Matrix b)
{
    if(a.columns == b.rows)
    {
        Matrix M;
        Matrix *p = &M;
        int c = M.columns;
        int r = M.rows;
        int n = 0;
        for (int i = 0; i < r; ++i)
        {
            for (int j = 0; j < c; ++j)
            {
                //p->m[i] = a.m[i] + b.m[i];

            }

        }

        return M;
    }
    else
    {
        perror(" Wrong Matrix dimensions ");
    }

}

Matrix matrix_scal_mult(Matrix a,Matrix b)
{
    if(a.rows == b.rows && a.columns == b.columns)
    {
        Matrix M;
        Matrix *p = &M;
        int n = a.rows*a.columns;
        for (int i = 0; i < n; ++i)
        {
            p->m[i] = a.m[i] * b.m[i];
        }

        return M;
    }
    else
    {
        perror(" Wrong Matrix dimensions ");
    }

}


