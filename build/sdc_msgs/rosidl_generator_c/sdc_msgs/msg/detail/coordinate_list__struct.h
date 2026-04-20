// generated from rosidl_generator_c/resource/idl__struct.h.em
// with input from sdc_msgs:msg/CoordinateList.idl
// generated code does not contain a copyright notice

#ifndef SDC_MSGS__MSG__DETAIL__COORDINATE_LIST__STRUCT_H_
#define SDC_MSGS__MSG__DETAIL__COORDINATE_LIST__STRUCT_H_

#ifdef __cplusplus
extern "C"
{
#endif

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>


// Constants defined in the message

// Include directives for member types
// Member 'header'
#include "std_msgs/msg/detail/header__struct.h"
// Member 'cone_coordinates'
#include "sdc_msgs/msg/detail/coordinate__struct.h"

/// Struct defined in msg/CoordinateList in the package sdc_msgs.
typedef struct sdc_msgs__msg__CoordinateList
{
  std_msgs__msg__Header header;
  uint8_t size;
  sdc_msgs__msg__Coordinate__Sequence cone_coordinates;
} sdc_msgs__msg__CoordinateList;

// Struct for a sequence of sdc_msgs__msg__CoordinateList.
typedef struct sdc_msgs__msg__CoordinateList__Sequence
{
  sdc_msgs__msg__CoordinateList * data;
  /// The number of valid items in data
  size_t size;
  /// The number of allocated items in data
  size_t capacity;
} sdc_msgs__msg__CoordinateList__Sequence;

#ifdef __cplusplus
}
#endif

#endif  // SDC_MSGS__MSG__DETAIL__COORDINATE_LIST__STRUCT_H_
