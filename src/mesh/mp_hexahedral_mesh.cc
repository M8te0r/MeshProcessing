#include "mp_hexahedral_mesh.h"
#include <iostream>

#include <OpenVolumeMesh/FileManager/FileManager.hh>
#include <OpenVolumeMesh/Mesh/HexahedralMesh.hh>
#include <igl/readMESH.h>

#include "io/mp_io_manager.h"
#include "utils/mp_utils.h"

namespace mesh_processing
{

    void HexahedralMesh::CreateSampleMesh()
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

        std::vector<std::vector<double>> temp_verts;
        std::vector<std::vector<unsigned int>> temp_polys;
        std::vector<int> vert_labels;   // store each vertex's face index
        std::vector<int> poly_labels;

        std::string str(filename);
        std::string filetype = "." + GetFileExtension(str);

        IOManager *io_manager = new IOManager();

        if (filetype.compare(".mesh") == 0 ||
            filetype.compare(".MESH") == 0)
        {
            io_manager->ReadMESH(filename, temp_verts, temp_polys, vert_labels, poly_labels);
            this->V = Eigen::MatrixXd(temp_verts.size(), 3);
            for (size_t i = 0; i < temp_verts.size(); i++)
            {
                this->V(i, 0) = temp_verts[i][0];
                this->V(i, 1) = temp_verts[i][1];
                this->V(i, 2) = temp_verts[i][2];
            }

            this->H = Eigen::MatrixXi(temp_polys.size(), 8);
            for (size_t i = 0; i < temp_polys.size(); i++)
            {
                if (temp_polys[i].size() != 8)
                {
                    std::cerr << "ERROR : " << __FILE__ << ", line " << __LINE__ << " : LoadMesh() : not a hexahedral mesh!" << std::endl;
                    return 1;
                }
                this->H(i, 0) = temp_polys[i][0];
                this->H(i, 1) = temp_polys[i][1];
                this->H(i, 2) = temp_polys[i][2];
                this->H(i, 3) = temp_polys[i][3];
                this->H(i, 4) = temp_polys[i][4];
                this->H(i, 5) = temp_polys[i][5];
                this->H(i, 6) = temp_polys[i][6];
                this->H(i, 7) = temp_polys[i][7];
            }

            return 1;
        }
        else if (filetype.compare(".ovm") == 0 ||
                 filetype.compare(".OVM") == 0)
        {
            io_manager->ReadOVM(filename, temp_verts, temp_polys, vert_labels, poly_labels);
            this->LoadOVM(filename);
        }
        // else if (filetype.compare(".vtu") == 0 ||
        //          filetype.compare(".VTU") == 0)
        // {
        //     io_manager->ReadVTU(filename, tmp_verts, tmp_polys);
        // }
        // else if (filetype.compare(".vtk") == 0 ||
        //          filetype.compare(".VTK") == 0)
        // {
        //     io_manager->->ReadVTK(filename, tmp_verts, tmp_polys);
        // }
        else
        {
            std::cerr << "ERROR : " << __FILE__ << ", line " << __LINE__ << " : load() : file format not supported yet " << std::endl;
        }

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