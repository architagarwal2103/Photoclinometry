#include "../include/matrix.hpp"
#include "../include/vector.hpp"
#include "../include/image_factory.hpp"
#include "../include/lbfgs.hpp"

#include <cmath>
#include <ctime>
#include <iostream>

int main()
{
    /*
    // Mesh → 2D image
    ImageFactory mesh("maillages/dragon.mesh");

    Vector<double> light_source(3, 0.0); // light source direction
    light_source(3) = 1.0;

    mesh.flatten(light_source);
    Matrix image = mesh.image;

    mesh.save2D("images/dragon2.ppm");
    */

    // 2D image → mesh reconstruction

    Matrix image = csvToMatrix("images/dragon.csv");
    const clock_t begin_time = clock(); // start timer

    // First optimization: recover directional derivatives of height
    std::cout << "L-BFGS on objective function\n";

    Vector<double> x0(2 * image.rows * image.cols, 0.5);
    double grad_tol_1 = 100.0; // stopping condition on objective gradient

    Vector<double> x = LBFGS(
        x0,
        objectiveFunction,
        computeGradient,
        image,
        grad_tol_1
    );

    Matrix height_derivatives = x.toMatrix(2 * image.rows, image.cols);

    // Second optimization: compute height at each pixel
    std::cout << "L-BFGS on height\n";

    Vector<double> h0(image.rows * image.cols, 0.0);
    double grad_tol_2 = std::pow(10.0, -3); // stopping condition on height gradient

    Vector<double> y = LBFGS(
        h0,
        heightObjective,
        heightGradient,
        height_derivatives,
        grad_tol_2
    );

    Matrix reconstructed = y.toMatrix(image.rows, image.cols);

    // Save reconstructed mesh
    matrixToMesh("maillages/dragon.mesh", reconstructed);

    // Print execution time
    std::cout << "Execution time (seconds): "
              << float(clock() - begin_time) / CLOCKS_PER_SEC
              << "\n";

    return 0;
}
