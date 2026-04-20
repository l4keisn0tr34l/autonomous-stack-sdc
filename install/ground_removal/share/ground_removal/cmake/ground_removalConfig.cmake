# generated from ament/cmake/core/templates/nameConfig.cmake.in

# prevent multiple inclusion
if(_ground_removal_CONFIG_INCLUDED)
  # ensure to keep the found flag the same
  if(NOT DEFINED ground_removal_FOUND)
    # explicitly set it to FALSE, otherwise CMake will set it to TRUE
    set(ground_removal_FOUND FALSE)
  elseif(NOT ground_removal_FOUND)
    # use separate condition to avoid uninitialized variable warning
    set(ground_removal_FOUND FALSE)
  endif()
  return()
endif()
set(_ground_removal_CONFIG_INCLUDED TRUE)

# output package information
if(NOT ground_removal_FIND_QUIETLY)
  message(STATUS "Found ground_removal: 0.0.0 (${ground_removal_DIR})")
endif()

# warn when using a deprecated package
if(NOT "" STREQUAL "")
  set(_msg "Package 'ground_removal' is deprecated")
  # append custom deprecation text if available
  if(NOT "" STREQUAL "TRUE")
    set(_msg "${_msg} ()")
  endif()
  # optionally quiet the deprecation message
  if(NOT ${ground_removal_DEPRECATED_QUIET})
    message(DEPRECATION "${_msg}")
  endif()
endif()

# flag package as ament-based to distinguish it after being find_package()-ed
set(ground_removal_FOUND_AMENT_PACKAGE TRUE)

# include all config extra files
set(_extras "")
foreach(_extra ${_extras})
  include("${ground_removal_DIR}/${_extra}")
endforeach()
