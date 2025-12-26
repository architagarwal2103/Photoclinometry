#include "../include/lbfgs.hpp"
#include "../include/matrix.hpp"
#include "../include/vector.hpp"

#include <cmath>
#include <iostream>

// Implementation of the L-BFGS gradient descent algorithm
Vector<double> LBFGS(
    Vector<double>& x,
    double (*objective)(const Vector<double>&, const Matrix&),
    Vector<double> (*objectiveGradient)(const Vector<double>&, const Matrix&),
    const Matrix& M,
    double epsilon
)
{
    DiagonalMatrix identity(x.dimension, 1.0);

    double gamma = 1.0;      // scaling factor
    int memory = 5;          // number of stored iterations (m)
    int iteration = 0;       // iteration counter
    int wolfe_max = 20;      // max iterations for Wolfe line search

    Vector<Vector<double>> s(memory);
    Vector<Vector<double>> y(memory);
    Vector<double> alpha_coeff(memory, 0.0);

    double beta;
    double c1 = std::pow(10.0, -4);
    double c2 = 0.9999999;

    while (true)
    {
        std::cout << "Iteration: " << iteration << "\n";

        Vector<double> gradient = objectiveGradient(x, M);
        Vector<double> q = gradient;

        std::cout << "Gradient norm: " << gradient.norm() << "\n";

        if (gradient.norm() < epsilon || iteration == 10000)
            return x;

        if (iteration > 0)
        {
            gamma =
                (s.values[(iteration - 1) % memory] *
                 y.values[(iteration - 1) % memory]) /
                (y.values[(iteration - 1) % memory] *
                 y.values[(iteration - 1) % memory]);
        }

        // Initial Hessian inverse approximation
        DiagonalMatrix H0 = identity * gamma;

        // Two-loop recursion (descent direction computation)
        for (int i = std::max(iteration - 1, 0);
             i > std::max(iteration - memory - 1, 0);
             i--)
        {
            alpha_coeff.values[i % memory] =
                (1.0 / (y.values[i % memory] * s.values[i % memory])) *
                (s.values[i % memory] * q);

            q = q - y.values[i % memory] * alpha_coeff.values[i % memory];
        }

        Vector<double> r = H0 * q;

        for (int i = std::max(iteration - memory, 0);
             i < std::max(iteration, 0);
             i++)
        {
            beta =
                (1.0 / (y.values[i % memory] * s.values[i % memory])) *
                (y.values[i % memory] * r);

            r = r + s.values[i % memory] *
                    (alpha_coeff.values[i % memory] - beta);
        }

        Vector<double> descent_direction = r * -1.0;

        // Wolfe line search
        double step = 1.0;
        double f0 = objective(x, M);

        std::cout << "Objective value: " << f0 << "\n";

        double directional_derivative = gradient * descent_direction;
        int wolfe_iter = 0;

        while (true)
        {
            Vector<double> x_trial = x + descent_direction * step;
            double f_trial = objective(x_trial, M);
            Vector<double> g_trial = objectiveGradient(x_trial, M);

            if ((f_trial <= f0 + c1 * step * directional_derivative &&
                 std::abs(g_trial * descent_direction) <=
                     c2 * std::abs(directional_derivative)) ||
                wolfe_iter > wolfe_max)
            {
                break;
            }

            step *= 0.5;
            wolfe_iter++;
        }

        Vector<double> x_next = x + descent_direction * step;

        y.values[iteration % memory] =
            objectiveGradient(x_next, M) - objectiveGradient(x, M);

        s.values[iteration % memory] = x_next - x;

        x = x_next;
        iteration++;
    }
}
