
cmake_minimum_required(VERSION 2.8)

set(INCL "${CMAKE_SOURCE_DIR}/include")

# add unit tests
enable_testing()
add_subdirectory(tests)
