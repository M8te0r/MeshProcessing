cmake_minimum_required(VERSION 3.20)
project(MeshProcessing)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wa,-mbig-obj,-O2")

if(MSVC)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /bigobj")
endif()


# output directories
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_CURRENT_SOURCE_DIR}/bin)

# hierarchy directories
# set(prj_root_dir ${CMAKE_CURRENT_SOURCE_DIR})
# set(app_root_dir ${prj_root_dir}/app)
# set(sandbox_root_dir ${prj_root_dir}/sandbox)
# set(deps_root_dir ${prj_root_dir}/deps)

# add_subdirectory(app)
# add_subdirectory(sandbox)
# add_subdirectory(deps)


# dependencies

add_subdirectory(dependencies/polyscope)

add_subdirectory(dependencies/OpenMesh)

add_subdirectory(dependencies/OpenVolumeMesh)

add_subdirectory(dependencies/eigen)

add_subdirectory(dependencies/libigl)



file(GLOB_RECURSE app_head_files "${CMAKE_CURRENT_SOURCE_DIR}/src/*.h")
file(GLOB_RECURSE app_source_files "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp" "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cc")

add_executable(${PROJECT_NAME} ${app_head_files} ${app_source_files})


target_include_directories(
    ${PROJECT_NAME} 
    PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/src
    ${CMAKE_CURRENT_SOURCE_DIR}/dependencies/gmm-5.4/include
    )

target_link_libraries(
    ${PROJECT_NAME} 
    PRIVATE 
    polyscope
    OpenMeshCore
    OpenMeshTools
    OpenVolumeMesh::OpenVolumeMesh
    Eigen3::Eigen
    igl::core
)