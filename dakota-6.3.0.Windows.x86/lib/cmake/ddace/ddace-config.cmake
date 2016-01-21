get_filename_component(_CURDIR "${CMAKE_CURRENT_LIST_FILE}" PATH)
include(${_CURDIR}/ddace-targets.cmake)

# Compute the installation prefix relative to this file.
get_filename_component(_PREFIX "${_CURDIR}" PATH)
get_filename_component(_PREFIX "${_PREFIX}" PATH)
get_filename_component(_PREFIX "${_PREFIX}" PATH)

set(DDACE_INCLUDE_DIRS ${_PREFIX}/include/ddace)
