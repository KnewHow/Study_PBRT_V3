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

####################### compiler params ############################
# Annoying compiler-specific details
IF(CMAKE_COMPILER_IS_GNUCXX)
  SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=gnu++11")
  SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-conversion-null")
ELSEIF(CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
  SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
  SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-deprecated-register")
ELSEIF(CMAKE_CXX_COMPILER_ID STREQUAL "Intel")
  SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
  FIND_PROGRAM(XIAR xiar)
  IF(XIAR)
    SET(CMAKE_AR "${XIAR}")
  ENDIF(XIAR)
  MARK_AS_ADVANCED(XIAR)

  FIND_PROGRAM(XILD xild)
  IF(XILD)
    SET(CMAKE_LINKER "${XILD}")
  ENDIF(XILD)
  MARK_AS_ADVANCED(XILD)

  # ICC will default to -fp-model fast=1, which performs value-unsafe optimizations which will
  # cause pbrt_test to fail. For safety, -fp-model precise is explicitly set here by default.
  set(FP_MODEL "precise" CACHE STRING "The floating point model to compile with.")
  set_property(CACHE FP_MODEL PROPERTY STRINGS "precise" "fast=1" "fast=2")

  SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fp-model ${FP_MODEL}")
ENDIF()

# ADD_SANITIZERS ( pbrt )

# SET ( PBRT_CXX11_FEATURES
#   cxx_auto_type
#   cxx_explicit_conversions
#   cxx_lambdas
#   cxx_nullptr
#   cxx_range_for
#   cxx_static_assert
# )
# TARGET_COMPILE_FEATURES ( pbrt PRIVATE ${PBRT_CXX11_FEATURES} )
