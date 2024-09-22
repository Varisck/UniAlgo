cmake_minimum_required(VERSION 3.14)
project(unialgo)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/build)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_TESTS ${CMAKE_BINARY_DIR}/tests)

option(DEFINE_CI_MACRO_ENV "Compile cmake for ci/cd pipeline" OFF)

if(DEFINE_CI_MACRO_ENV)
  add_compile_definitions(UNIALGO_CI_ENVIRONMENT_)
endif(DEFINE_CI_MACRO_ENV)


#add_compile_options(/I ${CMAKE_SOURCE_DIR}/)
include_directories("${PROJECT_SOURCE_DIR}/")

enable_testing()

add_subdirectory(unialgo)

include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)
