#pragma once
#include "mp_mesh_base.h"
#include <string>

namespace mesh_processing
{
    class HexahedralMesh : public VolumeMesh
    {
    public:
        bool LoadMesh(const std::string &filename);
        bool LoadOVM(const std::string &filename);

    private:
        void BuildMesh();

    private:
    };
} // namespace mesh_processing