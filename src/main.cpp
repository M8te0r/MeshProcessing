#include "polyscope/polyscope.h"
#include "gmm/gmm.h"

int main() {
  // a few options
  polyscope::options::programName = "important app";
  polyscope::options::verbosity = 0;
  polyscope::options::usePrefsFile = false;

  polyscope::options::autocenterStructures = true;
  polyscope::options::autoscaleStructures = true;

  polyscope::init();

  while (!polyscope::windowRequestsClose()) {
    polyscope::frameTick();
  }

  polyscope::show();

  gmm::dense_matrix<int> mult_table;

  return 0;
}