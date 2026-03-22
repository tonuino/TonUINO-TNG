cmake_minimum_required (VERSION 3.12)
project (TonUINO-TNG)
include(FetchContent)
FetchContent_Declare(
        googletest
        GIT_REPOSITORY https://github.com/google/googletest.git
        GIT_TAG        52eb8108c5bdec04579160ae17225d66034bd723 # release-1.17.0
)
FetchContent_MakeAvailable(googletest)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

enable_testing()
include(GoogleTest)

add_subdirectory (test)
