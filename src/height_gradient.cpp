#include "../include/matrix.hpp"
#include "../include/vector.hpp"
#include "../include/globals.hpp"

// Definition of the height gradient
Vector<double> heightGradient(const Vector<double>& h, const Matrix& x)
{
    int num_rows = x.rows / 2;
    int num_cols = x.cols;

    Matrix height = h.toMatrix(num_rows, num_cols);
    Matrix gradient(num_rows, num_cols, 0.0);

    for (int i = 1; i <= num_rows; i++)
    {
        for (int j = 1; j <= num_cols; j++)
        {
            if (i != 1 && j != 1 && i != num_rows && j != num_cols)
            {
                gradient(i, j) =
                    4 * height(i, j)
                    - height(i - 1, j) - step_size * x(i - 1, j)
                    - height(i + 1, j) + step_size * x(i, j)
                    - height(i, j - 1) - step_size * x(i + num_rows, j - 1)
                    - height(i, j + 1) + step_size * x(i + num_rows, j);
            }
        }
    }

    return toVector(gradient * 2);
}
