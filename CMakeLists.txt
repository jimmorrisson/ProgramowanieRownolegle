cmake_minimum_required(VERSION 3.10)
project(ParallelProgramming)

include_directories(libs/LBFGSpp/include)
include_directories(libs/eigen/)
include_directories(libs/Catch2/)
include_directories(math/)

add_subdirectory(math/)

set (CMAKE_CXX_STANDARD 17)
option(WITH_PARALLEL_PROG "Option description" ON)

if(WITH_PARALLEL_PROG)
  add_definitions(-DUSE_PARALLEL_PROG)
  if (WIN32)
    add_compile_options(/W4 /Od)
  else()
    add_compile_options(-Wall -O0)
  endif()
  find_package(OpenMP REQUIRED)
else()
  if (WIN32)
    add_compile_options(/W4 /Od -DEIGEN_DONT_PARALLELIZE)
  else()
    add_compile_options(-Wall -O0 -DEIGEN_DONT_PARALLELIZE)
  endif()
endif()

file(GLOB_RECURSE LIBS_DIR "libs/*.c" "libs/*.h" "libs/*.cpp" "libs/*.hpp")

set_source_files_properties(
  ${LIBS_DIR}
  PROPERTIES
  COMPILE_FLAGS ""
)

set(SOURCES
    main.cpp
    BFGSolver.cpp
    LBFGSExampleRosenbrock.cpp
    IFunction.cpp
    PowerSingularFunction.cpp
    )

add_compile_definitions(PARALLEL_PROG)
add_subdirectory(libs/eigen/)
add_executable(${CMAKE_PROJECT_NAME} ${SOURCES} ${HEADERS})
target_include_directories(${CMAKE_PROJECT_NAME} PRIVATE libs/LBFGSpp/include)
if(WITH_PARALLEL_PROG)
  target_link_libraries(${CMAKE_PROJECT_NAME} PUBLIC OpenMP::OpenMP_CXX math)
else()
  target_link_libraries(${CMAKE_PROJECT_NAME} math)
endif()
