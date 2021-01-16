cmake_minimum_required(VERSION 3.10)
project(ParallelProgramming)

include_directories(math/)

add_subdirectory(math/)

set (CMAKE_CXX_STANDARD 17)
option(WITH_OPEN_MPI "Option description" ON)

if(WITH_OPEN_MPI)
  add_definitions(-DUSE_PARALLEL_PROG)
  set(CMAKE_CXX_COMPILER mpic++)
  if (WIN32)
    add_compile_options(/W4 /Od)
  else()
    add_compile_options(-Wall -O0)
  endif()
else()
  if (WIN32)
    add_compile_options(/W4 /Od)
  else()
    add_compile_options(-Wall -O0)
  endif()
endif()

set(SOURCES
    main.cpp
    BFGSolver.cpp
    IFunction.cpp
    PowellSingularFunction.cpp
    )

add_compile_definitions(PARALLEL_PROG)
add_executable(${CMAKE_PROJECT_NAME} ${SOURCES} ${HEADERS})
target_link_libraries(${CMAKE_PROJECT_NAME} math)
