if (NOT EXISTS ${CMAKE_CURRENT_BINARY_DIR}/cmake/CPM.cmake)
# TODO make available download specific version like:
# https://github.com/cpm-cmake/CPM.cmake/releases/download/v0.40.0/CPM.cmake
# add variable, like, USE_CPM_VERSION and check it. If empty - use 'releases/latest/download/', in otherwise use 'releases/download/${USE_CPM_VERSION}/'
	file(
		DOWNLOAD https://github.com/cpm-cmake/CPM.cmake/releases/latest/download/CPM.cmake
		${CMAKE_CURRENT_BINARY_DIR}/cmake/CPM.cmake
	)
endif()
include(${CMAKE_CURRENT_BINARY_DIR}/cmake/CPM.cmake)
