#pragma once
#include "utils/mp_utils.h"

#include "io/mp_mesh_io.h"
#include "io/mp_vtk_io.h"
#include "io/mp_ovm_io.h"
#include "io/mp_obj_io.h"
#include <unordered_set>
namespace mesh_processing
{

    template <typename HexV, typename V, typename HE, typename HexE, typename E, typename HF, typename F, typename Hex>
    bool MyMesh<HexV, V, HE, HexE, E, HF, F, Hex>::ReadMesh(const char *filename)
    {
        std::vector<int> cell_types;
        std::vector<int> vert_labels;
        std::vector<int> poly_labels;

        std::string filetype = "." + GetFileExtension(filename);
        bool code = false;
        if (filetype.compare(".mesh") == 0 || filetype.compare(".MESH") == 0)
        {
            code = ReadMESH(filename, m_file_vertices, m_file_cells, vert_labels, poly_labels);
        }
        else if (filetype.compare(".vtk") == 0 || filetype.compare(".VTK") == 0)
        {
            code = ReadVTK2(filename, m_file_vertices, m_file_cells, cell_types);
        }
        else if (filetype.compare(".ovm") == 0 || filetype.compare(".OVM") == 0)
        {
            code = ReadOVM(filename, m_file_vertices, m_file_cells);
        }

        if (code)
        {
            this->m_nVertices = m_file_vertices.size();
            for (size_t i = 0; i < m_file_vertices.size(); ++i)
            {
                HMeshLib::CPoint p(m_file_vertices[i][0], m_file_vertices[i][1], m_file_vertices[i][2]);
                HMeshLib::CVertex *v = new HMeshLib::CVertex();
                v->id() = i;
                v->position() = p;

                this->m_pVertices.push_back(v);
                this->m_map_Vertices.insert(std::pair<int, HMeshLib::CVertex *>(i, v));
            }

            this->m_nHexs = m_file_cells.size();
            for (size_t hId = 0; hId < m_file_cells.size(); ++hId)
            {
                int vIds[] = {
                    static_cast<int>(m_file_cells[hId][0]),
                    static_cast<int>(m_file_cells[hId][1]),
                    static_cast<int>(m_file_cells[hId][2]),
                    static_cast<int>(m_file_cells[hId][3]),
                    static_cast<int>(m_file_cells[hId][4]),
                    static_cast<int>(m_file_cells[hId][5]),
                    static_cast<int>(m_file_cells[hId][6]),
                    static_cast<int>(m_file_cells[hId][7]),
                };

                HMeshLib::CHex *pHex = new HMeshLib::CHex();
                this->m_pHexs.push_back(pHex);
                this->m_map_Hexs.insert(std::pair<int, HMeshLib::CHex *>(hId, pHex));
                this->_construct_hex(pHex, hId, vIds);
            }

            this->_construct_faces();
            this->_construct_edges();

            for (typename std::list<HMeshLib::CVertex *>::iterator vIter = this->m_pVertices.begin(); vIter != this->m_pVertices.end(); vIter++)
            {
                HMeshLib::CVertex *pV = *vIter;
                if (pV->id() > this->m_maxVertexId)
                {
                    this->m_maxVertexId = pV->id();
                }
            }
            return true;
        }
        return false;
    }



    template <typename HexV, typename V, typename HE, typename HexE, typename E, typename HF, typename F, typename Hex>
    bool MyMesh<HexV, V, HE, HexE, E, HF, F, Hex>::ExportQuadrilaterals(const char *filename)
    {
        std::vector<std::vector<size_t>> boundary_faces;
        std::vector<size_t> unique_boundary_vertices;
        std::unordered_set<size_t> marks;
        boundary_faces.reserve(this->m_pHalfFaces.size());

        // 1. 收集边界面和顶点
        for (auto hf : this->m_pHalfFaces)
        {
            if (hf->face()->boundary())
            {
                // 获取边界面
                std::vector<size_t> vIndices;
                vIndices.reserve(4);
                // 遍历面上的每个half-edge，获得有顺序的顶点
                auto phe = this->HalfFaceHalfEdge(hf);
                for (int k = 0; k < 4; k++)
                {
                    vIndices.push_back(this->HalfEdgeTarget(phe)->id());
                    phe = this->HalfEdgeNext(phe);
                }
                
                boundary_faces.push_back(vIndices);

                // 获取边界顶点
                for (auto idx : vIndices)
                {
                    if (marks.find(idx) == marks.end())
                    {
                        marks.insert(idx);
                        unique_boundary_vertices.push_back(idx);
                    }
                }
            }
        }

        // 2. 构建旧索引 -> 新索引的映射
        std::unordered_map<size_t, size_t> old_to_new;
        old_to_new.reserve(unique_boundary_vertices.size());
        for (size_t new_idx = 0; new_idx < unique_boundary_vertices.size(); ++new_idx)
        {
            old_to_new[unique_boundary_vertices[new_idx]] = new_idx;
        }

        std::vector<std::vector<double>> vertices_coords;
        vertices_coords.reserve(unique_boundary_vertices.size());
        for (auto old_idx : unique_boundary_vertices)
        {
            // const auto &p = m_vertices[old_idx];  // 这里假设 m_vertices 存储原始坐标
            auto p = this->idVertex(static_cast<int>(old_idx))->position(); // 获取每个CPoint
            std::vector<double> tem_point={p[0],p[1],p[2]};
            vertices_coords.push_back(std::move(tem_point));
        }

        // 3. 重新映射边界面索引
        std::vector<std::vector<size_t>> remapped_faces;
        remapped_faces.reserve(boundary_faces.size());
        for (const auto &face : boundary_faces)
        {
            std::vector<size_t> new_face;
            new_face.reserve(face.size());
            for (auto old_idx : face)
            {
                new_face.push_back(old_to_new[old_idx]);
            }
            remapped_faces.push_back(new_face);
        }

        // 4. 写文件
        return WriteOBJ(filename, vertices_coords, remapped_faces);
    }

} // namespace mesh_processing
