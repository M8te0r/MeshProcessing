#pragma once
#include <vector>
namespace mesh_processing
{
    bool ReadQHEX(const char* filename,
                      std::vector<std::vector<double>> &verts,
                      std::vector<std::vector<size_t>> &hexs);
} // namespace mesh_processing
