cmake_minimum_required(VERSION 3.14)

project(intervalTests LANGUAGES CXX)

include(../cmake/project-is-top-level.cmake)
include(../cmake/folders.cmake)
include(../cmake/windows-set-path.cmake)

# ---- Dependencies ----

if(PROJECT_IS_TOP_LEVEL)
  find_package(interval REQUIRED)
  enable_testing()
endif()

# ---- Tests ----

add_executable(interval_test source/interval_test.cpp)
target_link_libraries(interval_test PRIVATE interval::interval)
target_compile_features(interval_test PRIVATE cxx_std_17)

add_test(NAME interval_test COMMAND interval_test)

# ---- End-of-file commands ----

add_folders(Test)
