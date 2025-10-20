#pragma once
#include <vector>
namespace mesh_processing
{
    bool WriteOBJ(const char* filename,std::vector<std::vector<double>> vertices, std::vector<std::vector<size_t>> face_connectivity);
} // namespace mesh_processing
