cmake_minimum_required(VERSION 3.10)
project(ParallelProgramming)

include_directories(libs/LBFGSpp/include)
include_directories(libs/eigen/)

set (CMAKE_CXX_STANDARD 17)
find_package(OpenMP REQUIRED)

set(HEADERS
    ExtendedRosenbrockFunction.h  
    IFunction.h
    TestUtil.h
    LBFGSExampleRosenbrock.h
    QuadraticFunction.h
    PowerSingularFunction.h
    BFGSolver.h
    )

set(SOURCES
    main.cpp
    )

add_subdirectory(libs/eigen/)
add_executable(${CMAKE_PROJECT_NAME} ${SOURCES} ${HEADERS})
target_include_directories(${CMAKE_PROJECT_NAME} PRIVATE libs/LBFGSpp/include)
target_link_libraries(${CMAKE_PROJECT_NAME} PUBLIC OpenMP::OpenMP_CXX)