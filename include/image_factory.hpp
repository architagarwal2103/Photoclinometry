#ifndef IMAGE_FACTORY_H
#define IMAGE_FACTORY_H

#include "./matrix.hpp"
#include "./vector.hpp"

class ImageFactory
{
public:
    int num_vertices;
    int num_quadrilaterals;   // number of vertices / quadrilaterals in the mesh

    int image_width;
    int image_height;         // image dimensions (pixels)

    Matrix vertices;          // mesh vertices
    Matrix quadrilaterals;    // mesh quadrilaterals (vertex indices)
    Matrix image;             // grayscale image (values 0–255)
    Matrix height_derivatives;// directional derivatives of height at each image point

    explicit ImageFactory(const char* filename);     // constructor that reads a mesh file
    void flatten(Vecteur<double>& source);            // 3D mesh → 2D matrix
    void save2D(const char* filename);                 // save 2D matrix to file
};

Matrix csvToMatrix(const char* csv_file);              // convert CSV file to matrix

#endif // IMAGE_FACTORY_H
