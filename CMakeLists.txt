cmake_minimum_required(VERSION 3.10)
project(ParallelProgramming)

include_directories(libs/LBFGSpp/include)
include_directories(libs/eigen/)

set (CMAKE_CXX_STANDARD 17)
add_compile_options(-Wall)
find_package(OpenMP REQUIRED)

file(GLOB_RECURSE LIBS_DIR "libs/*.c" "libs/*.h" "libs/*.cpp" "libs/*.hpp")

set_source_files_properties(
  ${LIBS_DIR}
  PROPERTIES
  COMPILE_FLAGS ""
)

set(SOURCES
    main.cpp
    BFGSolver.cpp
    IFunction.cpp
    PowerSingularFunction.cpp
    )

add_subdirectory(libs/eigen/)
add_executable(${CMAKE_PROJECT_NAME} ${SOURCES} ${HEADERS})
target_include_directories(${CMAKE_PROJECT_NAME} PRIVATE libs/LBFGSpp/include)
target_link_libraries(${CMAKE_PROJECT_NAME} PUBLIC OpenMP::OpenMP_CXX)