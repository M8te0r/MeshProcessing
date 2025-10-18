#pragma once

#include <stdio.h>
#include <vector>
#include "mesh/HMesh/HMeshCore/Geometry/Plane.h"
#include "mesh/HMesh/HMeshCore/Parser/strutil.h"
#include "mesh/HMesh/HMeshCore/Parser/parser.h"
#include "mesh/HMesh/HMeshCore/HexMesh/basehmesh.h"
#include "mesh/HMesh/HMeshCore/HexMesh/hiterators.h"

#include "io/mp_mesh_io.h"
#include "io/mp_vtk_io.h"
#include "io/mp_ovm_io.h"

namespace mesh_processing
{

    class MyHalfFace : public HMeshLib::CHalfFace
    {
    public:
        MyHalfFace() {};
        ~MyHalfFace() {};

        inline std::vector<size_t> GetVertexIndices()const {return std::vector<size_t>{static_cast<size_t>(m_key[0]),static_cast<size_t>(m_key[1]),static_cast<size_t>(m_key[2]),static_cast<size_t>(m_key[3])};}
    };

    template <typename HexV, typename V, typename HE, typename HexE, typename E, typename HF, typename F, typename Hex>
    class MyMesh : public HMeshLib::CHMesh<HexV, V, HE, HexE, E, HF, F, Hex>
    {
    public:
        // typedef HMeshLib::HMeshHexIterator<HexV, V, HE, HexE, E, HF, F, Hex> MeshHexIterator;
        // typedef HMeshLib::HexHalfFaceIterator<HexV, V, HE, HexE, E, HF, F, Hex> HexHFIterator;
        // typedef HMeshLib::HMeshEdgeIterator<HexV, V, HE, HexE, E, HF, F, Hex> MeshEdgeIterator;
        // typedef HMeshLib::HMeshFaceIterator<HexV, V, HE, HexE, E, HF, F, Hex> MeshFaceIterator;
        // typedef HMeshLib::HalfFaceVertexIterator<HexV, V, HE, HexE, E, HF, F, Hex> HalfFaceVertexIterator;
        // typedef HMeshLib::FaceVertexIterator<HexV, V, HE, HexE, E, HF, F, Hex> FaceVertexIterator;
        // typedef HMeshLib::HMeshVertexIterator<HexV, V, HE, HexE, E, HF, F, Hex> MeshVertexIterator;
        // typedef HMeshLib::HVertexVertexIterator<HexV, V, HE, HexE, E, HF, F, Hex> VertexVertexIterator;

    public:
        MyMesh() = default;
        ~MyMesh() = default;

    public:
        bool ReadMesh(const char *filename);
        bool ExportQuadrilaterals(const char *filename) const;
    private:
        

    private:

        std::vector<std::vector<double>> m_file_vertices;
        std::vector<std::vector<size_t>> m_file_cells;
    };

    typedef MyMesh<HMeshLib::CHVertex, HMeshLib::CVertex, HMeshLib::CHalfEdge, HMeshLib::CHEdge, HMeshLib::CEdge, MyHalfFace, HMeshLib::CFace, HMeshLib::CHex> CMyHMesh;

};

#include "mp_custom_hmesh.inl"