#ifndef MATRIX_H
#define MATRIX_H

#include <string>
#include <ostream>

template<typename T>
class Vector;

/**
 * @brief General dense matrix class
 */
class Matrix
{
public:
    int rows, cols;        // number of rows and columns
    double** values;       // matrix values

    // Constructors
    Matrix();                                      // default constructor
    Matrix(int r, int c);                           // size constructor, initialized to 0
    Matrix(int r, int c, double value);             // size + constant value
    Matrix(const Matrix& M);                        // copy constructor
    Matrix(int n, const std::string& id);           // identity matrix constructor

    // Destructor
    ~Matrix();

    // Assignment
    Matrix& operator=(const Matrix& M);

    // Element access
    double& operator()(int i, int j) const;

    // Matrix arithmetic
    Matrix operator+(const Matrix& M) const;
    Matrix operator-(const Matrix& M) const;
    Matrix operator*(const Matrix& M) const;
    Vector<double> operator*(const Vector<double>& v) const;

    // Scalar arithmetic
    Matrix operator*(double scalar) const;
    Matrix operator/(double scalar) const;

    Matrix& operator*=(double scalar);
    Matrix& operator/=(double scalar);

    Matrix& operator+=(const Matrix& M);
    Matrix& operator-=(const Matrix& M);

    // Norm
    double norm() const;

    // Display
    void print() const;
    friend std::ostream& operator<<(std::ostream&, const Matrix& M);
};

// Utility functions
Matrix transpose(const Matrix& M);
Vector<double> toVector(const Matrix& M);

/**
 * @brief Diagonal square matrix class
 */
class DiagonalMatrix
{
public:
    int size;          // size of the square matrix
    double* values;    // diagonal values only

    DiagonalMatrix();                          // default constructor
    DiagonalMatrix(int n, double value);       // size + constant diagonal value

    ~DiagonalMatrix();

    DiagonalMatrix& operator=(const DiagonalMatrix& M);
    DiagonalMatrix operator*(double scalar) const;
    Vector<double> operator*(const Vector<double>& v) const;

    friend std::ostream& operator<<(std::ostream&, const DiagonalMatrix& M);
};

#endif // MATRIX_H
