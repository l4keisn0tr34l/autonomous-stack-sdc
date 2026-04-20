// generated from rosidl_generator_cpp/resource/idl__struct.hpp.em
// with input from sdc_msgs:msg/Coordinate.idl
// generated code does not contain a copyright notice

#ifndef SDC_MSGS__MSG__DETAIL__COORDINATE__STRUCT_HPP_
#define SDC_MSGS__MSG__DETAIL__COORDINATE__STRUCT_HPP_

#include <algorithm>
#include <array>
#include <memory>
#include <string>
#include <vector>

#include "rosidl_runtime_cpp/bounded_vector.hpp"
#include "rosidl_runtime_cpp/message_initialization.hpp"


#ifndef _WIN32
# define DEPRECATED__sdc_msgs__msg__Coordinate __attribute__((deprecated))
#else
# define DEPRECATED__sdc_msgs__msg__Coordinate __declspec(deprecated)
#endif

namespace sdc_msgs
{

namespace msg
{

// message struct
template<class ContainerAllocator>
struct Coordinate_
{
  using Type = Coordinate_<ContainerAllocator>;

  explicit Coordinate_(rosidl_runtime_cpp::MessageInitialization _init = rosidl_runtime_cpp::MessageInitialization::ALL)
  {
    if (rosidl_runtime_cpp::MessageInitialization::ALL == _init ||
      rosidl_runtime_cpp::MessageInitialization::ZERO == _init)
    {
      this->size = 0;
      this->reconsize = 0;
      this->x = 0.0f;
      this->y = 0.0f;
      this->z = 0.0f;
      this->colour = 0.0f;
    }
  }

  explicit Coordinate_(const ContainerAllocator & _alloc, rosidl_runtime_cpp::MessageInitialization _init = rosidl_runtime_cpp::MessageInitialization::ALL)
  {
    (void)_alloc;
    if (rosidl_runtime_cpp::MessageInitialization::ALL == _init ||
      rosidl_runtime_cpp::MessageInitialization::ZERO == _init)
    {
      this->size = 0;
      this->reconsize = 0;
      this->x = 0.0f;
      this->y = 0.0f;
      this->z = 0.0f;
      this->colour = 0.0f;
    }
  }

  // field types and members
  using _size_type =
    uint8_t;
  _size_type size;
  using _reconsize_type =
    uint8_t;
  _reconsize_type reconsize;
  using _x_type =
    float;
  _x_type x;
  using _y_type =
    float;
  _y_type y;
  using _z_type =
    float;
  _z_type z;
  using _colour_type =
    float;
  _colour_type colour;
  using _left_type =
    std::vector<float, typename std::allocator_traits<ContainerAllocator>::template rebind_alloc<float>>;
  _left_type left;
  using _right_type =
    std::vector<float, typename std::allocator_traits<ContainerAllocator>::template rebind_alloc<float>>;
  _right_type right;
  using _front_type =
    std::vector<float, typename std::allocator_traits<ContainerAllocator>::template rebind_alloc<float>>;
  _front_type front;
  using _back_type =
    std::vector<float, typename std::allocator_traits<ContainerAllocator>::template rebind_alloc<float>>;
  _back_type back;
  using _top_type =
    std::vector<float, typename std::allocator_traits<ContainerAllocator>::template rebind_alloc<float>>;
  _top_type top;
  using _bottom_type =
    std::vector<float, typename std::allocator_traits<ContainerAllocator>::template rebind_alloc<float>>;
  _bottom_type bottom;

  // setters for named parameter idiom
  Type & set__size(
    const uint8_t & _arg)
  {
    this->size = _arg;
    return *this;
  }
  Type & set__reconsize(
    const uint8_t & _arg)
  {
    this->reconsize = _arg;
    return *this;
  }
  Type & set__x(
    const float & _arg)
  {
    this->x = _arg;
    return *this;
  }
  Type & set__y(
    const float & _arg)
  {
    this->y = _arg;
    return *this;
  }
  Type & set__z(
    const float & _arg)
  {
    this->z = _arg;
    return *this;
  }
  Type & set__colour(
    const float & _arg)
  {
    this->colour = _arg;
    return *this;
  }
  Type & set__left(
    const std::vector<float, typename std::allocator_traits<ContainerAllocator>::template rebind_alloc<float>> & _arg)
  {
    this->left = _arg;
    return *this;
  }
  Type & set__right(
    const std::vector<float, typename std::allocator_traits<ContainerAllocator>::template rebind_alloc<float>> & _arg)
  {
    this->right = _arg;
    return *this;
  }
  Type & set__front(
    const std::vector<float, typename std::allocator_traits<ContainerAllocator>::template rebind_alloc<float>> & _arg)
  {
    this->front = _arg;
    return *this;
  }
  Type & set__back(
    const std::vector<float, typename std::allocator_traits<ContainerAllocator>::template rebind_alloc<float>> & _arg)
  {
    this->back = _arg;
    return *this;
  }
  Type & set__top(
    const std::vector<float, typename std::allocator_traits<ContainerAllocator>::template rebind_alloc<float>> & _arg)
  {
    this->top = _arg;
    return *this;
  }
  Type & set__bottom(
    const std::vector<float, typename std::allocator_traits<ContainerAllocator>::template rebind_alloc<float>> & _arg)
  {
    this->bottom = _arg;
    return *this;
  }

  // constant declarations

  // pointer types
  using RawPtr =
    sdc_msgs::msg::Coordinate_<ContainerAllocator> *;
  using ConstRawPtr =
    const sdc_msgs::msg::Coordinate_<ContainerAllocator> *;
  using SharedPtr =
    std::shared_ptr<sdc_msgs::msg::Coordinate_<ContainerAllocator>>;
  using ConstSharedPtr =
    std::shared_ptr<sdc_msgs::msg::Coordinate_<ContainerAllocator> const>;

  template<typename Deleter = std::default_delete<
      sdc_msgs::msg::Coordinate_<ContainerAllocator>>>
  using UniquePtrWithDeleter =
    std::unique_ptr<sdc_msgs::msg::Coordinate_<ContainerAllocator>, Deleter>;

  using UniquePtr = UniquePtrWithDeleter<>;

  template<typename Deleter = std::default_delete<
      sdc_msgs::msg::Coordinate_<ContainerAllocator>>>
  using ConstUniquePtrWithDeleter =
    std::unique_ptr<sdc_msgs::msg::Coordinate_<ContainerAllocator> const, Deleter>;
  using ConstUniquePtr = ConstUniquePtrWithDeleter<>;

  using WeakPtr =
    std::weak_ptr<sdc_msgs::msg::Coordinate_<ContainerAllocator>>;
  using ConstWeakPtr =
    std::weak_ptr<sdc_msgs::msg::Coordinate_<ContainerAllocator> const>;

  // pointer types similar to ROS 1, use SharedPtr / ConstSharedPtr instead
  // NOTE: Can't use 'using' here because GNU C++ can't parse attributes properly
  typedef DEPRECATED__sdc_msgs__msg__Coordinate
    std::shared_ptr<sdc_msgs::msg::Coordinate_<ContainerAllocator>>
    Ptr;
  typedef DEPRECATED__sdc_msgs__msg__Coordinate
    std::shared_ptr<sdc_msgs::msg::Coordinate_<ContainerAllocator> const>
    ConstPtr;

  // comparison operators
  bool operator==(const Coordinate_ & other) const
  {
    if (this->size != other.size) {
      return false;
    }
    if (this->reconsize != other.reconsize) {
      return false;
    }
    if (this->x != other.x) {
      return false;
    }
    if (this->y != other.y) {
      return false;
    }
    if (this->z != other.z) {
      return false;
    }
    if (this->colour != other.colour) {
      return false;
    }
    if (this->left != other.left) {
      return false;
    }
    if (this->right != other.right) {
      return false;
    }
    if (this->front != other.front) {
      return false;
    }
    if (this->back != other.back) {
      return false;
    }
    if (this->top != other.top) {
      return false;
    }
    if (this->bottom != other.bottom) {
      return false;
    }
    return true;
  }
  bool operator!=(const Coordinate_ & other) const
  {
    return !this->operator==(other);
  }
};  // struct Coordinate_

// alias to use template instance with default allocator
using Coordinate =
  sdc_msgs::msg::Coordinate_<std::allocator<void>>;

// constant definitions

}  // namespace msg

}  // namespace sdc_msgs

#endif  // SDC_MSGS__MSG__DETAIL__COORDINATE__STRUCT_HPP_
