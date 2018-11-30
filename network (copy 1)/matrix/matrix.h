#ifndef OCR_MATRIX_H
#define OCR_MATRIX_H

typedef struct Matrix Matrix;
struct Matrix
{
    int columns;                 //number of columns
    int rows;                    //number of rows

    double m[4];                 //list of elements

};

Matrix new_matrix(int columns,int rows);

Matrix matrix_print(Matrix M);

Matrix matrix_add(Matrix a,Matrix b);

Matrix matrix_mult(Matrix a,Matrix b);

Matrix matrix_scal_mult(Matrix a,Matrix b);

#endif //OCR_MATRIX_H
