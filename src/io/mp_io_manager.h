#pragma once
#include <string>
#include <vector>
#include <array>
#include <Eigen/Core>

namespace mesh_processing
{
    enum class MeshFileType
    {
        OBJ,
        OFF,
        PLY,
        MESH,
        OVM,
        UNKNOWN
    };

    class IOManager
    {
    public:
        IOManager() = default;
        ~IOManager() = default;

    public:
        void LoadPolygonSoupOBJ(
            const std::string &filename,
            std::vector<std::array<double, 3>> &vertexPositionsOut,
            std::vector<std::vector<size_t>> &faceIndicesOut);

        // Load a mesh from a general file, detecting type from filename
        void LoadPolygonSoup(std::string filename,
                             std::vector<std::array<double, 3>> &vertexPositionsOut,
                             std::vector<std::vector<size_t>> &faceIndicesOut);


    };
} // namespace mesh_processing