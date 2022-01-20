include(CMakeFindDependencyMacro)

find_dependency(nlohmann_json)

include("${CMAKE_CURRENT_LIST_DIR}/cvaluateTargets.cmake")