// generated from rosidl_typesupport_introspection_c/resource/idl__type_support.c.em
// with input from sdc_msgs:msg/Coordinate.idl
// generated code does not contain a copyright notice

#include <stddef.h>
#include "sdc_msgs/msg/detail/coordinate__rosidl_typesupport_introspection_c.h"
#include "sdc_msgs/msg/rosidl_typesupport_introspection_c__visibility_control.h"
#include "rosidl_typesupport_introspection_c/field_types.h"
#include "rosidl_typesupport_introspection_c/identifier.h"
#include "rosidl_typesupport_introspection_c/message_introspection.h"
#include "sdc_msgs/msg/detail/coordinate__functions.h"
#include "sdc_msgs/msg/detail/coordinate__struct.h"


// Include directives for member types
// Member `left`
// Member `right`
// Member `front`
// Member `back`
// Member `top`
// Member `bottom`
#include "rosidl_runtime_c/primitives_sequence_functions.h"

#ifdef __cplusplus
extern "C"
{
#endif

void sdc_msgs__msg__Coordinate__rosidl_typesupport_introspection_c__Coordinate_init_function(
  void * message_memory, enum rosidl_runtime_c__message_initialization _init)
{
  // TODO(karsten1987): initializers are not yet implemented for typesupport c
  // see https://github.com/ros2/ros2/issues/397
  (void) _init;
  sdc_msgs__msg__Coordinate__init(message_memory);
}

void sdc_msgs__msg__Coordinate__rosidl_typesupport_introspection_c__Coordinate_fini_function(void * message_memory)
{
  sdc_msgs__msg__Coordinate__fini(message_memory);
}

size_t sdc_msgs__msg__Coordinate__rosidl_typesupport_introspection_c__size_function__Coordinate__left(
  const void * untyped_member)
{
  const rosidl_runtime_c__float__Sequence * member =
    (const rosidl_runtime_c__float__Sequence *)(untyped_member);
  return member->size;
}

const void * sdc_msgs__msg__Coordinate__rosidl_typesupport_introspection_c__get_const_function__Coordinate__left(
  const void * untyped_member, size_t index)
{
  const rosidl_runtime_c__float__Sequence * member =
    (const rosidl_runtime_c__float__Sequence *)(untyped_member);
  return &member->data[index];
}

void * sdc_msgs__msg__Coordinate__rosidl_typesupport_introspection_c__get_function__Coordinate__left(
  void * untyped_member, size_t index)
{
  rosidl_runtime_c__float__Sequence * member =
    (rosidl_runtime_c__float__Sequence *)(untyped_member);
  return &member->data[index];
}

void sdc_msgs__msg__Coordinate__rosidl_typesupport_introspection_c__fetch_function__Coordinate__left(
  const void * untyped_member, size_t index, void * untyped_value)
{
  const float * item =
    ((const float *)
    sdc_msgs__msg__Coordinate__rosidl_typesupport_introspection_c__get_const_function__Coordinate__left(untyped_member, index));
  float * value =
    (float *)(untyped_value);
  *value = *item;
}

void sdc_msgs__msg__Coordinate__rosidl_typesupport_introspection_c__assign_function__Coordinate__left(
  void * untyped_member, size_t index, const void * untyped_value)
{
  float * item =
    ((float *)
    sdc_msgs__msg__Coordinate__rosidl_typesupport_introspection_c__get_function__Coordinate__left(untyped_member, index));
  const float * value =
    (const float *)(untyped_value);
  *item = *value;
}

bool sdc_msgs__msg__Coordinate__rosidl_typesupport_introspection_c__resize_function__Coordinate__left(
  void * untyped_member, size_t size)
{
  rosidl_runtime_c__float__Sequence * member =
    (rosidl_runtime_c__float__Sequence *)(untyped_member);
  rosidl_runtime_c__float__Sequence__fini(member);
  return rosidl_runtime_c__float__Sequence__init(member, size);
}

size_t sdc_msgs__msg__Coordinate__rosidl_typesupport_introspection_c__size_function__Coordinate__right(
  const void * untyped_member)
{
  const rosidl_runtime_c__float__Sequence * member =
    (const rosidl_runtime_c__float__Sequence *)(untyped_member);
  return member->size;
}

const void * sdc_msgs__msg__Coordinate__rosidl_typesupport_introspection_c__get_const_function__Coordinate__right(
  const void * untyped_member, size_t index)
{
  const rosidl_runtime_c__float__Sequence * member =
    (const rosidl_runtime_c__float__Sequence *)(untyped_member);
  return &member->data[index];
}

void * sdc_msgs__msg__Coordinate__rosidl_typesupport_introspection_c__get_function__Coordinate__right(
  void * untyped_member, size_t index)
{
  rosidl_runtime_c__float__Sequence * member =
    (rosidl_runtime_c__float__Sequence *)(untyped_member);
  return &member->data[index];
}

void sdc_msgs__msg__Coordinate__rosidl_typesupport_introspection_c__fetch_function__Coordinate__right(
  const void * untyped_member, size_t index, void * untyped_value)
{
  const float * item =
    ((const float *)
    sdc_msgs__msg__Coordinate__rosidl_typesupport_introspection_c__get_const_function__Coordinate__right(untyped_member, index));
  float * value =
    (float *)(untyped_value);
  *value = *item;
}

void sdc_msgs__msg__Coordinate__rosidl_typesupport_introspection_c__assign_function__Coordinate__right(
  void * untyped_member, size_t index, const void * untyped_value)
{
  float * item =
    ((float *)
    sdc_msgs__msg__Coordinate__rosidl_typesupport_introspection_c__get_function__Coordinate__right(untyped_member, index));
  const float * value =
    (const float *)(untyped_value);
  *item = *value;
}

bool sdc_msgs__msg__Coordinate__rosidl_typesupport_introspection_c__resize_function__Coordinate__right(
  void * untyped_member, size_t size)
{
  rosidl_runtime_c__float__Sequence * member =
    (rosidl_runtime_c__float__Sequence *)(untyped_member);
  rosidl_runtime_c__float__Sequence__fini(member);
  return rosidl_runtime_c__float__Sequence__init(member, size);
}

size_t sdc_msgs__msg__Coordinate__rosidl_typesupport_introspection_c__size_function__Coordinate__front(
  const void * untyped_member)
{
  const rosidl_runtime_c__float__Sequence * member =
    (const rosidl_runtime_c__float__Sequence *)(untyped_member);
  return member->size;
}

const void * sdc_msgs__msg__Coordinate__rosidl_typesupport_introspection_c__get_const_function__Coordinate__front(
  const void * untyped_member, size_t index)
{
  const rosidl_runtime_c__float__Sequence * member =
    (const rosidl_runtime_c__float__Sequence *)(untyped_member);
  return &member->data[index];
}

void * sdc_msgs__msg__Coordinate__rosidl_typesupport_introspection_c__get_function__Coordinate__front(
  void * untyped_member, size_t index)
{
  rosidl_runtime_c__float__Sequence * member =
    (rosidl_runtime_c__float__Sequence *)(untyped_member);
  return &member->data[index];
}

void sdc_msgs__msg__Coordinate__rosidl_typesupport_introspection_c__fetch_function__Coordinate__front(
  const void * untyped_member, size_t index, void * untyped_value)
{
  const float * item =
    ((const float *)
    sdc_msgs__msg__Coordinate__rosidl_typesupport_introspection_c__get_const_function__Coordinate__front(untyped_member, index));
  float * value =
    (float *)(untyped_value);
  *value = *item;
}

void sdc_msgs__msg__Coordinate__rosidl_typesupport_introspection_c__assign_function__Coordinate__front(
  void * untyped_member, size_t index, const void * untyped_value)
{
  float * item =
    ((float *)
    sdc_msgs__msg__Coordinate__rosidl_typesupport_introspection_c__get_function__Coordinate__front(untyped_member, index));
  const float * value =
    (const float *)(untyped_value);
  *item = *value;
}

bool sdc_msgs__msg__Coordinate__rosidl_typesupport_introspection_c__resize_function__Coordinate__front(
  void * untyped_member, size_t size)
{
  rosidl_runtime_c__float__Sequence * member =
    (rosidl_runtime_c__float__Sequence *)(untyped_member);
  rosidl_runtime_c__float__Sequence__fini(member);
  return rosidl_runtime_c__float__Sequence__init(member, size);
}

size_t sdc_msgs__msg__Coordinate__rosidl_typesupport_introspection_c__size_function__Coordinate__back(
  const void * untyped_member)
{
  const rosidl_runtime_c__float__Sequence * member =
    (const rosidl_runtime_c__float__Sequence *)(untyped_member);
  return member->size;
}

const void * sdc_msgs__msg__Coordinate__rosidl_typesupport_introspection_c__get_const_function__Coordinate__back(
  const void * untyped_member, size_t index)
{
  const rosidl_runtime_c__float__Sequence * member =
    (const rosidl_runtime_c__float__Sequence *)(untyped_member);
  return &member->data[index];
}

void * sdc_msgs__msg__Coordinate__rosidl_typesupport_introspection_c__get_function__Coordinate__back(
  void * untyped_member, size_t index)
{
  rosidl_runtime_c__float__Sequence * member =
    (rosidl_runtime_c__float__Sequence *)(untyped_member);
  return &member->data[index];
}

void sdc_msgs__msg__Coordinate__rosidl_typesupport_introspection_c__fetch_function__Coordinate__back(
  const void * untyped_member, size_t index, void * untyped_value)
{
  const float * item =
    ((const float *)
    sdc_msgs__msg__Coordinate__rosidl_typesupport_introspection_c__get_const_function__Coordinate__back(untyped_member, index));
  float * value =
    (float *)(untyped_value);
  *value = *item;
}

void sdc_msgs__msg__Coordinate__rosidl_typesupport_introspection_c__assign_function__Coordinate__back(
  void * untyped_member, size_t index, const void * untyped_value)
{
  float * item =
    ((float *)
    sdc_msgs__msg__Coordinate__rosidl_typesupport_introspection_c__get_function__Coordinate__back(untyped_member, index));
  const float * value =
    (const float *)(untyped_value);
  *item = *value;
}

bool sdc_msgs__msg__Coordinate__rosidl_typesupport_introspection_c__resize_function__Coordinate__back(
  void * untyped_member, size_t size)
{
  rosidl_runtime_c__float__Sequence * member =
    (rosidl_runtime_c__float__Sequence *)(untyped_member);
  rosidl_runtime_c__float__Sequence__fini(member);
  return rosidl_runtime_c__float__Sequence__init(member, size);
}

size_t sdc_msgs__msg__Coordinate__rosidl_typesupport_introspection_c__size_function__Coordinate__top(
  const void * untyped_member)
{
  const rosidl_runtime_c__float__Sequence * member =
    (const rosidl_runtime_c__float__Sequence *)(untyped_member);
  return member->size;
}

const void * sdc_msgs__msg__Coordinate__rosidl_typesupport_introspection_c__get_const_function__Coordinate__top(
  const void * untyped_member, size_t index)
{
  const rosidl_runtime_c__float__Sequence * member =
    (const rosidl_runtime_c__float__Sequence *)(untyped_member);
  return &member->data[index];
}

void * sdc_msgs__msg__Coordinate__rosidl_typesupport_introspection_c__get_function__Coordinate__top(
  void * untyped_member, size_t index)
{
  rosidl_runtime_c__float__Sequence * member =
    (rosidl_runtime_c__float__Sequence *)(untyped_member);
  return &member->data[index];
}

void sdc_msgs__msg__Coordinate__rosidl_typesupport_introspection_c__fetch_function__Coordinate__top(
  const void * untyped_member, size_t index, void * untyped_value)
{
  const float * item =
    ((const float *)
    sdc_msgs__msg__Coordinate__rosidl_typesupport_introspection_c__get_const_function__Coordinate__top(untyped_member, index));
  float * value =
    (float *)(untyped_value);
  *value = *item;
}

void sdc_msgs__msg__Coordinate__rosidl_typesupport_introspection_c__assign_function__Coordinate__top(
  void * untyped_member, size_t index, const void * untyped_value)
{
  float * item =
    ((float *)
    sdc_msgs__msg__Coordinate__rosidl_typesupport_introspection_c__get_function__Coordinate__top(untyped_member, index));
  const float * value =
    (const float *)(untyped_value);
  *item = *value;
}

bool sdc_msgs__msg__Coordinate__rosidl_typesupport_introspection_c__resize_function__Coordinate__top(
  void * untyped_member, size_t size)
{
  rosidl_runtime_c__float__Sequence * member =
    (rosidl_runtime_c__float__Sequence *)(untyped_member);
  rosidl_runtime_c__float__Sequence__fini(member);
  return rosidl_runtime_c__float__Sequence__init(member, size);
}

size_t sdc_msgs__msg__Coordinate__rosidl_typesupport_introspection_c__size_function__Coordinate__bottom(
  const void * untyped_member)
{
  const rosidl_runtime_c__float__Sequence * member =
    (const rosidl_runtime_c__float__Sequence *)(untyped_member);
  return member->size;
}

const void * sdc_msgs__msg__Coordinate__rosidl_typesupport_introspection_c__get_const_function__Coordinate__bottom(
  const void * untyped_member, size_t index)
{
  const rosidl_runtime_c__float__Sequence * member =
    (const rosidl_runtime_c__float__Sequence *)(untyped_member);
  return &member->data[index];
}

void * sdc_msgs__msg__Coordinate__rosidl_typesupport_introspection_c__get_function__Coordinate__bottom(
  void * untyped_member, size_t index)
{
  rosidl_runtime_c__float__Sequence * member =
    (rosidl_runtime_c__float__Sequence *)(untyped_member);
  return &member->data[index];
}

void sdc_msgs__msg__Coordinate__rosidl_typesupport_introspection_c__fetch_function__Coordinate__bottom(
  const void * untyped_member, size_t index, void * untyped_value)
{
  const float * item =
    ((const float *)
    sdc_msgs__msg__Coordinate__rosidl_typesupport_introspection_c__get_const_function__Coordinate__bottom(untyped_member, index));
  float * value =
    (float *)(untyped_value);
  *value = *item;
}

void sdc_msgs__msg__Coordinate__rosidl_typesupport_introspection_c__assign_function__Coordinate__bottom(
  void * untyped_member, size_t index, const void * untyped_value)
{
  float * item =
    ((float *)
    sdc_msgs__msg__Coordinate__rosidl_typesupport_introspection_c__get_function__Coordinate__bottom(untyped_member, index));
  const float * value =
    (const float *)(untyped_value);
  *item = *value;
}

bool sdc_msgs__msg__Coordinate__rosidl_typesupport_introspection_c__resize_function__Coordinate__bottom(
  void * untyped_member, size_t size)
{
  rosidl_runtime_c__float__Sequence * member =
    (rosidl_runtime_c__float__Sequence *)(untyped_member);
  rosidl_runtime_c__float__Sequence__fini(member);
  return rosidl_runtime_c__float__Sequence__init(member, size);
}

static rosidl_typesupport_introspection_c__MessageMember sdc_msgs__msg__Coordinate__rosidl_typesupport_introspection_c__Coordinate_message_member_array[12] = {
  {
    "size",  // name
    rosidl_typesupport_introspection_c__ROS_TYPE_UINT8,  // type
    0,  // upper bound of string
    NULL,  // members of sub message
    false,  // is array
    0,  // array size
    false,  // is upper bound
    offsetof(sdc_msgs__msg__Coordinate, size),  // bytes offset in struct
    NULL,  // default value
    NULL,  // size() function pointer
    NULL,  // get_const(index) function pointer
    NULL,  // get(index) function pointer
    NULL,  // fetch(index, &value) function pointer
    NULL,  // assign(index, value) function pointer
    NULL  // resize(index) function pointer
  },
  {
    "reconsize",  // name
    rosidl_typesupport_introspection_c__ROS_TYPE_UINT8,  // type
    0,  // upper bound of string
    NULL,  // members of sub message
    false,  // is array
    0,  // array size
    false,  // is upper bound
    offsetof(sdc_msgs__msg__Coordinate, reconsize),  // bytes offset in struct
    NULL,  // default value
    NULL,  // size() function pointer
    NULL,  // get_const(index) function pointer
    NULL,  // get(index) function pointer
    NULL,  // fetch(index, &value) function pointer
    NULL,  // assign(index, value) function pointer
    NULL  // resize(index) function pointer
  },
  {
    "x",  // name
    rosidl_typesupport_introspection_c__ROS_TYPE_FLOAT,  // type
    0,  // upper bound of string
    NULL,  // members of sub message
    false,  // is array
    0,  // array size
    false,  // is upper bound
    offsetof(sdc_msgs__msg__Coordinate, x),  // bytes offset in struct
    NULL,  // default value
    NULL,  // size() function pointer
    NULL,  // get_const(index) function pointer
    NULL,  // get(index) function pointer
    NULL,  // fetch(index, &value) function pointer
    NULL,  // assign(index, value) function pointer
    NULL  // resize(index) function pointer
  },
  {
    "y",  // name
    rosidl_typesupport_introspection_c__ROS_TYPE_FLOAT,  // type
    0,  // upper bound of string
    NULL,  // members of sub message
    false,  // is array
    0,  // array size
    false,  // is upper bound
    offsetof(sdc_msgs__msg__Coordinate, y),  // bytes offset in struct
    NULL,  // default value
    NULL,  // size() function pointer
    NULL,  // get_const(index) function pointer
    NULL,  // get(index) function pointer
    NULL,  // fetch(index, &value) function pointer
    NULL,  // assign(index, value) function pointer
    NULL  // resize(index) function pointer
  },
  {
    "z",  // name
    rosidl_typesupport_introspection_c__ROS_TYPE_FLOAT,  // type
    0,  // upper bound of string
    NULL,  // members of sub message
    false,  // is array
    0,  // array size
    false,  // is upper bound
    offsetof(sdc_msgs__msg__Coordinate, z),  // bytes offset in struct
    NULL,  // default value
    NULL,  // size() function pointer
    NULL,  // get_const(index) function pointer
    NULL,  // get(index) function pointer
    NULL,  // fetch(index, &value) function pointer
    NULL,  // assign(index, value) function pointer
    NULL  // resize(index) function pointer
  },
  {
    "colour",  // name
    rosidl_typesupport_introspection_c__ROS_TYPE_FLOAT,  // type
    0,  // upper bound of string
    NULL,  // members of sub message
    false,  // is array
    0,  // array size
    false,  // is upper bound
    offsetof(sdc_msgs__msg__Coordinate, colour),  // bytes offset in struct
    NULL,  // default value
    NULL,  // size() function pointer
    NULL,  // get_const(index) function pointer
    NULL,  // get(index) function pointer
    NULL,  // fetch(index, &value) function pointer
    NULL,  // assign(index, value) function pointer
    NULL  // resize(index) function pointer
  },
  {
    "left",  // name
    rosidl_typesupport_introspection_c__ROS_TYPE_FLOAT,  // type
    0,  // upper bound of string
    NULL,  // members of sub message
    true,  // is array
    0,  // array size
    false,  // is upper bound
    offsetof(sdc_msgs__msg__Coordinate, left),  // bytes offset in struct
    NULL,  // default value
    sdc_msgs__msg__Coordinate__rosidl_typesupport_introspection_c__size_function__Coordinate__left,  // size() function pointer
    sdc_msgs__msg__Coordinate__rosidl_typesupport_introspection_c__get_const_function__Coordinate__left,  // get_const(index) function pointer
    sdc_msgs__msg__Coordinate__rosidl_typesupport_introspection_c__get_function__Coordinate__left,  // get(index) function pointer
    sdc_msgs__msg__Coordinate__rosidl_typesupport_introspection_c__fetch_function__Coordinate__left,  // fetch(index, &value) function pointer
    sdc_msgs__msg__Coordinate__rosidl_typesupport_introspection_c__assign_function__Coordinate__left,  // assign(index, value) function pointer
    sdc_msgs__msg__Coordinate__rosidl_typesupport_introspection_c__resize_function__Coordinate__left  // resize(index) function pointer
  },
  {
    "right",  // name
    rosidl_typesupport_introspection_c__ROS_TYPE_FLOAT,  // type
    0,  // upper bound of string
    NULL,  // members of sub message
    true,  // is array
    0,  // array size
    false,  // is upper bound
    offsetof(sdc_msgs__msg__Coordinate, right),  // bytes offset in struct
    NULL,  // default value
    sdc_msgs__msg__Coordinate__rosidl_typesupport_introspection_c__size_function__Coordinate__right,  // size() function pointer
    sdc_msgs__msg__Coordinate__rosidl_typesupport_introspection_c__get_const_function__Coordinate__right,  // get_const(index) function pointer
    sdc_msgs__msg__Coordinate__rosidl_typesupport_introspection_c__get_function__Coordinate__right,  // get(index) function pointer
    sdc_msgs__msg__Coordinate__rosidl_typesupport_introspection_c__fetch_function__Coordinate__right,  // fetch(index, &value) function pointer
    sdc_msgs__msg__Coordinate__rosidl_typesupport_introspection_c__assign_function__Coordinate__right,  // assign(index, value) function pointer
    sdc_msgs__msg__Coordinate__rosidl_typesupport_introspection_c__resize_function__Coordinate__right  // resize(index) function pointer
  },
  {
    "front",  // name
    rosidl_typesupport_introspection_c__ROS_TYPE_FLOAT,  // type
    0,  // upper bound of string
    NULL,  // members of sub message
    true,  // is array
    0,  // array size
    false,  // is upper bound
    offsetof(sdc_msgs__msg__Coordinate, front),  // bytes offset in struct
    NULL,  // default value
    sdc_msgs__msg__Coordinate__rosidl_typesupport_introspection_c__size_function__Coordinate__front,  // size() function pointer
    sdc_msgs__msg__Coordinate__rosidl_typesupport_introspection_c__get_const_function__Coordinate__front,  // get_const(index) function pointer
    sdc_msgs__msg__Coordinate__rosidl_typesupport_introspection_c__get_function__Coordinate__front,  // get(index) function pointer
    sdc_msgs__msg__Coordinate__rosidl_typesupport_introspection_c__fetch_function__Coordinate__front,  // fetch(index, &value) function pointer
    sdc_msgs__msg__Coordinate__rosidl_typesupport_introspection_c__assign_function__Coordinate__front,  // assign(index, value) function pointer
    sdc_msgs__msg__Coordinate__rosidl_typesupport_introspection_c__resize_function__Coordinate__front  // resize(index) function pointer
  },
  {
    "back",  // name
    rosidl_typesupport_introspection_c__ROS_TYPE_FLOAT,  // type
    0,  // upper bound of string
    NULL,  // members of sub message
    true,  // is array
    0,  // array size
    false,  // is upper bound
    offsetof(sdc_msgs__msg__Coordinate, back),  // bytes offset in struct
    NULL,  // default value
    sdc_msgs__msg__Coordinate__rosidl_typesupport_introspection_c__size_function__Coordinate__back,  // size() function pointer
    sdc_msgs__msg__Coordinate__rosidl_typesupport_introspection_c__get_const_function__Coordinate__back,  // get_const(index) function pointer
    sdc_msgs__msg__Coordinate__rosidl_typesupport_introspection_c__get_function__Coordinate__back,  // get(index) function pointer
    sdc_msgs__msg__Coordinate__rosidl_typesupport_introspection_c__fetch_function__Coordinate__back,  // fetch(index, &value) function pointer
    sdc_msgs__msg__Coordinate__rosidl_typesupport_introspection_c__assign_function__Coordinate__back,  // assign(index, value) function pointer
    sdc_msgs__msg__Coordinate__rosidl_typesupport_introspection_c__resize_function__Coordinate__back  // resize(index) function pointer
  },
  {
    "top",  // name
    rosidl_typesupport_introspection_c__ROS_TYPE_FLOAT,  // type
    0,  // upper bound of string
    NULL,  // members of sub message
    true,  // is array
    0,  // array size
    false,  // is upper bound
    offsetof(sdc_msgs__msg__Coordinate, top),  // bytes offset in struct
    NULL,  // default value
    sdc_msgs__msg__Coordinate__rosidl_typesupport_introspection_c__size_function__Coordinate__top,  // size() function pointer
    sdc_msgs__msg__Coordinate__rosidl_typesupport_introspection_c__get_const_function__Coordinate__top,  // get_const(index) function pointer
    sdc_msgs__msg__Coordinate__rosidl_typesupport_introspection_c__get_function__Coordinate__top,  // get(index) function pointer
    sdc_msgs__msg__Coordinate__rosidl_typesupport_introspection_c__fetch_function__Coordinate__top,  // fetch(index, &value) function pointer
    sdc_msgs__msg__Coordinate__rosidl_typesupport_introspection_c__assign_function__Coordinate__top,  // assign(index, value) function pointer
    sdc_msgs__msg__Coordinate__rosidl_typesupport_introspection_c__resize_function__Coordinate__top  // resize(index) function pointer
  },
  {
    "bottom",  // name
    rosidl_typesupport_introspection_c__ROS_TYPE_FLOAT,  // type
    0,  // upper bound of string
    NULL,  // members of sub message
    true,  // is array
    0,  // array size
    false,  // is upper bound
    offsetof(sdc_msgs__msg__Coordinate, bottom),  // bytes offset in struct
    NULL,  // default value
    sdc_msgs__msg__Coordinate__rosidl_typesupport_introspection_c__size_function__Coordinate__bottom,  // size() function pointer
    sdc_msgs__msg__Coordinate__rosidl_typesupport_introspection_c__get_const_function__Coordinate__bottom,  // get_const(index) function pointer
    sdc_msgs__msg__Coordinate__rosidl_typesupport_introspection_c__get_function__Coordinate__bottom,  // get(index) function pointer
    sdc_msgs__msg__Coordinate__rosidl_typesupport_introspection_c__fetch_function__Coordinate__bottom,  // fetch(index, &value) function pointer
    sdc_msgs__msg__Coordinate__rosidl_typesupport_introspection_c__assign_function__Coordinate__bottom,  // assign(index, value) function pointer
    sdc_msgs__msg__Coordinate__rosidl_typesupport_introspection_c__resize_function__Coordinate__bottom  // resize(index) function pointer
  }
};

static const rosidl_typesupport_introspection_c__MessageMembers sdc_msgs__msg__Coordinate__rosidl_typesupport_introspection_c__Coordinate_message_members = {
  "sdc_msgs__msg",  // message namespace
  "Coordinate",  // message name
  12,  // number of fields
  sizeof(sdc_msgs__msg__Coordinate),
  sdc_msgs__msg__Coordinate__rosidl_typesupport_introspection_c__Coordinate_message_member_array,  // message members
  sdc_msgs__msg__Coordinate__rosidl_typesupport_introspection_c__Coordinate_init_function,  // function to initialize message memory (memory has to be allocated)
  sdc_msgs__msg__Coordinate__rosidl_typesupport_introspection_c__Coordinate_fini_function  // function to terminate message instance (will not free memory)
};

// this is not const since it must be initialized on first access
// since C does not allow non-integral compile-time constants
static rosidl_message_type_support_t sdc_msgs__msg__Coordinate__rosidl_typesupport_introspection_c__Coordinate_message_type_support_handle = {
  0,
  &sdc_msgs__msg__Coordinate__rosidl_typesupport_introspection_c__Coordinate_message_members,
  get_message_typesupport_handle_function,
};

ROSIDL_TYPESUPPORT_INTROSPECTION_C_EXPORT_sdc_msgs
const rosidl_message_type_support_t *
ROSIDL_TYPESUPPORT_INTERFACE__MESSAGE_SYMBOL_NAME(rosidl_typesupport_introspection_c, sdc_msgs, msg, Coordinate)() {
  if (!sdc_msgs__msg__Coordinate__rosidl_typesupport_introspection_c__Coordinate_message_type_support_handle.typesupport_identifier) {
    sdc_msgs__msg__Coordinate__rosidl_typesupport_introspection_c__Coordinate_message_type_support_handle.typesupport_identifier =
      rosidl_typesupport_introspection_c__identifier;
  }
  return &sdc_msgs__msg__Coordinate__rosidl_typesupport_introspection_c__Coordinate_message_type_support_handle;
}
#ifdef __cplusplus
}
#endif
