cmake_minimum_required (VERSION 3.12)
project (TonUINO-TNG)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

enable_testing()
find_package(GTest REQUIRED)
include(GoogleTest)

add_subdirectory (test)
