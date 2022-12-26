#ifdef CMSSW_GIT_HASH
#include "MATHUSLA/Tile/interface/ProjectPath.hh"
#include "UserUtils/Common/interface/STLUtils/Filesystem.hpp"
#else
#include "ProjectPath.hh"
#endif

#ifdef CMSSW_GIT_HASH
extern const std::string project_base
  = usr::subpkgpath("MATHUSLA","Tile" ).string();
#else
extern const std::string project_base = std::string("/Users/yhlai/Desktop/test/MATHUSLA") + std::string("/Tile/");
#endif
