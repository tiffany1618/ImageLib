//
// Created by tiffany on 9/21/20.
//

#ifndef PHOTOEDITOR_MATRIX_H
#define PHOTOEDITOR_MATRIX_H

template<typename T>
class Matrix {
public:
    Matrix(int rows, int cols);
    Matrix(int rows, int cols, T **mat);
    Matrix(const Matrix &m);
    ~Matrix();
    Matrix<T>& operator=(const Matrix &m);
    Matrix<T>& operator=(T **mat);

    int get_rows() const;
    int get_cols() const;
    T** get_mat() const;

    Matrix<T> operator*(const Matrix<T> &m) const;
    Matrix<T> operator*(T scalar) const;
    Matrix<T> operator+(const Matrix<T> &m) const;
    Matrix<T> operator-(const Matrix<T> &m) const;

    void zero();
    void print();

private:
    int rows;
    int cols;
    T **mat;

    void init_mat();
    void copy_mat(T **m);
    void delete_mat();
};

#include "Matrix.cpp"

#endif //PHOTOEDITOR_MATRIX_H
