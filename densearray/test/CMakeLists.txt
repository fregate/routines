cmake_minimum_required(VERSION 3.14)

project(densearrayTests LANGUAGES CXX)

include(../cmake/project-is-top-level.cmake)
include(../cmake/folders.cmake)
include(../cmake/windows-set-path.cmake)

# ---- Dependencies ----

if(PROJECT_IS_TOP_LEVEL)
  find_package(densearray REQUIRED)
  enable_testing()
endif()

# ---- Tests ----

add_executable(densearray_test source/densearray_test.cpp)
target_link_libraries(densearray_test PRIVATE densearray::densearray)
target_compile_features(densearray_test PRIVATE cxx_std_17)

add_test(NAME densearray_test COMMAND densearray_test)

# ---- End-of-file commands ----

add_folders(Test)
