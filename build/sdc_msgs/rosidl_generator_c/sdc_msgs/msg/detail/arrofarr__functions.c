// generated from rosidl_generator_c/resource/idl__functions.c.em
// with input from sdc_msgs:msg/Arrofarr.idl
// generated code does not contain a copyright notice
#include "sdc_msgs/msg/detail/arrofarr__functions.h"

#include <assert.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#include "rcutils/allocator.h"


// Include directives for member types
// Member `header`
#include "std_msgs/msg/detail/header__functions.h"
// Member `data`
#include "sdc_msgs/msg/detail/arr__functions.h"

bool
sdc_msgs__msg__Arrofarr__init(sdc_msgs__msg__Arrofarr * msg)
{
  if (!msg) {
    return false;
  }
  // header
  if (!std_msgs__msg__Header__init(&msg->header)) {
    sdc_msgs__msg__Arrofarr__fini(msg);
    return false;
  }
  // data
  if (!sdc_msgs__msg__Arr__Sequence__init(&msg->data, 0)) {
    sdc_msgs__msg__Arrofarr__fini(msg);
    return false;
  }
  return true;
}

void
sdc_msgs__msg__Arrofarr__fini(sdc_msgs__msg__Arrofarr * msg)
{
  if (!msg) {
    return;
  }
  // header
  std_msgs__msg__Header__fini(&msg->header);
  // data
  sdc_msgs__msg__Arr__Sequence__fini(&msg->data);
}

bool
sdc_msgs__msg__Arrofarr__are_equal(const sdc_msgs__msg__Arrofarr * lhs, const sdc_msgs__msg__Arrofarr * rhs)
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
  // data
  if (!sdc_msgs__msg__Arr__Sequence__are_equal(
      &(lhs->data), &(rhs->data)))
  {
    return false;
  }
  return true;
}

bool
sdc_msgs__msg__Arrofarr__copy(
  const sdc_msgs__msg__Arrofarr * input,
  sdc_msgs__msg__Arrofarr * output)
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
  // data
  if (!sdc_msgs__msg__Arr__Sequence__copy(
      &(input->data), &(output->data)))
  {
    return false;
  }
  return true;
}

sdc_msgs__msg__Arrofarr *
sdc_msgs__msg__Arrofarr__create()
{
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  sdc_msgs__msg__Arrofarr * msg = (sdc_msgs__msg__Arrofarr *)allocator.allocate(sizeof(sdc_msgs__msg__Arrofarr), allocator.state);
  if (!msg) {
    return NULL;
  }
  memset(msg, 0, sizeof(sdc_msgs__msg__Arrofarr));
  bool success = sdc_msgs__msg__Arrofarr__init(msg);
  if (!success) {
    allocator.deallocate(msg, allocator.state);
    return NULL;
  }
  return msg;
}

void
sdc_msgs__msg__Arrofarr__destroy(sdc_msgs__msg__Arrofarr * msg)
{
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  if (msg) {
    sdc_msgs__msg__Arrofarr__fini(msg);
  }
  allocator.deallocate(msg, allocator.state);
}


bool
sdc_msgs__msg__Arrofarr__Sequence__init(sdc_msgs__msg__Arrofarr__Sequence * array, size_t size)
{
  if (!array) {
    return false;
  }
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  sdc_msgs__msg__Arrofarr * data = NULL;

  if (size) {
    data = (sdc_msgs__msg__Arrofarr *)allocator.zero_allocate(size, sizeof(sdc_msgs__msg__Arrofarr), allocator.state);
    if (!data) {
      return false;
    }
    // initialize all array elements
    size_t i;
    for (i = 0; i < size; ++i) {
      bool success = sdc_msgs__msg__Arrofarr__init(&data[i]);
      if (!success) {
        break;
      }
    }
    if (i < size) {
      // if initialization failed finalize the already initialized array elements
      for (; i > 0; --i) {
        sdc_msgs__msg__Arrofarr__fini(&data[i - 1]);
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
sdc_msgs__msg__Arrofarr__Sequence__fini(sdc_msgs__msg__Arrofarr__Sequence * array)
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
      sdc_msgs__msg__Arrofarr__fini(&array->data[i]);
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

sdc_msgs__msg__Arrofarr__Sequence *
sdc_msgs__msg__Arrofarr__Sequence__create(size_t size)
{
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  sdc_msgs__msg__Arrofarr__Sequence * array = (sdc_msgs__msg__Arrofarr__Sequence *)allocator.allocate(sizeof(sdc_msgs__msg__Arrofarr__Sequence), allocator.state);
  if (!array) {
    return NULL;
  }
  bool success = sdc_msgs__msg__Arrofarr__Sequence__init(array, size);
  if (!success) {
    allocator.deallocate(array, allocator.state);
    return NULL;
  }
  return array;
}

void
sdc_msgs__msg__Arrofarr__Sequence__destroy(sdc_msgs__msg__Arrofarr__Sequence * array)
{
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  if (array) {
    sdc_msgs__msg__Arrofarr__Sequence__fini(array);
  }
  allocator.deallocate(array, allocator.state);
}

bool
sdc_msgs__msg__Arrofarr__Sequence__are_equal(const sdc_msgs__msg__Arrofarr__Sequence * lhs, const sdc_msgs__msg__Arrofarr__Sequence * rhs)
{
  if (!lhs || !rhs) {
    return false;
  }
  if (lhs->size != rhs->size) {
    return false;
  }
  for (size_t i = 0; i < lhs->size; ++i) {
    if (!sdc_msgs__msg__Arrofarr__are_equal(&(lhs->data[i]), &(rhs->data[i]))) {
      return false;
    }
  }
  return true;
}

bool
sdc_msgs__msg__Arrofarr__Sequence__copy(
  const sdc_msgs__msg__Arrofarr__Sequence * input,
  sdc_msgs__msg__Arrofarr__Sequence * output)
{
  if (!input || !output) {
    return false;
  }
  if (output->capacity < input->size) {
    const size_t allocation_size =
      input->size * sizeof(sdc_msgs__msg__Arrofarr);
    rcutils_allocator_t allocator = rcutils_get_default_allocator();
    sdc_msgs__msg__Arrofarr * data =
      (sdc_msgs__msg__Arrofarr *)allocator.reallocate(
      output->data, allocation_size, allocator.state);
    if (!data) {
      return false;
    }
    // If reallocation succeeded, memory may or may not have been moved
    // to fulfill the allocation request, invalidating output->data.
    output->data = data;
    for (size_t i = output->capacity; i < input->size; ++i) {
      if (!sdc_msgs__msg__Arrofarr__init(&output->data[i])) {
        // If initialization of any new item fails, roll back
        // all previously initialized items. Existing items
        // in output are to be left unmodified.
        for (; i-- > output->capacity; ) {
          sdc_msgs__msg__Arrofarr__fini(&output->data[i]);
        }
        return false;
      }
    }
    output->capacity = input->size;
  }
  output->size = input->size;
  for (size_t i = 0; i < input->size; ++i) {
    if (!sdc_msgs__msg__Arrofarr__copy(
        &(input->data[i]), &(output->data[i])))
    {
      return false;
    }
  }
  return true;
}
