#pragma once
#include "mp_mesh_base.h"
#include <string>
#include <Eigen/Core>

namespace mesh_processing
{
    class HexahedralMesh : public VolumeMesh
    {
    public:
        bool LoadMesh(const std::string &filename);
        bool LoadOVM(const std::string &filename);

        const Eigen::MatrixXd &GetVertices() const { return this->V; }
        const Eigen::MatrixXi &GetHexadera() const { return this->H; }
        const Eigen::MatrixXi &GetFaces() const { return this->F; }

    private:
        void CreateSampleMesh();
        void BuildMesh();

    private:
        Eigen::MatrixXd V; // vertex positions
        Eigen::MatrixXi H; // hexahedra
        Eigen::MatrixXi F; // faces (we don't use these here)
    };
} // namespace mesh_processing