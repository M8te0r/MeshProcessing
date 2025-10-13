#include "mp_vtk_io.h"
#include "utils/mp_utils.h"
#include <iostream>
#include <fstream>
#include <sstream>

namespace mesh_processing
{
    bool ReadVTK(const std::string &filename,
                  std::vector<std::vector<double>> &out_vertices,
                  std::vector<std::vector<size_t>> &out_cells,
                  std::vector<int> &out_cell_types)
    {
        std::ifstream file(filename); // 打开文件

        //  --- 0. 检查文件是否能打开 ---
        if (!file.is_open())
        {
            std::cerr << "[Error] Failed to open file: " << filename << ".\n";
            return false;
        }

        // --- 1. 版本信息（通常是注释行） ---
        std::string line;
        std::getline(file, line);
        if (line != "# vtk DataFile Version 3.0 ")
        {
            std::cerr << "[Error] Unsupported VTK version. Expected '# vtk DataFile Version 3.0 '.\n"
                      << std::endl;
            return false;
        }

        // --- 2. 标题行 ---
        if (!ReadNextTrimLine(file, line))
            return false;

        std::string title = line;
        // std::cout << "Title: " << title << "\n";


        // --- 3. 存储格式（ASCII / BINARY） ---
        if (!ReadNextTrimLine(file, line))
            return false;
        std::string format = line;
        // std::cout << "Format: " << format << "\n";
        if (format != "ASCII")
        {
            std::cerr << "[Error] Only ASCII files are supported!" << std::endl;
            return false;
        }

        // --- 4. 数据集类型 ---
        if (!ReadNextTrimLine(file, line))
            return false;

        std::istringstream iss(line);
        std::string key, dataset_type;
        iss >> key >> dataset_type;
        if (key != "DATASET" || dataset_type != "UNSTRUCTURED_GRID")
        {
            std::cerr << "[Error] Only DATASET UNSTRUCTURED_GRID is supported.\n";
            return false;
        }

        // --- 5. 读取点信息 ---
        if (!ReadNextTrimLine(file, line))
            return false;

        iss.clear();
        iss.str(line);

        size_t point_num = 0;
        std::string point_type;
        iss >> key >> point_num >> point_type;
        if (key != "POINTS")
        {
            std::cerr << "[Error] Expected POINTS but got " << key << ".\n";
            return false;
        }

        // 保存坐标
        std::vector<std::vector<double>> points;
        points.reserve(point_num);
        for (size_t i = 0; i < point_num; ++i)
        {
            if (!ReadNextTrimLine(file, line))
                return false;

            std::istringstream iss_point(line);
            std::vector<double> point(3);

            if (!(iss_point >> point[0] >> point[1] >> point[2]))
            {
                std::cerr << "[Error] Failed to read coordinates for point " << i << ".\n";
                return false;
            }
            points.push_back(std::move(point));
        }

        // --- 5. 读取 CELLS ---
        if (!ReadNextTrimLine(file, line))
            return false;

        iss.clear();
        iss.str(line);
        size_t num_cells = 0;
        size_t total_index_count = 0;
        iss >> key >> num_cells >> total_index_count;

        if (key != "CELLS")
        {
            std::cerr << "[Error] Expected CELLS but got " << key << ".\n";
            return false;
        }

        std::vector<std::vector<size_t>> cells;
        cells.reserve(num_cells);

        for (size_t i = 0; i < num_cells; ++i)
        {
            if (!ReadNextTrimLine(file, line))
                return false;

            std::istringstream iss_cell(line);
            size_t num_vert_in_cell;
            iss_cell >> num_vert_in_cell;
            std::vector<size_t> cell(num_vert_in_cell);
            for (size_t j = 0; j < num_vert_in_cell; ++j)
                iss_cell >> cell[j];
            cells.push_back(std::move(cell));
        }

        // --- 7. 读取 CELL_TYPES ---
        if (!ReadNextTrimLine(file, line))
            return false;

        iss.clear();
        iss.str(line);

        size_t num_cell_types = 0;
        iss >> key >> num_cell_types;
        if (key != "CELL_TYPES")
        {
            std::cerr << "[Error] Expected CELL_TYPES but got " << key << ".\n";
            return false;
        }

        if (num_cell_types != num_cells)
        {
            std::cerr << "[Error] CELL_TYPES count does not match CELLS count.\n";
            return false;
        }

        std::istringstream iss_cell_type(line);
        std::vector<int> cell_types(num_cell_types);
        for (size_t i = 0; i < num_cell_types; ++i)
        {
            if (!ReadNextTrimLine(file, line))
                return false;
            iss_cell_type >> cell_types[i];
        }

        // 成功读取
        std::cout << "[Info] VTK file loaded successfully.\n";
        std::cout << "[Info] Title: " << title << "\n";
        std::cout << "[Info] Points: " << point_num << "\n";
        std::cout << "[Info] Cells: " << num_cells << "\n";
        std::cout << "[Info] Cell types: " << num_cell_types << "\n";

        out_vertices = points;
        out_cells = cells;
        out_cell_types = cell_types;
        return true;
    }

    bool WriteVTK(const std::string &filename,
                  const std::vector<std::vector<double>> &vertices,
                  const std::vector<std::vector<size_t>> &cells,
                  const std::vector<int> &cell_types)
    {
        std::ofstream file(filename);
        if (!file.is_open())
        {
            std::cerr << "[Error] Could not open file for writing: " << filename << "\n";
            return false;
        }

        // 写入头部信息
        file << "# vtk DataFile Version 3.0\n";
        file << filename << "\n";
        file << "ASCII\n";
        file << "DATASET UNSTRUCTURED_GRID\n";

        // 写入点信息
        file << "POINTS " << vertices.size() << " double\n";
        for (const auto &v : vertices)
        {
            file << v[0] << " " << v[1] << " " << v[2] << "\n";
        }

        // 写入单元格信息
        size_t total_indices = 0;
        for (const auto &cell : cells)
            total_indices += cell.size() + 1; // 每个单元格前面有一个顶点数

        file << "CELLS " << cells.size() << " " << total_indices << "\n";
        for (const auto &cell : cells)
        {
            file << cell.size();
            for (const auto &idx : cell)
                file << " " << idx;
            file << "\n";
        }

        // 写入单元格类型信息
        file << "CELL_TYPES " << cell_types.size() << "\n";
        for (const auto &ct : cell_types)
        {
            file << ct << "\n";
        }

        std::cout << "[Info] VTK file written successfully: " << filename << "\n";
        return true;
    }
}