#include "polyscope/polyscope.h"
#include "polyscope/volume_mesh.h"
#include "gmm/gmm.h"

#include "mesh/mp_tetrahedral_mesh.h"
#include "mesh/mp_hexahedral_mesh.h"
#include "mesh/mp_mesh_creator.h"

#include <igl/readMESH.h>
#include <Eigen/Core>

#include "mesh/mp_test_hmesh.h"
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

void TetTest()
{
  std::string filename="D:/dev_project/SingularityConstrainedOctahedralFields/demo/scof/ellipsoid.ovm";
  
  polyscope::init();

  auto tet = mesh_processing::MeshCreator<mesh_processing::Tetrahedral>::CreateMesh();
  if (!tet->LoadMesh("D:/dev_project/MeshProcessing/assets/Kitten_Tet.mesh"))
  {
    std::cout << "load tet mesh failed!" << std::endl;
  }
  polyscope::registerTetMesh("tet_mesh", tet->GetVertices(), tet->GetTetrahedras());
  // Add a scalar quantity
  size_t nVerts = tet->GetVertices().rows();
  std::vector<double> scalarV(nVerts);
  for (size_t i = 0; i < nVerts; i++)
  {
    // use the x-coordinate of vertex position as a test function
    scalarV[i] = tet->GetVertices()(i, 0);
  }

  polyscope::getVolumeMesh("tet_mesh")->addVertexScalarQuantity("scalar Q", scalarV);

  // Show the GUI
  polyscope::show();
}

void HexTest()
{
  polyscope::init();

  auto hex = mesh_processing::MeshCreator<mesh_processing::HexahedralMesh>::CreateMesh();
  if (!hex->LoadMesh("D:/dev_project/MeshProcessing/assets/polycube_result_1.0_Hex.mesh"))
  {
    std::cout << "load hex mesh failed!" << std::endl;
  }

  polyscope::registerHexMesh("hex_mesh", hex->GetVertices(), hex->GetHexadera());

  // Add a scalar quantity
  size_t nVerts = hex->GetVertices().rows();
  std::vector<double> scalarV(nVerts);
  for (size_t i = 0; i < nVerts; i++)
  {
    // use the x-coordinate of vertex position as a test function
    scalarV[i] = hex->GetVertices()(i, 0);
  }

  polyscope::getVolumeMesh("hex_mesh")->addVertexScalarQuantity("scalar Q", scalarV);
  // Show the GUI
  polyscope::show();
}

void HMeshTest()
{
  // CMyHMesh mesh;
  mesh_processing::CMyHMesh mesh;
  try
  {
    mesh._load_Qhex("D:/dev_project/MeshProcessing/assets/bunny.QHex");
  }
  catch (...)
  {
    std::cout << "load mesh failed!" << std::endl;
  }
}

void VTKTest()
{
  std::string filename1="D:/dev_project/MeshProcessing/assets/Ankle_1.vtk";
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

void OVMTest(){
  std::string filename2="D:/dev_project/SingularityConstrainedOctahedralFields/build/Build/test.ovm";
  std::string filename1="D:/dev_project/MeshProcessing/assets/sample_volume_mesh.ovm";
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

int main()
{
  //TetTest();
  // HMeshTest();

  //VTKTest();
  OVMTest();
  return 0;
}