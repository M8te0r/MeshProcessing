#pragma once
#include "utils/mp_utils.h"
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
            code = ReadVTK(filename, m_file_vertices, m_file_cells, cell_types);
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
        }
        return false;
    }

    template <typename HexV, typename V, typename HE, typename HexE, typename E, typename HF, typename F, typename Hex>
    bool MyMesh<HexV, V, HE, HexE, E, HF, F, Hex>::ExportQuadrilaterals(const char *filename) const
    {
        std::vector<std::vector<size_t>> expo_faces;
        std::vector<std::vector<double>> expo_vertices;
        std::vector<size_t> unique_boundary_vertices; 
        std::unordered_set<size_t> marks;


        for (const auto &hf : this->m_pHalfFaces)
        {
            if (hf->face()->boundary())
            {
                // 获取边界面
                std::vector<size_t> vIndices = hf->GetVertexIndices();
                expo_faces.push_back(vIndices);

                // 获取边界顶点
                for(const auto& idx:vIndices){
                    if(marks.find(idx)==mars.end()){
                        marks.insert(idx);
                        unique_boundary_vertices.insert(idx);
                    }
                }
            }
        }

        expo_vertices;
        for(size_t expo_index=0;expo_index<unique_boundary_vertices.size();++expo_index){

        }
        for(const auto&v:unique_boundary_vertices){
            std::vector<double> coord={this->idVertex(v)[0],this->idVertex(v)[1],this->idVertex(v)[2]};

        }

        return true;
    }

} // namespace mesh_processing
