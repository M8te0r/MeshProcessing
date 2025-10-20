#include "polyscope/polyscope.h"
#include "polyscope/volume_mesh.h"
#include "gmm/gmm.h"

#include "mesh/hMesh/mp_custom_hmesh.h"
#include "mesh/mp_mesh_creator.h"

#include <igl/readMESH.h>
#include <Eigen/Core>

#include "io/mp_vtk_io.h"
#include "io/mp_ovm_io.h"

void forTest()
{
  polyscope::init();

  // Read mesh from file
  Eigen::MatrixXd V; // vertex positions
  Eigen::MatrixXi T; // tetrahedra
  Eigen::MatrixXi F; // faces (we don't use these here)
  igl::readMESH("../assets/kitten_Tet.mesh", V, T, F);

  // Register the volume mesh with Polyscope
  polyscope::registerTetMesh("my tet mesh", V, T);
  // polyscope::registerHexMesh("my hex mesh", V, T);

  // Add a scalar quantity
  size_t nVerts = V.rows();
  std::vector<double> scalarV(nVerts);
  for (size_t i = 0; i < nVerts; i++)
  {
    // use the x-coordinate of vertex position as a test function
    scalarV[i] = V(i, 0);
  }
  polyscope::getVolumeMesh("my tet mesh2")->addVertexScalarQuantity("scalar Q", scalarV);

  // Show the GUI
  polyscope::show();
}

void HMeshTest()
{
  // CMyHMesh mesh;
  auto hmesh = mesh_processing::MeshCreator<mesh_processing::CMyHMesh>::CreateMesh();

  try
  {

    hmesh->ReadMesh("D:/dev_project/MeshProcessing/assets/Ankle_1.vtk");
    // mesh._load_Qhex("D:/dev_project/MeshProcessing/assets/bunny.QHex");
    // mesh->c_hmesh.ReadMesh("D:/dev_project/MeshProcessing/assets/Ankle_1.vtk");
    hmesh->ExportQuadrilaterals("D:/dev_project/MeshProcessing/assets/Ankle_1_output.obj");
  }
  catch (...)
  {
    std::cout << "load mesh failed!" << std::endl;
  }
}

void VTKTest()
{
  std::string filename1 = "D:/dev_project/MeshProcessing/assets/Ankle_1.vtk";
  std::vector<std::vector<double>> vertices;
  std::vector<std::vector<size_t>> cells;
  std::vector<int> cell_types;

  if (!mesh_processing::ReadVTK(filename1, vertices, cells, cell_types))
    return;

  bool all_hex = std::all_of(cell_types.begin(), cell_types.end(), [](int x)
                             { return x == 12; });
  if (all_hex)
    std::cout << "all hex!" << std::endl;
  else
    std::cout << "not all hex!" << std::endl;

  polyscope::init();
  polyscope::registerHexMesh("hex_mesh", vertices, cells);

  // Add a scalar quantity
  size_t nVerts = vertices.size();
  std::vector<double> scalarV(nVerts);
  for (size_t i = 0; i < nVerts; i++)
  {
    // use the x-coordinate of vertex position as a test function
    scalarV[i] = vertices[i][0];
  }

  polyscope::getVolumeMesh("hex_mesh")->addVertexScalarQuantity("scalar Q", scalarV);
  // Show the GUI
  polyscope::show();
}

void OVMTest()
{
  std::string filename2 = "D:/dev_project/SingularityConstrainedOctahedralFields/build/Build/test.ovm";
  std::string filename1 = "D:/dev_project/MeshProcessing/assets/sample_volume_mesh.ovm";
  std::vector<std::vector<double>> vertices;
  std::vector<std::vector<size_t>> cells;

  if (!mesh_processing::ReadOVM(filename2, vertices, cells))
    return;

  polyscope::init();
  polyscope::registerTetMesh("tet_mesh", vertices, cells);

  // Add a scalar quantity
  size_t nVerts = vertices.size();
  std::vector<double> scalarV(nVerts);
  for (size_t i = 0; i < nVerts; i++)
  {
    // use the x-coordinate of vertex position as a test function
    scalarV[i] = vertices[i][0];
  }

  polyscope::getVolumeMesh("tet_mesh")->addVertexScalarQuantity("scalar Q", scalarV);

  // Show the GUI
  polyscope::show();
}

void printUsage(const std::string &progname)
{
  std::cout << "usage: " << progname << " mesh.vtk quadrilaterals.obj" << std::endl
            << "notice: support only .obj file exporting" << std::endl
            << std::endl;
}

int main(int argc, char *argv[])
{
#if 1

  if (argc != 3)
  {
    printUsage(argv[0]);
    return 0;
  }

  auto hmesh = mesh_processing::MeshCreator<mesh_processing::CMyHMesh>::CreateMesh();
  if (!hmesh->ReadMesh(argv[1]))
  {
    std::cerr << "[ERROR] Read mesh failed!\n";
  }

  if (!hmesh->ExportQuadrilaterals(argv[2]))
  {
    std::cerr << "[ERROR] export failed!\n";
  }

  std::cout << "[Info] export success!\n";
  return 0;

#else

  std::string fn_in = "D:/dev_project/MeshProcessing/assets/bpgc.vtk";
  std::string fn_out = "D:/dev_project/MeshProcessing/assets/bpgc_output.obj";
  auto hmesh = mesh_processing::MeshCreator<mesh_processing::CMyHMesh>::CreateMesh();
  if (hmesh->ReadMesh(fn_in.c_str()))
  {
    hmesh->ExportQuadrilaterals(fn_out.c_str());
    std::cout << "\n[Info] export success!\n";
    return 0;
  }

  // TetTest();
  // HMeshTest();

  // VTKTest();
  // OVMTest();
  std::cerr << "[ERROR] export failed!\n";
  return 0;
#endif
}
