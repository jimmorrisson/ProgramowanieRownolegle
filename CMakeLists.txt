cmake_minimum_required(VERSION 3.10)
project(ParallelProgramming)

include_directories(libs/LBFGSpp/include)
include_directories(libs/eigen/)

set(SOURCES
    main.cpp
 "ExtendedRosenbrockFunction.h" "IFunction.h" "TestUtil.h" "LBFGSExampleRosenbrock.h" "QuadraticFunction.h" "PowerSingularFunction.h")

add_subdirectory(libs/eigen/)
# add_executable(ExampleQuadratic libs/LBFGSpp/example_quadratic.cpp)
add_executable(${CMAKE_PROJECT_NAME} ${SOURCES})
target_include_directories(${CMAKE_PROJECT_NAME} PRIVATE libs/LBFGSpp/include)