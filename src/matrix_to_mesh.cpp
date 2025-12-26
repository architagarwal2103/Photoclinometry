#include "../include/matrix.hpp"

#include <iostream>
#include <fstream>
#include <string>

// Save height matrix to mesh file
void matrixToMesh(const std::string& filename, const Matrix& M)
{
    std::ofstream mesh(filename);

    mesh << "\n";
    mesh << "MeshVersionFormatted\n";
    mesh << "1\n\n";

    mesh << "Dimension\n";
    mesh << "3\n\n";

    mesh << "Vertices\n";
    mesh << M.rows * M.cols << "\n";

    for (int j = 0; j < M.cols; j++)
    {
        for (int i = 0; i < M.rows; i++)
        {
            mesh << i << " " << j << " " << M.values[i][j] << " 0\n";
        }
    }

    mesh << "\n";
    mesh << "Quadrilaterals\n";
    mesh << (M.cols - 1) * (M.rows - 1) << "\n";

    for (int j = 0; j < M.cols - 1; j++)
    {
        for (int i = 0; i < M.rows - 1; i++)
        {
            mesh
                << j * M.rows + i + 1 << " "
                << j * M.rows + i + 2 << " "
                << (j + 1) * M.rows + i + 2 << " "
                << (j + 1) * M.rows + i + 1 << " 0\n";
        }
    }
}
