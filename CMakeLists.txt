cmake_minimum_required(VERSION 3.1.0)
project(PBRT)

file(GLOB PBRT_SOURCES
    "src/core/*"
)

INCLUDE_DIRECTORIES ( src/core )
INCLUDE_DIRECTORIES ( src )

add_executable(pbrt_exe src/main/pbrt.cpp)


####################### extra lib ############################
# Google logging Library
find_package (glog 0.6.0 REQUIRED)
set(GLOG_DIR "ext/glog")
add_subdirectory("${GLOG_DIR}")
target_link_libraries (pbrt_exe glog::glog)

####################### global micro ############################
# DEBUG OR RELEASE
if (NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
  message(STATUS "Setting build type to 'Release' as none was specified.")
  set(CMAKE_BUILD_TYPE Release CACHE STRING "Choose the type of build." FORCE)
  set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS "Debug" "Release"
    "MinSizeRel" "RelWithDebInfo")
endif()
if(CMAKE_BUILD_TYPE MATCHES RELEASE) 
    add_definitions(-DNDEBUG)
endif()

# FLOAT OR DOUBLE
option(PBRT_FLOAT_AS_DOUBLE "Use 64-bit floats" OFF)
if(PBRT_FLOAT_AS_DOUBLE)
    add_definitions(-D PBRT_FLOAT_AS_DOUBLE)
endif()


