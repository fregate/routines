cmake_minimum_required(VERSION 3.14)

project(buffexTests LANGUAGES CXX)

include(../cmake/project-is-top-level.cmake)
include(../cmake/folders.cmake)
include(../cmake/windows-set-path.cmake)

# ---- Dependencies ----

if(PROJECT_IS_TOP_LEVEL)
  find_package(buffex REQUIRED)
  enable_testing()
endif()

# ---- Tests ----

add_executable(buffex_test source/buffex_test.cpp)
target_link_libraries(buffex_test PRIVATE buffex::buffex)
target_compile_features(buffex_test PRIVATE cxx_std_17)

add_test(NAME buffex_test COMMAND buffex_test)

# ---- End-of-file commands ----

add_folders(Test)
