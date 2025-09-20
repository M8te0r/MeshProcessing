#include "polyscope/polyscope.h"

int main()
{
  // a few options
  polyscope::options::programName = "important app";
  polyscope::options::verbosity = 0;
  polyscope::options::usePrefsFile = false;

  polyscope::init();

  while (!polyscope::windowRequestsClose())
  {
    polyscope::frameTick();
  }

  polyscope::show();

  return 0;
}