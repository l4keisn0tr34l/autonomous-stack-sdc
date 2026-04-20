// generated from rosidl_generator_c/resource/idl__functions.c.em
// with input from sdc_msgs:msg/Coordinate.idl
// generated code does not contain a copyright notice
#include "sdc_msgs/msg/detail/coordinate__functions.h"

#include <assert.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#include "rcutils/allocator.h"


// Include directives for member types
// Member `left`
// Member `right`
// Member `front`
// Member `back`
// Member `top`
// Member `bottom`
#include "rosidl_runtime_c/primitives_sequence_functions.h"

bool
sdc_msgs__msg__Coordinate__init(sdc_msgs__msg__Coordinate * msg)
{
  if (!msg) {
    return false;
  }
  // size
  // reconsize
  // x
  // y
  // z
  // colour
  // left
  if (!rosidl_runtime_c__float__Sequence__init(&msg->left, 0)) {
    sdc_msgs__msg__Coordinate__fini(msg);
    return false;
  }
  // right
  if (!rosidl_runtime_c__float__Sequence__init(&msg->right, 0)) {
    sdc_msgs__msg__Coordinate__fini(msg);
    return false;
  }
  // front
  if (!rosidl_runtime_c__float__Sequence__init(&msg->front, 0)) {
    sdc_msgs__msg__Coordinate__fini(msg);
    return false;
  }
  // back
  if (!rosidl_runtime_c__float__Sequence__init(&msg->back, 0)) {
    sdc_msgs__msg__Coordinate__fini(msg);
    return false;
  }
  // top
  if (!rosidl_runtime_c__float__Sequence__init(&msg->top, 0)) {
    sdc_msgs__msg__Coordinate__fini(msg);
    return false;
  }
  // bottom
  if (!rosidl_runtime_c__float__Sequence__init(&msg->bottom, 0)) {
    sdc_msgs__msg__Coordinate__fini(msg);
    return false;
  }
  return true;
}

void
sdc_msgs__msg__Coordinate__fini(sdc_msgs__msg__Coordinate * msg)
{
  if (!msg) {
    return;
  }
  // size
  // reconsize
  // x
  // y
  // z
  // colour
  // left
  rosidl_runtime_c__float__Sequence__fini(&msg->left);
  // right
  rosidl_runtime_c__float__Sequence__fini(&msg->right);
  // front
  rosidl_runtime_c__float__Sequence__fini(&msg->front);
  // back
  rosidl_runtime_c__float__Sequence__fini(&msg->back);
  // top
  rosidl_runtime_c__float__Sequence__fini(&msg->top);
  // bottom
  rosidl_runtime_c__float__Sequence__fini(&msg->bottom);
}

bool
sdc_msgs__msg__Coordinate__are_equal(const sdc_msgs__msg__Coordinate * lhs, const sdc_msgs__msg__Coordinate * rhs)
{
  if (!lhs || !rhs) {
    return false;
  }
  // size
  if (lhs->size != rhs->size) {
    return false;
  }
  // reconsize
  if (lhs->reconsize != rhs->reconsize) {
    return false;
  }
  // x
  if (lhs->x != rhs->x) {
    return false;
  }
  // y
  if (lhs->y != rhs->y) {
    return false;
  }
  // z
  if (lhs->z != rhs->z) {
    return false;
  }
  // colour
  if (lhs->colour != rhs->colour) {
    return false;
  }
  // left
  if (!rosidl_runtime_c__float__Sequence__are_equal(
      &(lhs->left), &(rhs->left)))
  {
    return false;
  }
  // right
  if (!rosidl_runtime_c__float__Sequence__are_equal(
      &(lhs->right), &(rhs->right)))
  {
    return false;
  }
  // front
  if (!rosidl_runtime_c__float__Sequence__are_equal(
      &(lhs->front), &(rhs->front)))
  {
    return false;
  }
  // back
  if (!rosidl_runtime_c__float__Sequence__are_equal(
      &(lhs->back), &(rhs->back)))
  {
    return false;
  }
  // top
  if (!rosidl_runtime_c__float__Sequence__are_equal(
      &(lhs->top), &(rhs->top)))
  {
    return false;
  }
  // bottom
  if (!rosidl_runtime_c__float__Sequence__are_equal(
      &(lhs->bottom), &(rhs->bottom)))
  {
    return false;
  }
  return true;
}

bool
sdc_msgs__msg__Coordinate__copy(
  const sdc_msgs__msg__Coordinate * input,
  sdc_msgs__msg__Coordinate * output)
{
  if (!input || !output) {
    return false;
  }
  // size
  output->size = input->size;
  // reconsize
  output->reconsize = input->reconsize;
  // x
  output->x = input->x;
  // y
  output->y = input->y;
  // z
  output->z = input->z;
  // colour
  output->colour = input->colour;
  // left
  if (!rosidl_runtime_c__float__Sequence__copy(
      &(input->left), &(output->left)))
  {
    return false;
  }
  // right
  if (!rosidl_runtime_c__float__Sequence__copy(
      &(input->right), &(output->right)))
  {
    return false;
  }
  // front
  if (!rosidl_runtime_c__float__Sequence__copy(
      &(input->front), &(output->front)))
  {
    return false;
  }
  // back
  if (!rosidl_runtime_c__float__Sequence__copy(
      &(input->back), &(output->back)))
  {
    return false;
  }
  // top
  if (!rosidl_runtime_c__float__Sequence__copy(
      &(input->top), &(output->top)))
  {
    return false;
  }
  // bottom
  if (!rosidl_runtime_c__float__Sequence__copy(
      &(input->bottom), &(output->bottom)))
  {
    return false;
  }
  return true;
}

sdc_msgs__msg__Coordinate *
sdc_msgs__msg__Coordinate__create()
{
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  sdc_msgs__msg__Coordinate * msg = (sdc_msgs__msg__Coordinate *)allocator.allocate(sizeof(sdc_msgs__msg__Coordinate), allocator.state);
  if (!msg) {
    return NULL;
  }
  memset(msg, 0, sizeof(sdc_msgs__msg__Coordinate));
  bool success = sdc_msgs__msg__Coordinate__init(msg);
  if (!success) {
    allocator.deallocate(msg, allocator.state);
    return NULL;
  }
  return msg;
}

void
sdc_msgs__msg__Coordinate__destroy(sdc_msgs__msg__Coordinate * msg)
{
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  if (msg) {
    sdc_msgs__msg__Coordinate__fini(msg);
  }
  allocator.deallocate(msg, allocator.state);
}


bool
sdc_msgs__msg__Coordinate__Sequence__init(sdc_msgs__msg__Coordinate__Sequence * array, size_t size)
{
  if (!array) {
    return false;
  }
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  sdc_msgs__msg__Coordinate * data = NULL;

  if (size) {
    data = (sdc_msgs__msg__Coordinate *)allocator.zero_allocate(size, sizeof(sdc_msgs__msg__Coordinate), allocator.state);
    if (!data) {
      return false;
    }
    // initialize all array elements
    size_t i;
    for (i = 0; i < size; ++i) {
      bool success = sdc_msgs__msg__Coordinate__init(&data[i]);
      if (!success) {
        break;
      }
    }
    if (i < size) {
      // if initialization failed finalize the already initialized array elements
      for (; i > 0; --i) {
        sdc_msgs__msg__Coordinate__fini(&data[i - 1]);
      }
      allocator.deallocate(data, allocator.state);
      return false;
    }
  }
  array->data = data;
  array->size = size;
  array->capacity = size;
  return true;
}

void
sdc_msgs__msg__Coordinate__Sequence__fini(sdc_msgs__msg__Coordinate__Sequence * array)
{
  if (!array) {
    return;
  }
  rcutils_allocator_t allocator = rcutils_get_default_allocator();

  if (array->data) {
    // ensure that data and capacity values are consistent
    assert(array->capacity > 0);
    // finalize all array elements
    for (size_t i = 0; i < array->capacity; ++i) {
      sdc_msgs__msg__Coordinate__fini(&array->data[i]);
    }
    allocator.deallocate(array->data, allocator.state);
    array->data = NULL;
    array->size = 0;
    array->capacity = 0;
  } else {
    // ensure that data, size, and capacity values are consistent
    assert(0 == array->size);
    assert(0 == array->capacity);
  }
}

sdc_msgs__msg__Coordinate__Sequence *
sdc_msgs__msg__Coordinate__Sequence__create(size_t size)
{
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  sdc_msgs__msg__Coordinate__Sequence * array = (sdc_msgs__msg__Coordinate__Sequence *)allocator.allocate(sizeof(sdc_msgs__msg__Coordinate__Sequence), allocator.state);
  if (!array) {
    return NULL;
  }
  bool success = sdc_msgs__msg__Coordinate__Sequence__init(array, size);
  if (!success) {
    allocator.deallocate(array, allocator.state);
    return NULL;
  }
  return array;
}

void
sdc_msgs__msg__Coordinate__Sequence__destroy(sdc_msgs__msg__Coordinate__Sequence * array)
{
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  if (array) {
    sdc_msgs__msg__Coordinate__Sequence__fini(array);
  }
  allocator.deallocate(array, allocator.state);
}

bool
sdc_msgs__msg__Coordinate__Sequence__are_equal(const sdc_msgs__msg__Coordinate__Sequence * lhs, const sdc_msgs__msg__Coordinate__Sequence * rhs)
{
  if (!lhs || !rhs) {
    return false;
  }
  if (lhs->size != rhs->size) {
    return false;
  }
  for (size_t i = 0; i < lhs->size; ++i) {
    if (!sdc_msgs__msg__Coordinate__are_equal(&(lhs->data[i]), &(rhs->data[i]))) {
      return false;
    }
  }
  return true;
}

bool
sdc_msgs__msg__Coordinate__Sequence__copy(
  const sdc_msgs__msg__Coordinate__Sequence * input,
  sdc_msgs__msg__Coordinate__Sequence * output)
{
  if (!input || !output) {
    return false;
  }
  if (output->capacity < input->size) {
    const size_t allocation_size =
      input->size * sizeof(sdc_msgs__msg__Coordinate);
    rcutils_allocator_t allocator = rcutils_get_default_allocator();
    sdc_msgs__msg__Coordinate * data =
      (sdc_msgs__msg__Coordinate *)allocator.reallocate(
      output->data, allocation_size, allocator.state);
    if (!data) {
      return false;
    }
    // If reallocation succeeded, memory may or may not have been moved
    // to fulfill the allocation request, invalidating output->data.
    output->data = data;
    for (size_t i = output->capacity; i < input->size; ++i) {
      if (!sdc_msgs__msg__Coordinate__init(&output->data[i])) {
        // If initialization of any new item fails, roll back
        // all previously initialized items. Existing items
        // in output are to be left unmodified.
        for (; i-- > output->capacity; ) {
          sdc_msgs__msg__Coordinate__fini(&output->data[i]);
        }
        return false;
      }
    }
    output->capacity = input->size;
  }
  output->size = input->size;
  for (size_t i = 0; i < input->size; ++i) {
    if (!sdc_msgs__msg__Coordinate__copy(
        &(input->data[i]), &(output->data[i])))
    {
      return false;
    }
  }
  return true;
}
