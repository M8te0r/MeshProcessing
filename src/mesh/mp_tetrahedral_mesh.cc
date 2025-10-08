#include "mp_tetrahedral_mesh.h"

#include <igl/readMESH.h>

namespace mesh_processing
{
    bool Tetrahedral::LoadMesh(const std::string &filename)
    {
        // TODO: Implement mesh loading logic here
        return igl::readMESH(filename, this->V, this->T, this->F);
    }

    void Tetrahedral::BuildMesh()
    {
    }
}