// Jacobian (gradient) computation of the objective function

#include "../include/matrix.hpp"
#include "../include/vector.hpp"
#include "../include/globals.hpp"
#include <cmath>

// Definition of the gradient of the objective function to be minimized
Vector<double> computeGradient(const Vector<double>& x, const Matrix& image)
{
    Matrix p = x(0, image.rows * image.cols - 1)
                   .toMatrix(image.rows, image.cols);

    Matrix q = x(image.rows * image.cols,
                 2 * image.rows * image.cols - 1)
                   .toMatrix(image.rows, image.cols);

    Matrix G1(2 * image.rows, image.cols);
    Matrix G2(2 * image.rows, image.cols);
    Matrix G3(2 * image.rows, image.cols);

    // Gradient w.r.t. p
    for (int i = 1; i <= image.rows; i++)
    {
        for (int j = 1; j <= image.cols; j++)
        {
            G1(i, j) =
                -255.0 * p(i, j) *
                (255.0 - image(i, j) *
                std::sqrt(std::pow(q(i, j), 2) + std::pow(p(i, j), 2) + 1.0)) /
                std::pow(std::pow(q(i, j), 2) + std::pow(p(i, j), 2) + 1.0, 2);

            if (i != 1 && j != 1 && i != image.rows && j != image.cols)
            {
                G2(i, j) =
                    2 * p(i, j) - p(i, j - 1) - p(i, j + 1)
                    - q(i, j) + q(i + 1, j)
                    - q(i + 1, j - 1) + q(i, j - 1);

                G3(i, j) =
                    4 * p(i, j) - p(i - 1, j) - p(i + 1, j)
                    - p(i, j - 1) - p(i, j + 1);
            }
        }
    }

    // Gradient w.r.t. q
    for (int i = image.rows + 1; i <= 2 * image.rows; i++)
    {
        for (int j = 1; j <= image.cols; j++)
        {
            G1(i, j) =
                -255.0 * q(i - image.rows, j) *
                (255.0 - image(i - image.rows, j) *
                std::sqrt(std::pow(q(i - image.rows, j), 2)
                        + std::pow(p(i - image.rows, j), 2) + 1.0)) /
                std::pow(std::pow(q(i - image.rows, j), 2)
                       + std::pow(p(i - image.rows, j), 2) + 1.0, 2);

            if (i != image.rows + 1 && j != 1 &&
                i != 2 * image.rows && j != image.cols)
            {
                G2(i, j) =
                    2 * q(i - image.rows, j)
                    - q(i - 1 - image.rows, j)
                    - q(i + 1 - image.rows, j)
                    - p(i - image.rows, j)
                    + p(i - image.rows, j + 1)
                    - p(i - 1 - image.rows, j + 1)
                    + p(i - 1 - image.rows, j);

                G3(i, j) =
                    4 * q(i - image.rows, j)
                    - q(i - 1 - image.rows, j)
                    - q(i + 1 - image.rows, j)
                    - q(i - image.rows, j - 1)
                    - q(i - image.rows, j + 1);
            }
        }
    }

    G1 *= step_size * step_size;
    G2 *= lambda_internal;
    G3 *= lambda_csmo;

    return toVector((G1 + G2 + G3) * 2);
}
