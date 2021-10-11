cmake_minimum_required(VERSION 3.0.0)
project(PBRT)

file(GLOB PBRT_SOURCES
    "src/core/pbrt.h"
)

# ADD_LIBRARY (PBRT STATIC
#   ${PBRT_SOURCES}
# )


add_executable(pbrt_exe src/main/pbrt.cpp)



#######################extra lib ############################
# Google logging Library
SET(WITH_GFLAGS OFF CACHE BOOL "Use gflags")
SET(BUILD_SHARED_LIBS OFF CACHE BOOL " " FORCE)
find_package (glog 0.6.0 REQUIRED)
set(GLOG_DIR "ext/glog")
add_subdirectory("${GLOG_DIR}")
target_link_libraries (pbrt_exe glog::glog)

