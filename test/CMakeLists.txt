
file(GLOB tonuino_sources ../src/*.cpp)
file(GLOB test_tonuino_sources src/*.cpp)
file(GLOB libs_tonuino_sources libs/*.cpp)

include_directories(../src libs)

function(build_and_run_tests config_name)
	add_library(lib_${config_name} ${tonuino_sources} ${libs_tonuino_sources})
	target_compile_definitions(lib_${config_name} PRIVATE ${ARGN})
	
	add_executable(test_${config_name} ${test_tonuino_sources})
	target_compile_definitions(test_${config_name} PRIVATE ${ARGN})
	target_link_libraries(test_${config_name} lib_${config_name} gtest gtest_main)
	gtest_discover_tests(test_${config_name} TEST_PREFIX ${config_name}:)
endfunction()

build_and_run_tests(tonuino_classic_three TonUINO_Classic)
build_and_run_tests(tonuino_classic_five  TonUINO_Classic FIVEBUTTONS )
build_and_run_tests(tonuino_AiO_plus      ALLinONE_Plus)
build_and_run_tests(tonuino_AiO           ALLinONE)

