#pragma once
#include <vector>
namespace mesh_processing
{
    bool ReadMESH(const char *filename,
                  std::vector<std::vector<double>> &verts,
                  std::vector<std::vector<unsigned int>> &polys,
                  std::vector<int> &vert_labels,
                  std::vector<int> &poly_labels);

    bool ReadMESH(const char *filename,
                  std::vector<std::vector<double>> &verts,
                  std::vector<std::vector<size_t>> &polys,
                  std::vector<int> &vert_labels,
                  std::vector<int> &poly_labels);
} // namespace mesh_processing
