#pragma once
#include <memory>
namespace mesh_processing
{

    template <typename MeshT>
    class MeshCreator
    {
    private:
        MeshCreator() = default; // 私有构造函数，防止实例化
    public:
        ~MeshCreator() = default;
        static std::unique_ptr<MeshT> CreateMesh()
        {
            return std::make_unique<MeshT>();
        }
    };
} // namespace name
