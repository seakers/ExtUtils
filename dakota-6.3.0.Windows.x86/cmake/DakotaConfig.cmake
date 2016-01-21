# - Config file for the Dakota package
# It defines the following variables
#  Dakota_INCLUDE_DIRS - include directories for Dakota
#  Dakota_LIBRARIES    - libraries to link against
#  Dakota_EXECUTABLE   - the dakota executable

# Compute paths
get_filename_component(DAKOTA_CMAKE_DIR "${CMAKE_CURRENT_LIST_FILE}" PATH)

# Our library dependencies (contains definitions for IMPORTED targets)
include("${DAKOTA_CMAKE_DIR}/DakotaTargets.cmake")


####### Expanded from @PACKAGE_INIT@ by configure_package_config_file() #######
####### Any changes to this file will be overwritten by the next CMake run ####
####### The input file was DakotaConfig.cmake.in                            ########

get_filename_component(PACKAGE_PREFIX_DIR "${CMAKE_CURRENT_LIST_DIR}/../" ABSOLUTE)

macro(set_and_check _var _file)
  set(${_var} "${_file}")
  if(NOT EXISTS "${_file}")
    message(FATAL_ERROR "File or directory ${_file} referenced by variable ${_var} does not exist !")
  endif()
endmacro()

macro(check_required_components _NAME)
  foreach(comp ${${_NAME}_FIND_COMPONENTS})
    if(NOT ${_NAME}_${comp}_FOUND)
      if(${_NAME}_FIND_REQUIRED_${comp})
        set(${_NAME}_FOUND FALSE)
      endif()
    endif()
  endforeach()
endmacro()

####################################################################################

# Build tree option not yet working
if (Dakota_BUILD_TREE)
  # Dakota's build tree doesn't have a single convenient place for headers
  set(Dakota_INCLUDE_DIRS_TMP "C:/jenkins/workspace/dakota_release_build_public_windows/build;C:/jenkins/workspace/dakota_release_build_public_windows/build/generated;C:/jenkins/workspace/dakota_release_build_public_windows/build/src;c:/jenkins/workspace/dakota_release_build_public_windows/source/src;c:/jenkins/workspace/dakota_release_build_public_windows/source/packages;C:/jenkins/workspace/dakota_release_build_public_windows/build/packages/teuchos;c:/jenkins/workspace/dakota_release_build_public_windows/source/packages/teuchos/cmake/tribits/common_tools/win_interface/include;C:/jenkins/workspace/dakota_release_build_public_windows/build/packages/teuchos/.;C:/jenkins/workspace/dakota_release_build_public_windows/build/packages/teuchos/packages/teuchos/src;c:/jenkins/workspace/dakota_release_build_public_windows/source/packages/teuchos/packages/teuchos/src;C:/jenkins/workspace/dakota_release_build_public_windows/build/packages/pecos;c:/jenkins/workspace/dakota_release_build_public_windows/source/packages/pecos/src;c:/jenkins/workspace/dakota_release_build_public_windows/source/packages/VPISparseGrid/src;C:/jenkins/workspace/dakota_release_build_public_windows/build/packages/surfpack;c:/jenkins/workspace/dakota_release_build_public_windows/source/packages/surfpack/src;C:/jenkins/workspace/dakota_release_build_public_windows/build/packages/acro/packages;C:/jenkins/workspace/dakota_release_build_public_windows/build/packages/acro/packages/utilib;c:/jenkins/workspace/dakota_release_build_public_windows/source/packages/acro/packages/interfaces/src;c:/jenkins/workspace/dakota_release_build_public_windows/source/packages/acro/packages/scolib/src;c:/jenkins/workspace/dakota_release_build_public_windows/source/packages/acro/packages/utilib/src;c:/jenkins/workspace/dakota_release_build_public_windows/source/packages/acro/packages/colin/src;c:/jenkins/workspace/dakota_release_build_public_windows/source/packages/acro/packages/pebbl/src/bb;c:/jenkins/workspace/dakota_release_build_public_windows/source/packages/acro/packages/pebbl/src/comm;c:/jenkins/workspace/dakota_release_build_public_windows/source/packages/acro/packages/pebbl/src/misc;c:/jenkins/workspace/dakota_release_build_public_windows/source/packages/acro/packages/pebbl/src/pbb;c:/jenkins/workspace/dakota_release_build_public_windows/source/packages/acro/packages/pebbl/src/sched;c:/jenkins/workspace/dakota_release_build_public_windows/source/packages/acro/tpl/tinyxml;C:/jenkins/workspace/dakota_release_build_public_windows/build/packages/DDACE;c:/jenkins/workspace/dakota_release_build_public_windows/source/packages/DDACE/include;c:/jenkins/workspace/dakota_release_build_public_windows/source/packages/dream;C:/jenkins/workspace/dakota_release_build_public_windows/build/packages/FSUDace;c:/jenkins/workspace/dakota_release_build_public_windows/source/packages/FSUDace;C:/jenkins/workspace/dakota_release_build_public_windows/build/packages/hopspack/src/src-shared;c:/jenkins/workspace/dakota_release_build_public_windows/source/packages/hopspack/src/src-shared;c:/jenkins/workspace/dakota_release_build_public_windows/source/packages/hopspack/src/src-citizens;c:/jenkins/workspace/dakota_release_build_public_windows/source/packages/hopspack/src/src-evaluator;c:/jenkins/workspace/dakota_release_build_public_windows/source/packages/hopspack/src/src-executor;c:/jenkins/workspace/dakota_release_build_public_windows/source/packages/hopspack/src/src-framework;c:/jenkins/workspace/dakota_release_build_public_windows/source/packages/hopspack/src/src-main;C:/jenkins/workspace/dakota_release_build_public_windows/build/packages/JEGA;C:/jenkins/workspace/dakota_release_build_public_windows/build/packages/JEGA/eddy;c:/jenkins/workspace/dakota_release_build_public_windows/source/packages/JEGA/eddy;c:/jenkins/workspace/dakota_release_build_public_windows/source/packages/JEGA/include;C:/jenkins/workspace/dakota_release_build_public_windows/build/packages/NOMAD/src;c:/jenkins/workspace/dakota_release_build_public_windows/source/packages/NOMAD/src;C:/jenkins/workspace/dakota_release_build_public_windows/build/packages/OPTPP/include;c:/jenkins/workspace/dakota_release_build_public_windows/source/packages/OPTPP/include;c:/jenkins/workspace/dakota_release_build_public_windows/source/packages/PSUADE;C:/jenkins/workspace/dakota_release_build_public_windows/build/packages/ampl")
  # This won't work yet.  Since Dakota is in a build tree the
  # libraries are scattered, but the import doesn't know where they
  # are.
  set(Dakota_LIBRARY_DIRS_TMP "C:/jenkins/workspace/dakota_release_build_public_windows/build/packages/teuchos/packages/teuchos/src")
else()
  set_and_check(Dakota_INCLUDE_DIRS_TMP "${PACKAGE_PREFIX_DIR}/include")
  set_and_check(Dakota_LIB_DIRS_TMP "${PACKAGE_PREFIX_DIR}/lib")
endif()

# Variables intended for use by other projects

set_and_check(Dakota_DIR ${DAKOTA_CMAKE_DIR})

# TODO: reference in src vs bin
set(Dakota_EXECUTABLE dakota)


set(Dakota_DEFINES "_CRT_SECURE_NO_DEPRECATE;_CRT_NONSTDC_NO_DEPRECATE;_CRT_SECURE_NO_WARNINGS;_SCL_SECURE_NO_DEPRECATE;_SCL_SECURE_NO_WARNINGS;HAVE_CONFIG_H;BOOST_SIGNALS_NO_DEPRECATION_WARNING;HAVE_CONFIG_H;DISABLE_DAKOTA_CONFIG_H;BOOST_DISABLE_ASSERTS;HAVE_SYSTEM;DAKOTA_F90;HAVE_PECOS;HAVE_SURFPACK;DAKOTA_COLINY;DAKOTA_UTILIB;HAVE_ADAPTIVE_SAMPLING;HAVE_ESM;HAVE_CONMIN;HAVE_DDACE;HAVE_DREAM;HAVE_FSUDACE;DAKOTA_HOPS;HAVE_JEGA;HAVE_NCSU;HAVE_NL2SOL;HAVE_NOMAD;HAVE_OPTPP;DAKOTA_OPTPP;HAVE_PSUADE;HAVE_AMPL")

set(Dakota_INCLUDE_DIRS ${Dakota_INCLUDE_DIRS_TMP})

set(Dakota_LIBRARY_DIRS ${Dakota_LIB_DIRS_TMP})

# These are IMPORTED targets created by DakotaTargets.cmake
set(Dakota_LIBRARIES "dakota_src;dakota_src_fortran;nidr;teuchos;pecos;pecos_src;lhs;lhs_mods;lhs_mod;dfftpack;sparsegrid;surfpack;surfpack;surfpack_fortran;utilib;colin;interfaces;scolib;3po;pebbl;tinyxml;conmin;ddace;dream;fsudace;hopspack;jega;jega_fe;moga;soga;eutils;utilities;ncsuopt;cport;nomad;optpp;psuade;amplsolver")

# Not built in Dakota's CMake process
set(Dakota_EXTPROJ_LIBRARIES "")

# TPLs external to the build
set(Dakota_TPL_INCLUDE_DIRS "C:/sems/boost/1.54.0/include/boost-1_54;C:/sems/boost/1.54.0/include/boost-1_54")

set(Dakota_TPL_LIBRARY_DIRS "C:/sems/boost/1.54.0/lib")

set(Dakota_TPL_LIBRARIES "optimized;C:/sems/boost/1.54.0/lib/libboost_filesystem-vc100-mt-1_54.lib;debug;C:/sems/boost/1.54.0/lib/libboost_filesystem-vc100-mt-gd-1_54.lib;optimized;C:/sems/boost/1.54.0/lib/libboost_program_options-vc100-mt-1_54.lib;debug;C:/sems/boost/1.54.0/lib/libboost_program_options-vc100-mt-gd-1_54.lib;optimized;C:/sems/boost/1.54.0/lib/libboost_regex-vc100-mt-1_54.lib;debug;C:/sems/boost/1.54.0/lib/libboost_regex-vc100-mt-gd-1_54.lib;optimized;C:/sems/boost/1.54.0/lib/libboost_serialization-vc100-mt-1_54.lib;debug;C:/sems/boost/1.54.0/lib/libboost_serialization-vc100-mt-gd-1_54.lib;optimized;C:/sems/boost/1.54.0/lib/libboost_system-vc100-mt-1_54.lib;debug;C:/sems/boost/1.54.0/lib/libboost_system-vc100-mt-gd-1_54.lib;optimized;C:/sems/boost/1.54.0/lib/libboost_signals-vc100-mt-1_54.lib;debug;C:/sems/boost/1.54.0/lib/libboost_signals-vc100-mt-gd-1_54.lib;lapack;blas")
