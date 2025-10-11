#pragma once

#include <stdio.h>
#include <vector>
#include "mesh/HMesh/Geometry/Plane.h"
#include "mesh/HMesh/Parser/strutil.h"
#include "mesh/HMesh/Parser/Parser.h"
#include "mesh/HMesh/HexMesh/basehmesh.h"
#include "mesh/HMesh/HexMesh/hiterators.h"

namespace MeshProcessing
{

    

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
    };

    typedef MyMesh<HMeshLib::CHVertex, HMeshLib::CVertex, HMeshLib::CHalfEdge, HMeshLib::CHEdge, HMeshLib::CEdge, HMeshLib::CHalfFace, HMeshLib::CFace, HMeshLib::CHex> CMyHMesh;

};