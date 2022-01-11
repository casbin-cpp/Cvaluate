include(CMakeFindDependencyMacro)

# find_package(json 3.10.1 REQUIRED)
find_dependency(nlohmann_json)
# find_dependency(json REQUIRED)

include("${CMAKE_CURRENT_LIST_DIR}/cvaluateTargets.cmake")