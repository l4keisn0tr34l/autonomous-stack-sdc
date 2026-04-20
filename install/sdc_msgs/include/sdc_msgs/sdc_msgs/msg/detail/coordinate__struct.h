// generated from rosidl_generator_c/resource/idl__struct.h.em
// with input from sdc_msgs:msg/Coordinate.idl
// generated code does not contain a copyright notice

#ifndef SDC_MSGS__MSG__DETAIL__COORDINATE__STRUCT_H_
#define SDC_MSGS__MSG__DETAIL__COORDINATE__STRUCT_H_

#ifdef __cplusplus
extern "C"
{
#endif

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>


// Constants defined in the message

// Include directives for member types
// Member 'left'
// Member 'right'
// Member 'front'
// Member 'back'
// Member 'top'
// Member 'bottom'
#include "rosidl_runtime_c/primitives_sequence.h"

/// Struct defined in msg/Coordinate in the package sdc_msgs.
typedef struct sdc_msgs__msg__Coordinate
{
  uint8_t size;
  uint8_t reconsize;
  float x;
  float y;
  float z;
  float colour;
  rosidl_runtime_c__float__Sequence left;
  rosidl_runtime_c__float__Sequence right;
  rosidl_runtime_c__float__Sequence front;
  rosidl_runtime_c__float__Sequence back;
  rosidl_runtime_c__float__Sequence top;
  rosidl_runtime_c__float__Sequence bottom;
} sdc_msgs__msg__Coordinate;

// Struct for a sequence of sdc_msgs__msg__Coordinate.
typedef struct sdc_msgs__msg__Coordinate__Sequence
{
  sdc_msgs__msg__Coordinate * data;
  /// The number of valid items in data
  size_t size;
  /// The number of allocated items in data
  size_t capacity;
} sdc_msgs__msg__Coordinate__Sequence;

#ifdef __cplusplus
}
#endif

#endif  // SDC_MSGS__MSG__DETAIL__COORDINATE__STRUCT_H_
