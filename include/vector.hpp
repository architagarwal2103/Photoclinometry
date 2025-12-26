#ifndef VECTOR_H
#define VECTOR_H

#include <iostream>
#include <cmath>
#include "./matrix.hpp"

template <typename T>
class Vector
{
public:
    int dimension;
    T* values;

    // Constructors
    Vector();
    Vector(int d, T value = T(0));
    Vector(const Vector& V);

    // Destructor
    ~Vector();

    // Internal operators
    Vector<T>& operator=(const Vector<T>&);
    Vector<T>& operator*=(const T);
    Vector<T>& operator/=(const T);

    Vector<T> operator^(const Vector<T>&);          // cross product (3D)
    Vector<T> operator+(const Vector<T>&);
    Vector<T> operator-(const Vector<T>&);
    Vector<T> operator*(const T);

    Vector<T> concatenate(const Vector<T>&);
    Matrix toMatrix(int rows, int cols) const;

    T operator*(const Vector<T>&);                   // dot product
    T& operator()(int) const;                        // 1-based indexing
    Vector<T> operator()(int, int) const;            // subvector

    // Output
    template <typename U>
    friend std::ostream& operator<<(std::ostream&, const Vector<U>&);

    // Norm
    double norm() const;
};

/* ===================== IMPLEMENTATION ===================== */

// Constructors
template <typename T>
Vector<T>::Vector() : dimension(0), values(nullptr) {}

template <typename T>
Vector<T>::Vector(int d, T v) : dimension(d)
{
    values = new T[dimension];
    for (int i = 0; i < dimension; i++)
        values[i] = v;
}

template <typename T>
Vector<T>::Vector(const Vector<T>& V) : dimension(V.dimension)
{
    if (dimension <= 0)
    {
        values = nullptr;
        return;
    }
    values = new T[dimension];
    for (int i = 0; i < dimension; i++)
        values[i] = V.values[i];
}

// Destructor
template <typename T>
Vector<T>::~Vector()
{
    delete[] values;
}

// Assignment
template <typename T>
Vector<T>& Vector<T>::operator=(const Vector<T>& V)
{
    if (this == &V)
        return *this;

    dimension = V.dimension;
    delete[] values;

    values = new T[dimension];
    for (int i = 0; i < dimension; i++)
        values[i] = V.values[i];

    return *this;
}

// Scalar operations
template <typename T>
Vector<T>& Vector<T>::operator*=(const T f)
{
    for (int i = 0; i < dimension; i++)
        values[i] *= f;
    return *this;
}

template <typename T>
Vector<T>& Vector<T>::operator/=(const T f)
{
    if (f == 0)
    {
        std::cerr << "Error: division by zero\n";
        std::exit(1);
    }
    for (int i = 0; i < dimension; i++)
        values[i] /= f;
    return *this;
}

// Element access (1-based indexing)
template <typename T>
T& Vector<T>::operator()(int i) const
{
    return values[i - 1];
}

// Subvector [i, j]
template <typename T>
Vector<T> Vector<T>::operator()(int i, int j) const
{
    if (i >= j)
    {
        std::cerr << "Error: i must be strictly less than j\n";
        std::exit(1);
    }

    int size = j - i + 1;
    Vector<T> result(size);

    for (int k = 0; k < size; k++)
        result.values[k] = values[i + k];

    return result;
}

// Vector addition
template <typename T>
Vector<T> Vector<T>::operator+(const Vector<T>& A)
{
    if (A.dimension != dimension)
    {
        std::cerr << "Error: vectors do not have the same dimension\n";
        std::exit(1);
    }

    Vector<T> C = *this;
    for (int i = 0; i < dimension; i++)
        C.values[i] += A.values[i];

    return C;
}

// Vector subtraction
template <typename T>
Vector<T> Vector<T>::operator-(const Vector<T>& A)
{
    if (A.dimension != dimension)
    {
        std::cerr << "Error: vectors do not have the same dimension\n";
        std::exit(1);
    }

    Vector<T> C = *this;
    for (int i = 0; i < dimension; i++)
        C.values[i] -= A.values[i];

    return C;
}

// Scalar multiplication
template <typename T>
Vector<T> Vector<T>::operator*(const T a)
{
    Vector<T> V = *this;
    for (int i = 0; i < dimension; i++)
        V.values[i] *= a;
    return V;
}

// Concatenation
template <typename T>
Vector<T> Vector<T>::concatenate(const Vector<T>& A)
{
    Vector<T> C(dimension + A.dimension);

    for (int i = 0; i < dimension; i++)
        C.values[i] = values[i];

    for (int j = 0; j < A.dimension; j++)
        C.values[j + dimension] = A.values[j];

    return C;
}

// Convert to matrix
template <typename T>
Matrix Vector<T>::toMatrix(int rows, int cols) const
{
    if (rows * cols != dimension)
    {
        std::cerr << "Error: incompatible dimensions\n";
        std::exit(1);
    }

    Matrix M(rows, cols);
    for (int r = 0; r < rows; r++)
        for (int c = 0; c < cols; c++)
            M.values[r][c] = values[c + r * cols];

    return M;
}

// Cross product (3D only)
template <typename T>
Vector<T> Vector<T>::operator^(const Vector<T>& V)
{
    if (dimension != 3 || V.dimension != 3)
    {
        std::cerr << "Error: cross product is defined only for 3D vectors\n";
        std::exit(1);
    }

    Vector<T> result(3);
    result(1) = values[1] * V(3) - values[2] * V(2);
    result(2) = values[2] * V(1) - values[0] * V(3);
    result(3) = values[0] * V(2) - values[1] * V(1);

    return result;
}

// Dot product
template <typename T>
T Vector<T>::operator*(const Vector<T>& V)
{
    T result = T(0);
    for (int i = 0; i < dimension; i++)
        result += values[i] * V.values[i];
    return result;
}

// Output
template <typename T>
std::ostream& operator<<(std::ostream& out, const Vector<T>& V)
{
    out << "( ";
    for (int i = 0; i < V.dimension; i++)
        out << V.values[i] << " ";
    out << ")";
    return out;
}

// Norm
template <typename T>
double Vector<T>::norm() const
{
    return std::sqrt((*this) * (*this));
}

#endif // VECTOR_H
