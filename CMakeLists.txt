cmake_minimum_required(VERSION 3.10)

# set the project name
project(CZ4031)

# specify the C++ standard
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

file(GLOB src CONFIGURE_DEPENDS "*.h" "*.cpp")

# add the executable
add_executable(test ${src})