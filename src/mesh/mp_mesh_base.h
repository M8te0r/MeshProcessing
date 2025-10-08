#pragma once
#include <string>
namespace mesh_processing
{
    class MeshBase
    {
    public:
        MeshBase() = default;
        virtual ~MeshBase() = default;
    };

    class SurfaceMesh : public MeshBase
    {
    public:
        ~SurfaceMesh() override = default;
    };

    class VolumeMesh : public MeshBase
    {
    public:
        ~VolumeMesh() override = default;
    };
} // namespace mesh_processing