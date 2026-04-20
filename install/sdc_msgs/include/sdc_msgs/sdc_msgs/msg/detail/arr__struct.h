// generated from rosidl_generator_c/resource/idl__struct.h.em
// with input from sdc_msgs:msg/Arr.idl
// generated code does not contain a copyright notice

#ifndef SDC_MSGS__MSG__DETAIL__ARR__STRUCT_H_
#define SDC_MSGS__MSG__DETAIL__ARR__STRUCT_H_

#ifdef __cplusplus
extern "C"
{
#endif

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>


// Constants defined in the message

// Include directives for member types
// Member 'data'
#include "rosidl_runtime_c/primitives_sequence.h"

/// Struct defined in msg/Arr in the package sdc_msgs.
typedef struct sdc_msgs__msg__Arr
{
  rosidl_runtime_c__float__Sequence data;
} sdc_msgs__msg__Arr;

// Struct for a sequence of sdc_msgs__msg__Arr.
typedef struct sdc_msgs__msg__Arr__Sequence
{
  sdc_msgs__msg__Arr * data;
  /// The number of valid items in data
  size_t size;
  /// The number of allocated items in data
  size_t capacity;
} sdc_msgs__msg__Arr__Sequence;

#ifdef __cplusplus
}
#endif

#endif  // SDC_MSGS__MSG__DETAIL__ARR__STRUCT_H_
