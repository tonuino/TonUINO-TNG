
file(GLOB trovalibre_sources ../src/*.cpp)
file(GLOB test_trovalibre_sources src/*.cpp)
file(GLOB libs_trovalibre_sources libs/*.cpp)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -DUNIT_TESTS")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -g -DUNIT_TESTS")

include_directories(../src libs)

function(build_and_run_tests config_name)
	add_library(lib_${config_name} ${trovalibre_sources} ${libs_trovalibre_sources})
	target_compile_definitions(lib_${config_name} PRIVATE ${ARGN})
	
	add_executable(test_${config_name} ${test_trovalibre_sources})
	target_compile_definitions(test_${config_name} PRIVATE ${ARGN})
	target_link_libraries(test_${config_name} lib_${config_name} gtest gtest_main)
	gtest_discover_tests(test_${config_name} TEST_PREFIX ${config_name}:)
endfunction()

build_and_run_tests(trovaLibre_classic_three TROVALIBRE_CLASSIC            )
build_and_run_tests(trovaLibre_classic_five  TROVALIBRE_CLASSIC FIVEBUTTONS)
build_and_run_tests(trovaLibre_classic_3x3   TROVALIBRE_CLASSIC BUTTONS3X3 )
build_and_run_tests(trovaLibre_AiO_plus      ALLinONE_Plus              )
build_and_run_tests(trovaLibre_AiO_plus_3x3  ALLinONE_Plus BUTTONS3X3   )
build_and_run_tests(trovaLibre_AiO           ALLinONE                   )
build_and_run_tests(trovaLibre_AiO_3x3       ALLinONE BUTTONS3X3        )

