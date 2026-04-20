// generated from rosidl_generator_cpp/resource/idl__struct.hpp.em
// with input from sdc_msgs:msg/CoordinateList.idl
// generated code does not contain a copyright notice

#ifndef SDC_MSGS__MSG__DETAIL__COORDINATE_LIST__STRUCT_HPP_
#define SDC_MSGS__MSG__DETAIL__COORDINATE_LIST__STRUCT_HPP_

#include <algorithm>
#include <array>
#include <memory>
#include <string>
#include <vector>

#include "rosidl_runtime_cpp/bounded_vector.hpp"
#include "rosidl_runtime_cpp/message_initialization.hpp"


// Include directives for member types
// Member 'header'
#include "std_msgs/msg/detail/header__struct.hpp"
// Member 'cone_coordinates'
#include "sdc_msgs/msg/detail/coordinate__struct.hpp"

#ifndef _WIN32
# define DEPRECATED__sdc_msgs__msg__CoordinateList __attribute__((deprecated))
#else
# define DEPRECATED__sdc_msgs__msg__CoordinateList __declspec(deprecated)
#endif

namespace sdc_msgs
{

namespace msg
{

// message struct
template<class ContainerAllocator>
struct CoordinateList_
{
  using Type = CoordinateList_<ContainerAllocator>;

  explicit CoordinateList_(rosidl_runtime_cpp::MessageInitialization _init = rosidl_runtime_cpp::MessageInitialization::ALL)
  : header(_init)
  {
    if (rosidl_runtime_cpp::MessageInitialization::ALL == _init ||
      rosidl_runtime_cpp::MessageInitialization::ZERO == _init)
    {
      this->size = 0;
    }
  }

  explicit CoordinateList_(const ContainerAllocator & _alloc, rosidl_runtime_cpp::MessageInitialization _init = rosidl_runtime_cpp::MessageInitialization::ALL)
  : header(_alloc, _init)
  {
    if (rosidl_runtime_cpp::MessageInitialization::ALL == _init ||
      rosidl_runtime_cpp::MessageInitialization::ZERO == _init)
    {
      this->size = 0;
    }
  }

  // field types and members
  using _header_type =
    std_msgs::msg::Header_<ContainerAllocator>;
  _header_type header;
  using _size_type =
    uint8_t;
  _size_type size;
  using _cone_coordinates_type =
    std::vector<sdc_msgs::msg::Coordinate_<ContainerAllocator>, typename std::allocator_traits<ContainerAllocator>::template rebind_alloc<sdc_msgs::msg::Coordinate_<ContainerAllocator>>>;
  _cone_coordinates_type cone_coordinates;

  // setters for named parameter idiom
  Type & set__header(
    const std_msgs::msg::Header_<ContainerAllocator> & _arg)
  {
    this->header = _arg;
    return *this;
  }
  Type & set__size(
    const uint8_t & _arg)
  {
    this->size = _arg;
    return *this;
  }
  Type & set__cone_coordinates(
    const std::vector<sdc_msgs::msg::Coordinate_<ContainerAllocator>, typename std::allocator_traits<ContainerAllocator>::template rebind_alloc<sdc_msgs::msg::Coordinate_<ContainerAllocator>>> & _arg)
  {
    this->cone_coordinates = _arg;
    return *this;
  }

  // constant declarations

  // pointer types
  using RawPtr =
    sdc_msgs::msg::CoordinateList_<ContainerAllocator> *;
  using ConstRawPtr =
    const sdc_msgs::msg::CoordinateList_<ContainerAllocator> *;
  using SharedPtr =
    std::shared_ptr<sdc_msgs::msg::CoordinateList_<ContainerAllocator>>;
  using ConstSharedPtr =
    std::shared_ptr<sdc_msgs::msg::CoordinateList_<ContainerAllocator> const>;

  template<typename Deleter = std::default_delete<
      sdc_msgs::msg::CoordinateList_<ContainerAllocator>>>
  using UniquePtrWithDeleter =
    std::unique_ptr<sdc_msgs::msg::CoordinateList_<ContainerAllocator>, Deleter>;

  using UniquePtr = UniquePtrWithDeleter<>;

  template<typename Deleter = std::default_delete<
      sdc_msgs::msg::CoordinateList_<ContainerAllocator>>>
  using ConstUniquePtrWithDeleter =
    std::unique_ptr<sdc_msgs::msg::CoordinateList_<ContainerAllocator> const, Deleter>;
  using ConstUniquePtr = ConstUniquePtrWithDeleter<>;

  using WeakPtr =
    std::weak_ptr<sdc_msgs::msg::CoordinateList_<ContainerAllocator>>;
  using ConstWeakPtr =
    std::weak_ptr<sdc_msgs::msg::CoordinateList_<ContainerAllocator> const>;

  // pointer types similar to ROS 1, use SharedPtr / ConstSharedPtr instead
  // NOTE: Can't use 'using' here because GNU C++ can't parse attributes properly
  typedef DEPRECATED__sdc_msgs__msg__CoordinateList
    std::shared_ptr<sdc_msgs::msg::CoordinateList_<ContainerAllocator>>
    Ptr;
  typedef DEPRECATED__sdc_msgs__msg__CoordinateList
    std::shared_ptr<sdc_msgs::msg::CoordinateList_<ContainerAllocator> const>
    ConstPtr;

  // comparison operators
  bool operator==(const CoordinateList_ & other) const
  {
    if (this->header != other.header) {
      return false;
    }
    if (this->size != other.size) {
      return false;
    }
    if (this->cone_coordinates != other.cone_coordinates) {
      return false;
    }
    return true;
  }
  bool operator!=(const CoordinateList_ & other) const
  {
    return !this->operator==(other);
  }
};  // struct CoordinateList_

// alias to use template instance with default allocator
using CoordinateList =
  sdc_msgs::msg::CoordinateList_<std::allocator<void>>;

// constant definitions

}  // namespace msg

}  // namespace sdc_msgs

#endif  // SDC_MSGS__MSG__DETAIL__COORDINATE_LIST__STRUCT_HPP_
