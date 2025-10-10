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

        /// @brief Read a .mesh file (Medit format)
        /// @param filename filename
        /// @param verts vertices coordinates
        /// @param polys vertices indices of each poly(tet/hex)
        /// @param vert_labels labels for each vertex, if no labels or all the same, this vector will be cleared
        /// @param poly_labels labels for each poly, if no labels or all the same, this vector will be cleared
        void ReadMESH(const std::string &filename,
                      std::vector<std::vector<double>> &verts,
                      std::vector<std::vector<unsigned int>> &polys,
                      std::vector<int> &vert_labels,
                      std::vector<int> &poly_labels);

        void ReadOVM(const std::string &filename,
                     std::vector<std::vector<double>> &verts,
                     std::vector<std::vector<unsigned int>> &polys,
                     std::vector<int> &vert_labels,
                     std::vector<int> &poly_labels);

        void ReadQHEX(const std::string &filename,
                      std::vector<std::vector<double>> &verts,
                      std::vector<std::vector<unsigned int>> &hexs);
    };
} // namespace mesh_processing