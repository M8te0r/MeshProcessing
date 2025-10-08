#pragma once
#include "mp_mesh_base.h"
#include <string>

#include <Eigen/Core>

namespace mesh_processing
{
    class Tetrahedral : public VolumeMesh
    {
    public:
        bool LoadMesh(const std::string &filename);

        const Eigen::MatrixXd &GetVertices() const { return this->V; }
        const Eigen::MatrixXi &GetTetrahedras() const { return this->T; }
        const Eigen::MatrixXi &GetFaces() const { return this->F; }

    private:
        void BuildMesh();

    private:
        Eigen::MatrixXd V; // vertex positions
        Eigen::MatrixXi T; // tetrahedra
        Eigen::MatrixXi F; // faces (we don't use these here)
    };
} // namespace mesh_processing