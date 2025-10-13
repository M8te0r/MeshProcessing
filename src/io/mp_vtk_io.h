#pragma once
#include <string>
#include <vector>
namespace mesh_processing
{
    
    bool ReadVTK(const std::string &filename,
                 std::vector<std::vector<double>> &vertices,
                 std::vector<std::vector<size_t>> &cells,
                 std::vector<int> &cell_types);


    bool WriteVTK(const std::string &filename,
                  const std::vector<std::vector<double>> &vertices,
                  const std::vector<std::vector<size_t>> &cells,
                  const std::vector<int> &cell_types);
}