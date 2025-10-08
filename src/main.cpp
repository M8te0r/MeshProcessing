#include "polyscope/polyscope.h"
#include "polyscope/volume_mesh.h"
#include "gmm/gmm.h"

#include "mesh/mp_tetrahedral_mesh.h"
#include "mesh/mp_hexahedral_mesh.h"
#include "mesh/mp_mesh_creator.h"

#include <igl/readMESH.h>
#include <Eigen/Core>

int main()
{

#if 1
  // 创建一个六面体网格对象
  auto hex = mesh_processing::MeshCreator<mesh_processing::HexahedralMesh>::CreateMesh();
  if (!hex->LoadMesh("../assets/sample_volume_mesh.ovm"))
  {
    std::cout << "load hex mesh failed!" << std::endl;
  }

  auto tet = mesh_processing::MeshCreator<mesh_processing::Tetrahedral>::CreateMesh();
  if (!tet->LoadMesh("../assets/kitten_Tet.mesh"))
  {
    std::cout << "load tet mesh failed!" << std::endl;
  }
  // polyscope::registerTetMesh("my tet mesh", tet->GetVertices(), tet->GetTetrahedras());

  polyscope::init();
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


#else
  // a few options
  polyscope::options::programName = "important app";
  polyscope::options::verbosity = 0;
  polyscope::options::usePrefsFile = false;

  polyscope::options::autocenterStructures = true;
  polyscope::options::autoscaleStructures = true;

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
#endif

  return 0;
}