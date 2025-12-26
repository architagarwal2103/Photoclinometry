#include "../include/image_factory.hpp"
#include "../include/matrix.hpp"
#include "../include/vector.hpp"

#include <string>
#include <cmath>
#include <iostream>
#include <fstream>

// Constructor: reads a mesh file
ImageFactory::ImageFactory(const char* filename)
{
    std::ifstream file(filename, std::ios::in);

    if (!file)
    {
        std::cerr << "Unable to open mesh file!" << std::endl;
        return;
    }

    // Skip header lines
    for (int i = 0; i < 8; i++)
        file.ignore(50, '\n');

    file >> num_vertices;
    Matrix V(num_vertices, 3);

    for (int i = 1; i <= num_vertices; i++)
    {
        file >> V(i, 1);
        file >> V(i, 2);
        file >> V(i, 3);
        file.ignore(50, '\n');
    }

    file.ignore(50, '\n');
    file.ignore(50, '\n');

    file >> num_quadrilaterals;
    Matrix Q(num_quadrilaterals, 4);

    for (int i = 1; i <= num_quadrilaterals; i++)
    {
        file >> Q(i, 1);
        file >> Q(i, 2);
        file >> Q(i, 3);
        file >> Q(i, 4);
        file.ignore(50, '\n');
    }

    file.close();

    vertices = V;
    quadrilaterals = Q;

    image_width  = static_cast<int>(vertices(num_vertices, 1));
    image_height = static_cast<int>(vertices(num_vertices, 2));
}

// Convert 3D mesh to 2D image
void ImageFactory::flatten(Vector<double>& source)
{
    Matrix img(image_height, image_width);

    int s1, s2, s3;
    Vector<double> v1(3), v2(3), v3(3);

    Matrix height_derivatives_local(2 * image_height, image_width);

    // Iterate over mesh quadrilaterals
    for (int i = 0; i < num_quadrilaterals; i++)
    {
        s1 = static_cast<int>(quadrilaterals(i + 1, 1));
        s2 = static_cast<int>(quadrilaterals(i + 1, 2));
        s3 = static_cast<int>(quadrilaterals(i + 1, 3));

        // Two edges of the quadrilateral
        v1(1) = vertices(s1, 1) - vertices(s2, 1);
        v1(2) = vertices(s1, 2) - vertices(s2, 2);
        v1(3) = vertices(s1, 3) - vertices(s2, 3);

        v2(1) = vertices(s3, 1) - vertices(s2, 1);
        v2(2) = vertices(s3, 2) - vertices(s2, 2);
        v2(3) = vertices(s3, 3) - vertices(s2, 3);

        // Mesh normal: cross product of two edges
        v3 = v1 ^ v2;
        v3 /= v3.norm();

        // Grayscale intensity (assumed constant per quadrilateral)
        img.values[i / image_height][i % image_width] =
            std::abs(255.0 * (source * v3));

        // Directional derivatives of height
        height_derivatives_local.values[i / image_width][i % image_width] =
            -v3(1) / v3(3);

        height_derivatives_local.values[(i / image_width) + image_height][i % image_width] =
            -v3(2) / v3(3);
    }

    height_derivatives = height_derivatives_local;
    image = img;
}

// Save 2D grayscale image to PPM file
void ImageFactory::save2D(const char* filename)
{
    std::ofstream img(filename);

    img << "P3\n";
    img << image_width << " " << image_height << "\n";
    img << "255\n";

    for (int i = 0; i < image_height; i++)
    {
        for (int j = 0; j < image_width; j++)
        {
            int grey = static_cast<int>(image.values[i][j]);
            img << grey << " " << grey << " " << grey << "\n";
        }
    }

    img.close();
}

// Read CSV file into a matrix
Matrix csvToMatrix(const char* csv_file)
{
    std::ifstream csv(csv_file, std::ios::in);

    int rows, cols;
    csv >> rows >> cols;

    Matrix M(rows, cols);

    for (int i = 0; i < rows; i++)
        for (int j = 0; j < cols; j++)
            csv >> M.values[i][j];

    return M;
}
