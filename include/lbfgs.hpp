#ifndef LBFGS_H
#define LBFGS_H

#include "matrix.hpp"
#include <string>

Vector<double> computeGradient(
    const Vector<double>& x,
    const Matrix& image
);

double objectiveFunction(
    const Vector<double>& x,
    const Matrix& image
);

Vector<double> LBFGS(
    Vector<double>& x,
    double (*objective)(const Vector<double>&, const Matrix&),
    Vector<double> (*objectiveGradient)(const Vector<double>&, const Matrix&),
    const Matrix& image,
    double epsilon
);

Vector<double> heightGradient(
    const Vector<double>& x,
    const Matrix& height
);

double heightObjective(
    const Vector<double>& x,
    const Matrix& height
);

void matrixToMesh(
    const std::string& filename,
    const Matrix& M
);

#endif // LBFGS_H
