# generated from ament/cmake/core/templates/nameConfig.cmake.in

# prevent multiple inclusion
if(_clustering_CONFIG_INCLUDED)
  # ensure to keep the found flag the same
  if(NOT DEFINED clustering_FOUND)
    # explicitly set it to FALSE, otherwise CMake will set it to TRUE
    set(clustering_FOUND FALSE)
  elseif(NOT clustering_FOUND)
    # use separate condition to avoid uninitialized variable warning
    set(clustering_FOUND FALSE)
  endif()
  return()
endif()
set(_clustering_CONFIG_INCLUDED TRUE)

# output package information
if(NOT clustering_FIND_QUIETLY)
  message(STATUS "Found clustering: 0.0.1 (${clustering_DIR})")
endif()

# warn when using a deprecated package
if(NOT "" STREQUAL "")
  set(_msg "Package 'clustering' is deprecated")
  # append custom deprecation text if available
  if(NOT "" STREQUAL "TRUE")
    set(_msg "${_msg} ()")
  endif()
  # optionally quiet the deprecation message
  if(NOT ${clustering_DEPRECATED_QUIET})
    message(DEPRECATION "${_msg}")
  endif()
endif()

# flag package as ament-based to distinguish it after being find_package()-ed
set(clustering_FOUND_AMENT_PACKAGE TRUE)

# include all config extra files
set(_extras "")
foreach(_extra ${_extras})
  include("${clustering_DIR}/${_extra}")
endforeach()
