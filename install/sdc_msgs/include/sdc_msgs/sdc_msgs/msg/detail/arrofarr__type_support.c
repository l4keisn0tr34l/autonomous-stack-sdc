// generated from rosidl_typesupport_introspection_c/resource/idl__type_support.c.em
// with input from sdc_msgs:msg/Arrofarr.idl
// generated code does not contain a copyright notice

#include <stddef.h>
#include "sdc_msgs/msg/detail/arrofarr__rosidl_typesupport_introspection_c.h"
#include "sdc_msgs/msg/rosidl_typesupport_introspection_c__visibility_control.h"
#include "rosidl_typesupport_introspection_c/field_types.h"
#include "rosidl_typesupport_introspection_c/identifier.h"
#include "rosidl_typesupport_introspection_c/message_introspection.h"
#include "sdc_msgs/msg/detail/arrofarr__functions.h"
#include "sdc_msgs/msg/detail/arrofarr__struct.h"


// Include directives for member types
// Member `header`
#include "std_msgs/msg/header.h"
// Member `header`
#include "std_msgs/msg/detail/header__rosidl_typesupport_introspection_c.h"
// Member `data`
#include "sdc_msgs/msg/arr.h"
// Member `data`
#include "sdc_msgs/msg/detail/arr__rosidl_typesupport_introspection_c.h"

#ifdef __cplusplus
extern "C"
{
#endif

void sdc_msgs__msg__Arrofarr__rosidl_typesupport_introspection_c__Arrofarr_init_function(
  void * message_memory, enum rosidl_runtime_c__message_initialization _init)
{
  // TODO(karsten1987): initializers are not yet implemented for typesupport c
  // see https://github.com/ros2/ros2/issues/397
  (void) _init;
  sdc_msgs__msg__Arrofarr__init(message_memory);
}

void sdc_msgs__msg__Arrofarr__rosidl_typesupport_introspection_c__Arrofarr_fini_function(void * message_memory)
{
  sdc_msgs__msg__Arrofarr__fini(message_memory);
}

size_t sdc_msgs__msg__Arrofarr__rosidl_typesupport_introspection_c__size_function__Arrofarr__data(
  const void * untyped_member)
{
  const sdc_msgs__msg__Arr__Sequence * member =
    (const sdc_msgs__msg__Arr__Sequence *)(untyped_member);
  return member->size;
}

const void * sdc_msgs__msg__Arrofarr__rosidl_typesupport_introspection_c__get_const_function__Arrofarr__data(
  const void * untyped_member, size_t index)
{
  const sdc_msgs__msg__Arr__Sequence * member =
    (const sdc_msgs__msg__Arr__Sequence *)(untyped_member);
  return &member->data[index];
}

void * sdc_msgs__msg__Arrofarr__rosidl_typesupport_introspection_c__get_function__Arrofarr__data(
  void * untyped_member, size_t index)
{
  sdc_msgs__msg__Arr__Sequence * member =
    (sdc_msgs__msg__Arr__Sequence *)(untyped_member);
  return &member->data[index];
}

void sdc_msgs__msg__Arrofarr__rosidl_typesupport_introspection_c__fetch_function__Arrofarr__data(
  const void * untyped_member, size_t index, void * untyped_value)
{
  const sdc_msgs__msg__Arr * item =
    ((const sdc_msgs__msg__Arr *)
    sdc_msgs__msg__Arrofarr__rosidl_typesupport_introspection_c__get_const_function__Arrofarr__data(untyped_member, index));
  sdc_msgs__msg__Arr * value =
    (sdc_msgs__msg__Arr *)(untyped_value);
  *value = *item;
}

void sdc_msgs__msg__Arrofarr__rosidl_typesupport_introspection_c__assign_function__Arrofarr__data(
  void * untyped_member, size_t index, const void * untyped_value)
{
  sdc_msgs__msg__Arr * item =
    ((sdc_msgs__msg__Arr *)
    sdc_msgs__msg__Arrofarr__rosidl_typesupport_introspection_c__get_function__Arrofarr__data(untyped_member, index));
  const sdc_msgs__msg__Arr * value =
    (const sdc_msgs__msg__Arr *)(untyped_value);
  *item = *value;
}

bool sdc_msgs__msg__Arrofarr__rosidl_typesupport_introspection_c__resize_function__Arrofarr__data(
  void * untyped_member, size_t size)
{
  sdc_msgs__msg__Arr__Sequence * member =
    (sdc_msgs__msg__Arr__Sequence *)(untyped_member);
  sdc_msgs__msg__Arr__Sequence__fini(member);
  return sdc_msgs__msg__Arr__Sequence__init(member, size);
}

static rosidl_typesupport_introspection_c__MessageMember sdc_msgs__msg__Arrofarr__rosidl_typesupport_introspection_c__Arrofarr_message_member_array[2] = {
  {
    "header",  // name
    rosidl_typesupport_introspection_c__ROS_TYPE_MESSAGE,  // type
    0,  // upper bound of string
    NULL,  // members of sub message (initialized later)
    false,  // is array
    0,  // array size
    false,  // is upper bound
    offsetof(sdc_msgs__msg__Arrofarr, header),  // bytes offset in struct
    NULL,  // default value
    NULL,  // size() function pointer
    NULL,  // get_const(index) function pointer
    NULL,  // get(index) function pointer
    NULL,  // fetch(index, &value) function pointer
    NULL,  // assign(index, value) function pointer
    NULL  // resize(index) function pointer
  },
  {
    "data",  // name
    rosidl_typesupport_introspection_c__ROS_TYPE_MESSAGE,  // type
    0,  // upper bound of string
    NULL,  // members of sub message (initialized later)
    true,  // is array
    0,  // array size
    false,  // is upper bound
    offsetof(sdc_msgs__msg__Arrofarr, data),  // bytes offset in struct
    NULL,  // default value
    sdc_msgs__msg__Arrofarr__rosidl_typesupport_introspection_c__size_function__Arrofarr__data,  // size() function pointer
    sdc_msgs__msg__Arrofarr__rosidl_typesupport_introspection_c__get_const_function__Arrofarr__data,  // get_const(index) function pointer
    sdc_msgs__msg__Arrofarr__rosidl_typesupport_introspection_c__get_function__Arrofarr__data,  // get(index) function pointer
    sdc_msgs__msg__Arrofarr__rosidl_typesupport_introspection_c__fetch_function__Arrofarr__data,  // fetch(index, &value) function pointer
    sdc_msgs__msg__Arrofarr__rosidl_typesupport_introspection_c__assign_function__Arrofarr__data,  // assign(index, value) function pointer
    sdc_msgs__msg__Arrofarr__rosidl_typesupport_introspection_c__resize_function__Arrofarr__data  // resize(index) function pointer
  }
};

static const rosidl_typesupport_introspection_c__MessageMembers sdc_msgs__msg__Arrofarr__rosidl_typesupport_introspection_c__Arrofarr_message_members = {
  "sdc_msgs__msg",  // message namespace
  "Arrofarr",  // message name
  2,  // number of fields
  sizeof(sdc_msgs__msg__Arrofarr),
  sdc_msgs__msg__Arrofarr__rosidl_typesupport_introspection_c__Arrofarr_message_member_array,  // message members
  sdc_msgs__msg__Arrofarr__rosidl_typesupport_introspection_c__Arrofarr_init_function,  // function to initialize message memory (memory has to be allocated)
  sdc_msgs__msg__Arrofarr__rosidl_typesupport_introspection_c__Arrofarr_fini_function  // function to terminate message instance (will not free memory)
};

// this is not const since it must be initialized on first access
// since C does not allow non-integral compile-time constants
static rosidl_message_type_support_t sdc_msgs__msg__Arrofarr__rosidl_typesupport_introspection_c__Arrofarr_message_type_support_handle = {
  0,
  &sdc_msgs__msg__Arrofarr__rosidl_typesupport_introspection_c__Arrofarr_message_members,
  get_message_typesupport_handle_function,
};

ROSIDL_TYPESUPPORT_INTROSPECTION_C_EXPORT_sdc_msgs
const rosidl_message_type_support_t *
ROSIDL_TYPESUPPORT_INTERFACE__MESSAGE_SYMBOL_NAME(rosidl_typesupport_introspection_c, sdc_msgs, msg, Arrofarr)() {
  sdc_msgs__msg__Arrofarr__rosidl_typesupport_introspection_c__Arrofarr_message_member_array[0].members_ =
    ROSIDL_TYPESUPPORT_INTERFACE__MESSAGE_SYMBOL_NAME(rosidl_typesupport_introspection_c, std_msgs, msg, Header)();
  sdc_msgs__msg__Arrofarr__rosidl_typesupport_introspection_c__Arrofarr_message_member_array[1].members_ =
    ROSIDL_TYPESUPPORT_INTERFACE__MESSAGE_SYMBOL_NAME(rosidl_typesupport_introspection_c, sdc_msgs, msg, Arr)();
  if (!sdc_msgs__msg__Arrofarr__rosidl_typesupport_introspection_c__Arrofarr_message_type_support_handle.typesupport_identifier) {
    sdc_msgs__msg__Arrofarr__rosidl_typesupport_introspection_c__Arrofarr_message_type_support_handle.typesupport_identifier =
      rosidl_typesupport_introspection_c__identifier;
  }
  return &sdc_msgs__msg__Arrofarr__rosidl_typesupport_introspection_c__Arrofarr_message_type_support_handle;
}
#ifdef __cplusplus
}
#endif
