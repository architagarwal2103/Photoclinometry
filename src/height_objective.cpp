#include "../include/vector.hpp"
#include "../include/matrix.hpp"
#include "../include/globals.hpp"
#include <cmath>

// Definition of the height objective function
double heightObjective(const Vector<double>& h, const Matrix& x)
{
    int num_rows = x.rows / 2;
    int num_cols = x.cols;

    Matrix height = h.toMatrix(num_rows, num_cols);
    double value = 0.0;

    for (int i = 1; i < num_rows; i++)
    {
        for (int j = 1; j < num_cols; j++)
        {
            value +=
                std::pow(height(i + 1, j) - height(i, j) - step_size * x(i, j), 2)
              + std::pow(height(i, j + 1) - height(i, j) - step_size * x(i + num_rows, j), 2);
        }
    }

    return value;
}
