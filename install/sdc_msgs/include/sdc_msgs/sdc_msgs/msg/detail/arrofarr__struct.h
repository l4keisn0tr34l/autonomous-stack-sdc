// generated from rosidl_generator_c/resource/idl__struct.h.em
// with input from sdc_msgs:msg/Arrofarr.idl
// generated code does not contain a copyright notice

#ifndef SDC_MSGS__MSG__DETAIL__ARROFARR__STRUCT_H_
#define SDC_MSGS__MSG__DETAIL__ARROFARR__STRUCT_H_

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
// Member 'data'
#include "sdc_msgs/msg/detail/arr__struct.h"

/// Struct defined in msg/Arrofarr in the package sdc_msgs.
typedef struct sdc_msgs__msg__Arrofarr
{
  std_msgs__msg__Header header;
  sdc_msgs__msg__Arr__Sequence data;
} sdc_msgs__msg__Arrofarr;

// Struct for a sequence of sdc_msgs__msg__Arrofarr.
typedef struct sdc_msgs__msg__Arrofarr__Sequence
{
  sdc_msgs__msg__Arrofarr * data;
  /// The number of valid items in data
  size_t size;
  /// The number of allocated items in data
  size_t capacity;
} sdc_msgs__msg__Arrofarr__Sequence;

#ifdef __cplusplus
}
#endif

#endif  // SDC_MSGS__MSG__DETAIL__ARROFARR__STRUCT_H_
