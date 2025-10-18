#include "mp_ovm_io.h"
#include "utils/mp_utils.h"
#include <fstream>
#include <sstream>

#include <OpenVolumeMesh/FileManager/FileManager.hh>
#include <OpenVolumeMesh/Mesh/PolyhedralMesh.hh>

namespace mesh_processing
{
    bool ReadOVM(const std::string &filename,
                 std::vector<std::vector<double>> &verts,
                 std::vector<std::vector<size_t>> &polys)
    {
        // 清空
        verts.clear();
        polys.clear();

        // Create an empty mesh object
        OpenVolumeMesh::GeometricPolyhedralMeshV3f myMesh;
        // Create file manager object
        OpenVolumeMesh::IO::FileManager fileManager;
        // Read mesh from file "myMesh.ovm" in the current directory
        fileManager.readFile(filename, myMesh);
        // Now myMesh contains the mesh specified in file "myMesh.ovm"

        verts.reserve(myMesh.n_vertices());
        int test_c_count = myMesh.n_cells();
        polys.reserve(myMesh.n_cells());

        for (const auto &v_handle : myMesh.vertices())
        {
            const auto &v = myMesh.vertex(v_handle);
            std::vector<double> point = {v[0], v[1], v[2]};
            verts.push_back(std::move(point));
        }

        // Get polyhedra (cells)
        for (const auto &c_handle : myMesh.cells())
        {
            std::vector<size_t> c_indices;

            // Get all vertices of the cell through its halffaces
            std::set<size_t> unique_vertex_indices;
            for (const auto &hf_handle : myMesh.cell_halffaces(c_handle))
            {
                for (const auto &he_handle : myMesh.halfface_halfedges(hf_handle))
                {
                    const auto &vh = myMesh.to_vertex_handle(he_handle);
                    unique_vertex_indices.insert(vh.idx());
                }
            }

            // Convert set to vector
            c_indices.assign(unique_vertex_indices.begin(), unique_vertex_indices.end());
            polys.push_back(std::move(c_indices));
        }

        return true;
    }

    bool WriteOVM(const std::string &filename,
                  const std::vector<std::vector<double>> &verts,
                  const std::vector<std::vector<size_t>> &polys)
    {
        std::ofstream file(filename);
        if (!file.is_open())
        {
            std::cerr << "[Error] Could not open file for writing: " << filename << "\n";
            return false;
        }

        file.imbue(std::locale::classic());

        // Write header
        file << "OVM ASCII" << std::endl;

        uint64_t n_vertices(verts.size());
        file << "Vertices" << std::endl;
        file << n_vertices << std::endl;

        for (const auto &v : verts)
        {
            file << v[0] << " " << v[1] << " " << v[2] << std::endl;
        }

        uint64_t n_cells(polys.size());
        file << "Polyhedra" << std::endl;
        file << n_cells << std::endl;

        for (const auto &c : polys)
        {
            file << static_cast<uint64_t>(c.size()) << " ";
            for (const auto &cv : c)
            {
                file << cv;
                // if ((*cv + 1) != halffaces.end())
                //     file << " ";
            }
        }
    }

} // namespace mesh_processing
