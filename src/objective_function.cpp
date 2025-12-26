#include "../include/matrix.hpp"
#include "../include/vector.hpp"
#include "../include/globals.hpp"
#include <cmath>

// Definition of the objective function to be minimized
double objectiveFunction(const Vector<double>& x, const Matrix& image)
{
    Matrix p = x(0, image.rows * image.cols - 1)
                   .toMatrix(image.rows, image.cols);

    Matrix q = x(image.rows * image.cols,
                 2 * image.rows * image.cols - 1)
                   .toMatrix(image.rows, image.cols);

    double data_term = 0.0;
    double integrability_term = 0.0;
    double smoothness_term = 0.0;

    for (int i = 1; i <= image.rows; i++)
    {
        for (int j = 1; j <= image.cols; j++)
        {
            data_term += std::pow(
                image(i, j) -
                (255.0 / std::sqrt(1 + std::pow(p(i, j), 2) + std::pow(q(i, j), 2))),
                2
            );

            if (i != image.rows && j != image.cols)
            {
                integrability_term += std::pow(
                    p(i, j + 1) - p(i, j) - q(i + 1, j) + q(i, j),
                    2
                );

                smoothness_term +=
                    std::pow(p(i + 1, j) - p(i, j), 2) +
                    std::pow(p(i, j + 1) - p(i, j), 2) +
                    std::pow(q(i, j + 1) - q(i, j), 2) +
                    std::pow(q(i + 1, j) - q(i, j), 2);
            }
        }
    }

    data_term *= step_size * step_size;
    integrability_term *= lambda_internal;
    smoothness_term *= lambda_csmo;

    return data_term + integrability_term + smoothness_term;
}
