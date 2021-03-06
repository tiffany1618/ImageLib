#ifndef PHOTOEDITOR_MATRIX_H
#define PHOTOEDITOR_MATRIX_H

#include <iostream>
#include <iomanip>

template <typename T>
class Matrix {
public:
    Matrix(int rows, int cols);
    Matrix(int rows, int cols, T **mat);
    Matrix(const Matrix &m);
    ~Matrix();
    Matrix<T>& operator=(const Matrix<T> &m);
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

template<typename T>
Matrix<T>::Matrix(int rows, int cols) : rows(rows), cols(cols) {
    init_mat();
}

template<typename T>
Matrix<T>::Matrix(int rows, int cols, T **mat) : rows(rows), cols(cols) {
    init_mat();
    copy_mat(mat);
}

template<typename T>
Matrix<T>::Matrix(const Matrix &m) : rows(m.rows), cols(m.cols) {
    init_mat();
    copy_mat(m.mat);
}

template<typename T>
Matrix<T>::~Matrix() {
    delete_mat();
}

template<typename T>
Matrix<T>& Matrix<T>::operator=(const Matrix<T> &m) {
    if (this != &m) {
        if ((this->rows != m.rows) || (this->cols != m.cols)) {
            delete_mat();
            this->rows = m.rows;
            this->cols = m.cols;
            init_mat();
        }

        copy_mat(m.mat);
    }

    return *this;
}

template<typename T>
Matrix<T>& Matrix<T>::operator=(T **mat) {
    if (this->mat != mat) {
        int c = sizeof(mat[0]);
        int r = sizeof(mat) / c;
        if ((this->rows != r) || (this->cols != c)) {
            delete_mat();
            this->rows = r;
            this->cols = c;
            init_mat();
        }

        copy_mat(mat);
    }

    return *this;
}

template<typename T>
int Matrix<T>::get_rows() const {
    return rows;
}

template<typename T>
int Matrix<T>::get_cols() const {
    return cols;
}

template<typename T>
T** Matrix<T>::get_mat() const {
    return mat;
}

template<typename T>
Matrix<T> Matrix<T>::operator*(const Matrix<T> &m) const {
    if (this->cols != m.get_rows()) {
        // TODO: throw error here
    }

    Matrix<T> output(this->rows, m.cols);
    output.zero();

    for (int i = 0; i < output.rows; i++) {
        for (int j = 0; j < output.cols; j++) {
            for (int k = 0; k < this->cols; k++)  {
                output.mat[i][j] += this->mat[i][k] * m.mat[k][j];
            }
        }
    }

    return output;
}

template<typename T>
Matrix<T> Matrix<T>::operator*(T scalar) const {
    Matrix<T> output(this->rows, this->cols);

    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            output.mat[i][j] = this->mat[i][j] * scalar;
        }
    }

    return output;
}

template<typename T>
Matrix<T> Matrix<T>::operator+(const Matrix<T> &m) const {
    if ((this->rows != m.rows) || (this->cols != m.cols)) {
        // TODO: throw error here
    }

    Matrix<T> output(this->rows, this->cols);

    for (int i = 0; i < output.rows; i++) {
        for (int j = 0; j < output.cols; j++) {
            output.data[i][j] = this->data[i][j] + m.data[i][j];
        }
    }

    return output;
}

template<typename T>
Matrix<T> Matrix<T>::operator-(const Matrix<T> &m) const {
    if ((this->rows != m.rows) || (this->cols != m.cols)) {
        // TODO: throw error here
    }

    Matrix<T> output(this->rows, this->cols);

    for (int i = 0; i < output.rows; i++) {
        for (int j = 0; j < output.cols; j++) {
            output.data[i][j] = this->data[i][j] - m.data[i][j];
        }
    }

    return output;
}

template<typename T>
void Matrix<T>::zero() {
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            mat[i][j] = 0;
        }
    }
}

template<typename T>
void Matrix<T>::print() {
    std::cout << std::fixed << std::setprecision(5);
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            std::cout << mat[i][j] << ",\t";
        }
        std::cout << std::endl;
    }
}

template<typename T>
void Matrix<T>::init_mat() {
    mat = new T*[rows];
    for (int i = 0; i < rows; i++) {
        mat[i] = new T[cols];
    }
}

template<typename T>
void Matrix<T>::copy_mat(T **m) {
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            this->mat[i][j] = m[i][j];
        }
    }
}

template<typename T>
void Matrix<T>::delete_mat() {
    for (int i = 0; i < rows; i++) {
        delete[] mat[i];
    }
}

#endif //PHOTOEDITOR_MATRIX_H
