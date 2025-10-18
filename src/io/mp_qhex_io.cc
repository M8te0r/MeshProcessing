#include "mp_qhex_io.h"
#include <iostream>
#include <fstream>
#include <sstream>
namespace mesh_processing
{
    bool ReadQHEX(const char *&filename,
                  std::vector<std::vector<double>> &verts,
                  std::vector<std::vector<size_t>> &hexs)
    {
        verts.clear();
        hexs.clear();

        std::ifstream fin(filename);
        if (!fin.is_open())
        {
            std::cerr << "Failed to open file: " << filename << std::endl;
            return false;
        }

        std::string line;
        while (std::getline(fin, line))
        {
            // 跳过空行
            if (line.empty())
                continue;

            std::istringstream iss(line);
            std::string keyword;
            iss >> keyword;

            if (keyword == "Vertex")
            {
                size_t vid;
                double x, y, z;
                iss >> vid >> x >> y >> z;

                // 确保verts大小足够
                if (verts.size() < vid)
                    verts.resize(vid);
                verts[vid - 1] = {x, y, z};
            }
            else if (keyword == "Hex")
            {
                size_t hid;
                std::vector<size_t> h(8);
                iss >> hid;
                for (int i = 0; i < 8; ++i)
                    iss >> h[i];

                hexs.push_back(h);
            }
            // 其他关键字不处理
        }

        fin.close();
        return true;
    }
} // namespace mesh_processing
