#include "../include/globals.hpp"

// Importance factor for the integrability constraint
const int lambda_internal = 10;

// Importance factor for solution smoothness
const int lambda_csmo = 10;

// Discretization step size
// (square root of the importance factor for the deviation
//  between the input image and the projected solution)
const double step_size = 1.0;
