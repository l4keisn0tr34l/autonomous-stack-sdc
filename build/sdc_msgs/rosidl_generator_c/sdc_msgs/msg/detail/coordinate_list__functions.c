// generated from rosidl_generator_c/resource/idl__functions.c.em
// with input from sdc_msgs:msg/CoordinateList.idl
// generated code does not contain a copyright notice
#include "sdc_msgs/msg/detail/coordinate_list__functions.h"

#include <assert.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#include "rcutils/allocator.h"


// Include directives for member types
// Member `header`
#include "std_msgs/msg/detail/header__functions.h"
// Member `cone_coordinates`
#include "sdc_msgs/msg/detail/coordinate__functions.h"

bool
sdc_msgs__msg__CoordinateList__init(sdc_msgs__msg__CoordinateList * msg)
{
  if (!msg) {
    return false;
  }
  // header
  if (!std_msgs__msg__Header__init(&msg->header)) {
    sdc_msgs__msg__CoordinateList__fini(msg);
    return false;
  }
  // size
  // cone_coordinates
  if (!sdc_msgs__msg__Coordinate__Sequence__init(&msg->cone_coordinates, 0)) {
    sdc_msgs__msg__CoordinateList__fini(msg);
    return false;
  }
  return true;
}

void
sdc_msgs__msg__CoordinateList__fini(sdc_msgs__msg__CoordinateList * msg)
{
  if (!msg) {
    return;
  }
  // header
  std_msgs__msg__Header__fini(&msg->header);
  // size
  // cone_coordinates
  sdc_msgs__msg__Coordinate__Sequence__fini(&msg->cone_coordinates);
}

bool
sdc_msgs__msg__CoordinateList__are_equal(const sdc_msgs__msg__CoordinateList * lhs, const sdc_msgs__msg__CoordinateList * rhs)
{
  if (!lhs || !rhs) {
    return false;
  }
  // header
  if (!std_msgs__msg__Header__are_equal(
      &(lhs->header), &(rhs->header)))
  {
    return false;
  }
  // size
  if (lhs->size != rhs->size) {
    return false;
  }
  // cone_coordinates
  if (!sdc_msgs__msg__Coordinate__Sequence__are_equal(
      &(lhs->cone_coordinates), &(rhs->cone_coordinates)))
  {
    return false;
  }
  return true;
}

bool
sdc_msgs__msg__CoordinateList__copy(
  const sdc_msgs__msg__CoordinateList * input,
  sdc_msgs__msg__CoordinateList * output)
{
  if (!input || !output) {
    return false;
  }
  // header
  if (!std_msgs__msg__Header__copy(
      &(input->header), &(output->header)))
  {
    return false;
  }
  // size
  output->size = input->size;
  // cone_coordinates
  if (!sdc_msgs__msg__Coordinate__Sequence__copy(
      &(input->cone_coordinates), &(output->cone_coordinates)))
  {
    return false;
  }
  return true;
}

sdc_msgs__msg__CoordinateList *
sdc_msgs__msg__CoordinateList__create()
{
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  sdc_msgs__msg__CoordinateList * msg = (sdc_msgs__msg__CoordinateList *)allocator.allocate(sizeof(sdc_msgs__msg__CoordinateList), allocator.state);
  if (!msg) {
    return NULL;
  }
  memset(msg, 0, sizeof(sdc_msgs__msg__CoordinateList));
  bool success = sdc_msgs__msg__CoordinateList__init(msg);
  if (!success) {
    allocator.deallocate(msg, allocator.state);
    return NULL;
  }
  return msg;
}

void
sdc_msgs__msg__CoordinateList__destroy(sdc_msgs__msg__CoordinateList * msg)
{
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  if (msg) {
    sdc_msgs__msg__CoordinateList__fini(msg);
  }
  allocator.deallocate(msg, allocator.state);
}


bool
sdc_msgs__msg__CoordinateList__Sequence__init(sdc_msgs__msg__CoordinateList__Sequence * array, size_t size)
{
  if (!array) {
    return false;
  }
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  sdc_msgs__msg__CoordinateList * data = NULL;

  if (size) {
    data = (sdc_msgs__msg__CoordinateList *)allocator.zero_allocate(size, sizeof(sdc_msgs__msg__CoordinateList), allocator.state);
    if (!data) {
      return false;
    }
    // initialize all array elements
    size_t i;
    for (i = 0; i < size; ++i) {
      bool success = sdc_msgs__msg__CoordinateList__init(&data[i]);
      if (!success) {
        break;
      }
    }
    if (i < size) {
      // if initialization failed finalize the already initialized array elements
      for (; i > 0; --i) {
        sdc_msgs__msg__CoordinateList__fini(&data[i - 1]);
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
sdc_msgs__msg__CoordinateList__Sequence__fini(sdc_msgs__msg__CoordinateList__Sequence * array)
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
      sdc_msgs__msg__CoordinateList__fini(&array->data[i]);
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

sdc_msgs__msg__CoordinateList__Sequence *
sdc_msgs__msg__CoordinateList__Sequence__create(size_t size)
{
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  sdc_msgs__msg__CoordinateList__Sequence * array = (sdc_msgs__msg__CoordinateList__Sequence *)allocator.allocate(sizeof(sdc_msgs__msg__CoordinateList__Sequence), allocator.state);
  if (!array) {
    return NULL;
  }
  bool success = sdc_msgs__msg__CoordinateList__Sequence__init(array, size);
  if (!success) {
    allocator.deallocate(array, allocator.state);
    return NULL;
  }
  return array;
}

void
sdc_msgs__msg__CoordinateList__Sequence__destroy(sdc_msgs__msg__CoordinateList__Sequence * array)
{
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  if (array) {
    sdc_msgs__msg__CoordinateList__Sequence__fini(array);
  }
  allocator.deallocate(array, allocator.state);
}

bool
sdc_msgs__msg__CoordinateList__Sequence__are_equal(const sdc_msgs__msg__CoordinateList__Sequence * lhs, const sdc_msgs__msg__CoordinateList__Sequence * rhs)
{
  if (!lhs || !rhs) {
    return false;
  }
  if (lhs->size != rhs->size) {
    return false;
  }
  for (size_t i = 0; i < lhs->size; ++i) {
    if (!sdc_msgs__msg__CoordinateList__are_equal(&(lhs->data[i]), &(rhs->data[i]))) {
      return false;
    }
  }
  return true;
}

bool
sdc_msgs__msg__CoordinateList__Sequence__copy(
  const sdc_msgs__msg__CoordinateList__Sequence * input,
  sdc_msgs__msg__CoordinateList__Sequence * output)
{
  if (!input || !output) {
    return false;
  }
  if (output->capacity < input->size) {
    const size_t allocation_size =
      input->size * sizeof(sdc_msgs__msg__CoordinateList);
    rcutils_allocator_t allocator = rcutils_get_default_allocator();
    sdc_msgs__msg__CoordinateList * data =
      (sdc_msgs__msg__CoordinateList *)allocator.reallocate(
      output->data, allocation_size, allocator.state);
    if (!data) {
      return false;
    }
    // If reallocation succeeded, memory may or may not have been moved
    // to fulfill the allocation request, invalidating output->data.
    output->data = data;
    for (size_t i = output->capacity; i < input->size; ++i) {
      if (!sdc_msgs__msg__CoordinateList__init(&output->data[i])) {
        // If initialization of any new item fails, roll back
        // all previously initialized items. Existing items
        // in output are to be left unmodified.
        for (; i-- > output->capacity; ) {
          sdc_msgs__msg__CoordinateList__fini(&output->data[i]);
        }
        return false;
      }
    }
    output->capacity = input->size;
  }
  output->size = input->size;
  for (size_t i = 0; i < input->size; ++i) {
    if (!sdc_msgs__msg__CoordinateList__copy(
        &(input->data[i]), &(output->data[i])))
    {
      return false;
    }
  }
  return true;
}
