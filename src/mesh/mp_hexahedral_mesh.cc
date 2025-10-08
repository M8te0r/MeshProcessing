#include "mp_hexahedral_mesh.h"
#include <iostream>

#include <OpenVolumeMesh/FileManager/FileManager.hh>
#include <OpenVolumeMesh/Mesh/HexahedralMesh.hh>
#include <igl/readMESH.h>

namespace mesh_processing
{

    void HexahedralMesh::BuildMesh()
    {
        // Create mesh object
        OpenVolumeMesh::GeometricHexahedralMeshV3f myMesh;
        // Add eight vertices
        OpenVolumeMesh::VertexHandle v0 = myMesh.add_vertex(OpenVolumeMesh::Vec3f(-1.0, 0.0, 0.0));
        OpenVolumeMesh::VertexHandle v1 = myMesh.add_vertex(OpenVolumeMesh::Vec3f(0.0, 0.0, 1.0));
        OpenVolumeMesh::VertexHandle v2 = myMesh.add_vertex(OpenVolumeMesh::Vec3f(1.0, 0.0, 0.0));
        OpenVolumeMesh::VertexHandle v3 = myMesh.add_vertex(OpenVolumeMesh::Vec3f(0.0, 0.0, -1.0));
        OpenVolumeMesh::VertexHandle v4 = myMesh.add_vertex(OpenVolumeMesh::Vec3f(0.0, 1.0, 0.0));
        std::vector<OpenVolumeMesh::VertexHandle> vertices;
        // Add faces
        vertices.push_back(v0);
        vertices.push_back(v1);
        vertices.push_back(v4);
        OpenVolumeMesh::FaceHandle f0 = myMesh.add_face(vertices);
        vertices.clear();
        vertices.push_back(v1);
        vertices.push_back(v2);
        vertices.push_back(v4);
        OpenVolumeMesh::FaceHandle f1 = myMesh.add_face(vertices);
        vertices.clear();
        vertices.push_back(v0);
        vertices.push_back(v1);
        vertices.push_back(v2);
        OpenVolumeMesh::FaceHandle f2 = myMesh.add_face(vertices);
        vertices.clear();
        vertices.push_back(v0);
        vertices.push_back(v4);
        vertices.push_back(v2);
        OpenVolumeMesh::FaceHandle f3 = myMesh.add_face(vertices);
        vertices.clear();
        vertices.push_back(v0);
        vertices.push_back(v4);
        vertices.push_back(v3);
        OpenVolumeMesh::FaceHandle f4 = myMesh.add_face(vertices);
        vertices.clear();
        vertices.push_back(v2);
        vertices.push_back(v3);
        vertices.push_back(v4);
        OpenVolumeMesh::FaceHandle f5 = myMesh.add_face(vertices);
        vertices.clear();
        vertices.push_back(v0);
        vertices.push_back(v2);
        vertices.push_back(v3);
        OpenVolumeMesh::FaceHandle f6 = myMesh.add_face(vertices);
        std::vector<OpenVolumeMesh::HalfFaceHandle> halffaces;
        // Add first tetrahedron
        halffaces.push_back(myMesh.halfface_handle(f0, 1));
        halffaces.push_back(myMesh.halfface_handle(f1, 1));
        halffaces.push_back(myMesh.halfface_handle(f2, 0));
        halffaces.push_back(myMesh.halfface_handle(f3, 1));
        myMesh.add_cell(halffaces);
        // Add second tetrahedron
        halffaces.clear();
        halffaces.push_back(myMesh.halfface_handle(f4, 1));
        halffaces.push_back(myMesh.halfface_handle(f5, 1));
        halffaces.push_back(myMesh.halfface_handle(f3, 0));
        halffaces.push_back(myMesh.halfface_handle(f6, 0));
        myMesh.add_cell(halffaces);
    }

    bool HexahedralMesh::LoadMesh(const std::string &filename)
    {
        // TODO:
        return 0;
    }

    bool HexahedralMesh::LoadOVM(const std::string &filename)
    {
        // Create mesh object
        OpenVolumeMesh::GeometricHexahedralMeshV3f myMesh;

        // Create file manager object
        OpenVolumeMesh::IO::FileManager fileManager;

        // Read mesh from file "myMesh.ovm" in the current directory
        return fileManager.readFile(filename, myMesh);
        // Now myMesh contains the mesh specified in file "myMesh.ovm"
    }

} // namespace mesh_processing