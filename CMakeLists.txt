cmake_minimum_required(VERSION 3.1.0)
project(PBRT)

#sanitizers-cmake
set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/ext/sanitizers-cmake/cmake" ${CMAKE_MODULE_PATH})
find_package(Sanitizers)
find_package ( Threads )



####################### extra lib ############################
# Google logging Library
SET(WITH_GFLAGS OFF CACHE BOOL "Use gflags")
SET(BUILD_SHARED_LIBS OFF CACHE BOOL " " FORCE)
IF(WIN32)
  ADD_DEFINITIONS( -D GOOGLE_GLOG_DLL_DECL= )
ENDIF()
add_subdirectory(ext/glog)
SET_PROPERTY(TARGET glog logging_unittest demangle_unittest utilities_unittest stl_logging_unittest PROPERTY FOLDER "ext")
INCLUDE_DIRECTORIES (
  ext/glog/src
  ${CMAKE_BINARY_DIR}/ext/glog
)

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
  SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=gnu++17")
  SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-conversion-null")
ELSEIF(CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
  SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17")
  SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-deprecated-register")
ELSEIF(CMAKE_CXX_COMPILER_ID STREQUAL "Intel")
  SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17")
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

CHECK_CXX_SOURCE_COMPILES (
  "int main() { constexpr int x = 0; }"
  HAVE_CONSTEXPR )
IF ( HAVE_CONSTEXPR )
  ADD_DEFINITIONS ( -D PBRT_HAVE_CONSTEXPR )
  ADD_DEFINITIONS ( -D PBRT_CONSTEXPR=constexpr )
ELSE ()
  ADD_DEFINITIONS ( -D PBRT_CONSTEXPR=const )
ENDIF ()

# Aligned memory allocation

CHECK_CXX_SOURCE_COMPILES ( "
#include <malloc.h>
int main() { void * ptr = _aligned_malloc(1024, 32); }
" HAVE__ALIGNED_MALLOC )

CHECK_CXX_SOURCE_COMPILES ( "
#include <stdlib.h>
int main() {
  void *ptr;
  posix_memalign(&ptr, 32, 1024);
} " HAVE_POSIX_MEMALIGN )

CHECK_CXX_SOURCE_COMPILES ( "
#include <malloc.h>
int main() {
    void *ptr = memalign(32, 1024);
} " HAVE_MEMALIGN )

IF ( HAVE__ALIGNED_MALLOC )
  ADD_DEFINITIONS ( -D PBRT_HAVE__ALIGNED_MALLOC )
ELSEIF ( HAVE_POSIX_MEMALIGN )
  ADD_DEFINITIONS ( -D PBRT_HAVE_POSIX_MEMALIGN )
ELSEIF ( HAVE_MEMALIGN )
  ADD_DEFINITIONS ( -D PBRTHAVE_MEMALIGN )
ELSE ()
  MESSAGE ( SEND_ERROR "Unable to find a way to allocate aligned memory" )
ENDIF ()

# thread-local variables

CHECK_CXX_SOURCE_COMPILES ( "
#ifdef __CYGWIN__
// Hack to work around https://gcc.gnu.org/bugzilla/show_bug.cgi?id=64697
#error \"No thread_local on cygwin\"
#endif  // __CYGWIN__
thread_local int x; int main() { }
" HAVE_THREAD_LOCAL )

CHECK_CXX_SOURCE_COMPILES ( "
__declspec(thread) int x; int main() { }
" HAVE_DECLSPEC_THREAD )

CHECK_CXX_SOURCE_COMPILES ( "
__thread int x; int main() { }
" HAVE___THREAD )

IF ( HAVE_THREAD_LOCAL )
  ADD_DEFINITIONS ( -D PBRT_THREAD_LOCAL=thread_local )
ELSEIF ( HAVE___THREAD )
  ADD_DEFINITIONS ( -D PBRT_THREAD_LOCAL=__thread )
ELSEIF ( HAVE_DECLSPEC_THREAD )
  ADD_DEFINITIONS ( -D "PBRT_THREAD_LOCAL=__declspec(thread)" )
ELSE ()
  MESSAGE ( SEND_ERROR "Unable to find a way to declare a thread-local variable")
ENDIF ()


####################### pbrt file ############################

file(GLOB PBRT_CORE_LIB
    src/core/*
)


file(GLOB PBRT_MINOR_LIB
    src/shape/*
    src/accelerators/*
    src/cameras/*
    src/lights/*
)

set(OBJ_FILE_LOADER ext/OBJ_Loader/OBJ_Loader.h)


INCLUDE_DIRECTORIES ( src/core )
INCLUDE_DIRECTORIES ( src )
include_directories ( ext/OBJ_Loader/)

# Visual Studio source folders
SOURCE_GROUP (core REGULAR_EXPRESSION src/core/.*)
SOURCE_GROUP (shape REGULAR_EXPRESSION src/shape/.*)

ADD_LIBRARY (pbrt STATIC
  ${PBRT_CORE_LIB}
  ${PBRT_MINOR_LIB}
  ${OBJ_FILE_LOADER}
)
ADD_SANITIZERS ( pbrt )

# A non-exhaustive but pretty representative set..
# Note that we work-around shoddy c++17 support in MSVC2013
# (constexpr, etc.), so don't test for that stuff here
SET (PBRT_CXX17_FEATURES
  cxx_auto_type
  cxx_explicit_conversions
  cxx_lambdas
  cxx_nullptr
  cxx_range_for
  cxx_static_assert
)
TARGET_COMPILE_FEATURES ( pbrt PRIVATE ${PBRT_CXX17_FEATURES} )

IF (WIN32)
  # Avoid a name clash when building on Visual Studio
  SET_TARGET_PROPERTIES ( pbrt PROPERTIES OUTPUT_NAME libpbrt )
ENDIF()

SET(ALL_PBRT_LIBS
  pbrt
  glog
  ${CMAKE_THREAD_LIBS_INIT}
)

# Main renderer
ADD_EXECUTABLE ( pbrt_exe src/main/pbrt.cpp )
ADD_SANITIZERS ( pbrt_exe )
SET_TARGET_PROPERTIES ( pbrt_exe PROPERTIES OUTPUT_NAME pbrt )
TARGET_COMPILE_FEATURES ( pbrt_exe PRIVATE ${PBRT_CXX17_FEATURES} )
TARGET_LINK_LIBRARIES ( pbrt_exe ${ALL_PBRT_LIBS} )

# Unit test
FILE (GLOB PBRT_TEST_SOURCE
  src/tests/*.cpp
  src/tests/*.h
  src/tests/gtest/*.cc
)
ENABLE_TESTING()
ADD_SANITIZERS ( pbrt_test )
ADD_EXECUTABLE ( pbrt_test ${PBRT_TEST_SOURCE} )
TARGET_COMPILE_FEATURES ( pbrt_test PRIVATE ${PBRT_CXX17_FEATURES} )
TARGET_LINK_LIBRARIES ( pbrt_test ${ALL_PBRT_LIBS} )
ADD_TEST ( pbrt_unit_test pbrt_test )


