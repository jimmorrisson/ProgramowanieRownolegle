
set (CMAKE_CXX_STANDARD 17)
add_library(math matrix.h vector.h matrix.cpp vector.cpp)
target_include_directories(math PUBLIC .)