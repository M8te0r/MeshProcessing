#include "mp_obj_io.h"

#include <iostream>
#include <fstream>
#include <sstream>

#include "utils/mp_utils.h"

namespace mesh_processing
{
    bool WriteOBJ(const char *filename, std::vector<std::vector<double>> vertices, std::vector<std::vector<size_t>> face_connectivity)
    {
        std::ofstream file(filename);
        if (!file.is_open())
        {
            std::cerr << "[Error] Could not open file for writing: " << filename << "\n";
            return false;
        }

        // 写入头部信息
        file << "# WaveFront obj " << filename << "\n";
        file << "# Mesh info: \n";
        file << "# [vertices]:" << vertices.size() << ", [faces]:" << face_connectivity.size() << "\n";
        file << "\n";

        // 写入点信息
        for (const auto &v : vertices)
        {
            file << "v " << v[0] << " " << v[1] << " " << v[2] << "\n";
        }

        file << "\n";

        // 写入face信息
        for (const auto &fs : face_connectivity)
        {
            //  file << "f " << f[0] << " " << f[1] << " " << f[2] << "\n";
            file << "f ";
            for (const auto &fv : fs)
            {
                file << fv + 1 << " ";  // 注意：面连通性中，顶点索引从1开始
            }
            file << "\n";
        }

        std::cout << "[Info] OBJ file written successfully: " << filename << "\n";
        return true;
    }

} // namespace mesh_processing