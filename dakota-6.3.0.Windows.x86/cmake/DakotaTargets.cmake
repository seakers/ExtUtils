# Generated by CMake 2.8.11.2

if("${CMAKE_MAJOR_VERSION}.${CMAKE_MINOR_VERSION}" LESS 2.5)
   message(FATAL_ERROR "CMake >= 2.6.0 required")
endif()
cmake_policy(PUSH)
cmake_policy(VERSION 2.6)
#----------------------------------------------------------------
# Generated CMake target import file.
#----------------------------------------------------------------

# Commands may need to know the format version.
set(CMAKE_IMPORT_FILE_VERSION 1)

set(_targetsDefined)
set(_targetsNotDefined)
set(_expectedTargets)
foreach(_expectedTarget teuchos conmin ncsuopt nidr amplsolver optpp dfftpack lhs_mod lhs_mods lhs sparsegrid pecos_src pecos surfpack_interpreter surfpack surfpack_fortran 3po tinyxml utilib colin pebbl scolib interfaces ddace dream fsudace fsu_halton_standalone fsu_hammersley_standalone fsu_cvt_standalone fsu_latinize_standalone fsu_quality_standalone hopspack moga soga utilities jega_fe eutils jega cport nomad psuade dakota_src_fortran dakota_src)
  list(APPEND _expectedTargets ${_expectedTarget})
  if(NOT TARGET ${_expectedTarget})
    list(APPEND _targetsNotDefined ${_expectedTarget})
  endif()
  if(TARGET ${_expectedTarget})
    list(APPEND _targetsDefined ${_expectedTarget})
  endif()
endforeach()
if("${_targetsDefined}" STREQUAL "${_expectedTargets}")
  set(CMAKE_IMPORT_FILE_VERSION)
  cmake_policy(POP)
  return()
endif()
if(NOT "${_targetsDefined}" STREQUAL "")
  message(FATAL_ERROR "Some (but not all) targets in this export set were already defined.\nTargets Defined: ${_targetsDefined}\nTargets not yet defined: ${_targetsNotDefined}\n")
endif()
unset(_targetsDefined)
unset(_targetsNotDefined)
unset(_expectedTargets)


# Compute the installation prefix relative to this file.
get_filename_component(_IMPORT_PREFIX "${CMAKE_CURRENT_LIST_FILE}" PATH)
get_filename_component(_IMPORT_PREFIX "${_IMPORT_PREFIX}" PATH)

# Create imported target teuchos
add_library(teuchos STATIC IMPORTED)

# Create imported target conmin
add_library(conmin STATIC IMPORTED)

# Create imported target ncsuopt
add_library(ncsuopt STATIC IMPORTED)

# Create imported target nidr
add_library(nidr STATIC IMPORTED)

# Create imported target amplsolver
add_library(amplsolver STATIC IMPORTED)

# Create imported target optpp
add_library(optpp STATIC IMPORTED)

# Create imported target dfftpack
add_library(dfftpack STATIC IMPORTED)

# Create imported target lhs_mod
add_library(lhs_mod STATIC IMPORTED)

# Create imported target lhs_mods
add_library(lhs_mods STATIC IMPORTED)

# Create imported target lhs
add_library(lhs STATIC IMPORTED)

# Create imported target sparsegrid
add_library(sparsegrid STATIC IMPORTED)

# Create imported target pecos_src
add_library(pecos_src STATIC IMPORTED)

# Create imported target pecos
add_library(pecos STATIC IMPORTED)

# Create imported target surfpack_interpreter
add_library(surfpack_interpreter STATIC IMPORTED)

# Create imported target surfpack
add_library(surfpack STATIC IMPORTED)

# Create imported target surfpack_fortran
add_library(surfpack_fortran STATIC IMPORTED)

# Create imported target 3po
add_library(3po STATIC IMPORTED)

# Create imported target tinyxml
add_library(tinyxml STATIC IMPORTED)

# Create imported target utilib
add_library(utilib STATIC IMPORTED)

# Create imported target colin
add_library(colin STATIC IMPORTED)

# Create imported target pebbl
add_library(pebbl STATIC IMPORTED)

# Create imported target scolib
add_library(scolib STATIC IMPORTED)

# Create imported target interfaces
add_library(interfaces STATIC IMPORTED)

# Create imported target ddace
add_library(ddace STATIC IMPORTED)

# Create imported target dream
add_library(dream STATIC IMPORTED)

# Create imported target fsudace
add_library(fsudace STATIC IMPORTED)

# Create imported target fsu_halton_standalone
add_executable(fsu_halton_standalone IMPORTED)

# Create imported target fsu_hammersley_standalone
add_executable(fsu_hammersley_standalone IMPORTED)

# Create imported target fsu_cvt_standalone
add_executable(fsu_cvt_standalone IMPORTED)

# Create imported target fsu_latinize_standalone
add_executable(fsu_latinize_standalone IMPORTED)

# Create imported target fsu_quality_standalone
add_executable(fsu_quality_standalone IMPORTED)

# Create imported target hopspack
add_library(hopspack STATIC IMPORTED)

# Create imported target moga
add_library(moga STATIC IMPORTED)

# Create imported target soga
add_library(soga STATIC IMPORTED)

# Create imported target utilities
add_library(utilities STATIC IMPORTED)

# Create imported target jega_fe
add_library(jega_fe STATIC IMPORTED)

# Create imported target eutils
add_library(eutils STATIC IMPORTED)

# Create imported target jega
add_library(jega STATIC IMPORTED)

# Create imported target cport
add_library(cport STATIC IMPORTED)

# Create imported target nomad
add_library(nomad STATIC IMPORTED)

# Create imported target psuade
add_library(psuade STATIC IMPORTED)

# Create imported target dakota_src_fortran
add_library(dakota_src_fortran STATIC IMPORTED)

# Create imported target dakota_src
add_library(dakota_src STATIC IMPORTED)

# Load information for each installed configuration.
get_filename_component(_DIR "${CMAKE_CURRENT_LIST_FILE}" PATH)
file(GLOB CONFIG_FILES "${_DIR}/DakotaTargets-*.cmake")
foreach(f ${CONFIG_FILES})
  include(${f})
endforeach()

# Cleanup temporary variables.
set(_IMPORT_PREFIX)

# Loop over all imported files and verify that they actually exist
foreach(target ${_IMPORT_CHECK_TARGETS} )
  foreach(file ${_IMPORT_CHECK_FILES_FOR_${target}} )
    if(NOT EXISTS "${file}" )
      message(FATAL_ERROR "The imported target \"${target}\" references the file
   \"${file}\"
but this file does not exist.  Possible reasons include:
* The file was deleted, renamed, or moved to another location.
* An install or uninstall procedure did not complete successfully.
* The installation package was faulty and contained
   \"${CMAKE_CURRENT_LIST_FILE}\"
but not all the files it references.
")
    endif()
  endforeach()
  unset(_IMPORT_CHECK_FILES_FOR_${target})
endforeach()
unset(_IMPORT_CHECK_TARGETS)

# This file does not depend on other imported targets which have
# been exported from the same project but in a separate export set.

# Commands beyond this point should not need to know the version.
set(CMAKE_IMPORT_FILE_VERSION)
cmake_policy(POP)
