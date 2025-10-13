#pragma once
#include <string>
#include <vector>
namespace mesh_processing
{
    bool ReadOVM(const std::string &filename,
                     std::vector<std::vector<double>> &verts,
                     std::vector<std::vector<size_t>> &polys);
}