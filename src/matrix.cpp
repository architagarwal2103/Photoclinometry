#include "../include/matrix.hpp"
#include "../include/vector.hpp"

#include <cstdlib>
#include <iostream>
#include <string>
#include <cmath>

// ====================== Matrix class ======================

// Constructors
Matrix::Matrix()
{
    rows = 0;
    cols = 0;
    values = nullptr;
}

Matrix::Matrix(int r, int c)
{
    rows = r;
    cols = c;
    values = new double*[rows];

    for (int i = 0; i < rows; i++)
    {
        values[i] = new double[cols];
        for (int j = 0; j < cols; j++)
            values[i][j] = 0.0;
    }
}

Matrix::Matrix(int r, int c, double value)
{
    rows = r;
    cols = c;
    values = new double*[rows];

    for (int i = 0; i < rows; i++)
    {
        values[i] = new double[cols];
        for (int j = 0; j < cols; j++)
            values[i][j] = value;
    }
}

Matrix::Matrix(const Matrix& M)
{
    rows = M.rows;
    cols = M.cols;

    values = new double*[rows];
    for (int i = 0; i < rows; i++)
    {
        values[i] = new double[cols];
        for (int j = 0; j < cols; j++)
            values[i][j] = M.values[i][j];
    }
}

// Identity matrix constructor
Matrix::Matrix(int dim, const std::string& id)
{
    if (id != "Id")
    {
        std::cerr << "Invalid identifier for identity matrix.\n";
        std::exit(1);
    }

    rows = cols = dim;
    values = new double*[dim];

    for (int i = 0; i < dim; i++)
    {
        values[i] = new double[dim];
        for (int j = 0; j < dim; j++)
            values[i][j] = (i == j) ? 1.0 : 0.0;
    }
}

// Destructor
Matrix::~Matrix()
{
    if (values)
    {
        for (int i = 0; i < rows; i++)
            delete[] values[i];
        delete[] values;
    }
    rows = cols = 0;
}

// Assignment
Matrix& Matrix::operator=(const Matrix& M)
{
    if (this == &M)
        return *this;

    if (rows != M.rows || cols != M.cols)
    {
        for (int i = 0; i < rows; i++)
            delete[] values[i];
        delete[] values;

        rows = M.rows;
        cols = M.cols;
        values = new double*[rows];
    }

    for (int i = 0; i < rows; i++)
    {
        values[i] = new double[cols];
        for (int j = 0; j < cols; j++)
            values[i][j] = M.values[i][j];
    }

    return *this;
}

// Element access (1-based indexing)
double& Matrix::operator()(int i, int j) const
{
    if (i > rows || j > cols)
    {
        std::cerr << "Error: index out of range.\n";
        std::exit(1);
    }
    return values[i - 1][j - 1];
}

// Matrix addition
Matrix Matrix::operator+(const Matrix& M) const
{
    Matrix tmp(rows, cols);
    for (int i = 0; i < rows; i++)
        for (int j = 0; j < cols; j++)
            tmp.values[i][j] = values[i][j] + M.values[i][j];

    return tmp;
}

// Matrix subtraction
Matrix Matrix::operator-(const Matrix& M) const
{
    Matrix tmp(rows, cols);
    for (int i = 0; i < rows; i++)
        for (int j = 0; j < cols; j++)
            tmp.values[i][j] = values[i][j] - M.values[i][j];

    return tmp;
}

// Matrix multiplication
Matrix Matrix::operator*(const Matrix& M) const
{
    if (cols != M.rows)
    {
        std::cerr << "Error: incompatible matrix sizes.\n";
        std::exit(1);
    }

    Matrix tmp(rows, M.cols, 0.0);

    for (int i = 0; i < rows; i++)
        for (int j = 0; j < M.cols; j++)
            for (int k = 0; k < cols; k++)
                tmp.values[i][j] += values[i][k] * M.values[k][j];

    return tmp;
}

// Matrix-vector multiplication
Vector<double> Matrix::operator*(const Vector<double>& V) const
{
    if (cols != V.dimension)
    {
        std::cerr << "Error: incompatible dimensions.\n";
        std::exit(1);
    }

    Vector<double> result(rows);

    for (int i = 0; i < rows; i++)
        for (int j = 0; j < cols; j++)
            result.values[i] += values[i][j] * V.values[j];

    return result;
}

// Scalar operations
Matrix Matrix::operator*(double scalar) const
{
    Matrix tmp(rows, cols);
    for (int i = 0; i < rows; i++)
        for (int j = 0; j < cols; j++)
            tmp.values[i][j] = values[i][j] * scalar;

    return tmp;
}

Matrix Matrix::operator/(double scalar) const
{
    if (!scalar)
    {
        std::cerr << "Error: division by zero.\n";
        std::exit(1);
    }

    Matrix tmp(rows, cols);
    for (int i = 0; i < rows; i++)
        for (int j = 0; j < cols; j++)
            tmp.values[i][j] = values[i][j] / scalar;

    return tmp;
}

Matrix& Matrix::operator*=(double scalar)
{
    for (int i = 0; i < rows; i++)
        for (int j = 0; j < cols; j++)
            values[i][j] *= scalar;

    return *this;
}

Matrix& Matrix::operator/=(double scalar)
{
    if (!scalar)
    {
        std::cerr << "Error: division by zero.\n";
        std::exit(1);
    }

    for (int i = 0; i < rows; i++)
        for (int j = 0; j < cols; j++)
            values[i][j] /= scalar;

    return *this;
}

Matrix& Matrix::operator+=(const Matrix& M)
{
    for (int i = 0; i < rows; i++)
        for (int j = 0; j < cols; j++)
            values[i][j] += M.values[i][j];

    return *this;
}

Matrix& Matrix::operator-=(const Matrix& M)
{
    for (int i = 0; i < rows; i++)
        for (int j = 0; j < cols; j++)
            values[i][j] -= M.values[i][j];

    return *this;
}

// Print
void Matrix::print() const
{
    for (int i = 0; i < rows; i++)
    {
        for (int j = 0; j < cols; j++)
            std::cout << values[i][j] << " ";
        std::cout << "\n";
    }
}

std::ostream& operator<<(std::ostream& out, const Matrix& M)
{
    if (!M.values)
    {
        std::cerr << "Matrix contains no data.\n";
        std::exit(1);
    }

    for (int i = 0; i < M.rows; i++)
    {
        for (int j = 0; j < M.cols; j++)
            out << M.values[i][j] << " ";
        out << "\n";
    }

    return out;
}

// Utilities
Vector<double> toVector(const Matrix& M)
{
    Vector<double> V(M.rows * M.cols);
    for (int i = 0; i < M.rows; i++)
        for (int j = 0; j < M.cols; j++)
            V.values[i * M.cols + j] = M.values[i][j];

    return V;
}

Matrix transpose(const Matrix& M)
{
    Matrix T(M.cols, M.rows);
    for (int i = 0; i < M.rows; i++)
        for (int j = 0; j < M.cols; j++)
            T.values[j][i] = M.values[i][j];

    return T;
}

double Matrix::norm() const
{
    double sum = 0.0;
    for (int i = 0; i < rows; i++)
        for (int j = 0; j < cols; j++)
            sum += values[i][j] * values[i][j];

    return std::sqrt(sum);
}

// ====================== DiagonalMatrix ======================

DiagonalMatrix::DiagonalMatrix()
{
    size = 0;
    values = nullptr;
}

DiagonalMatrix::DiagonalMatrix(int n, double value)
{
    size = n;
    values = new double[n];
    for (int i = 0; i < n; i++)
        values[i] = value;
}

DiagonalMatrix::~DiagonalMatrix()
{
    delete[] values;
    size = 0;
}

DiagonalMatrix& DiagonalMatrix::operator=(const DiagonalMatrix& M)
{
    if (this == &M)
        return *this;

    if (size != M.size)
    {
        delete[] values;
        size = M.size;
        values = new double[size];
    }

    for (int i = 0; i < size; i++)
        values[i] = M.values[i];

    return *this;
}

DiagonalMatrix DiagonalMatrix::operator*(double scalar) const
{
    DiagonalMatrix tmp(size, 0.0);
    for (int i = 0; i < size; i++)
        tmp.values[i] = values[i] * scalar;

    return tmp;
}

Vector<double> DiagonalMatrix::operator*(const Vector<double>& V) const
{
    if (size != V.dimension)
    {
        std::cerr << "Error: incompatible dimensions.\n";
        std::exit(1);
    }

    Vector<double> result(size);
    for (int i = 0; i < size; i++)
        result.values[i] += values[i] * V.values[i];

    return result;
}

std::ostream& operator<<(std::ostream& out, const DiagonalMatrix& M)
{
    if (!M.values)
    {
        std::cerr << "Diagonal matrix contains no data.\n";
        std::exit(1);
    }

    for (int i = 0; i < M.size; i++)
    {
        for (int j = 0; j < M.size; j++)
            out << (i == j ? M.values[i] : 0.0) << " ";
        out << "\n";
    }

    return out;
}
