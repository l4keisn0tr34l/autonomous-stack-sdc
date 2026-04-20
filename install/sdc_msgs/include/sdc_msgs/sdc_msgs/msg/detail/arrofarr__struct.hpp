// generated from rosidl_generator_cpp/resource/idl__struct.hpp.em
// with input from sdc_msgs:msg/Arrofarr.idl
// generated code does not contain a copyright notice

#ifndef SDC_MSGS__MSG__DETAIL__ARROFARR__STRUCT_HPP_
#define SDC_MSGS__MSG__DETAIL__ARROFARR__STRUCT_HPP_

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
// Member 'data'
#include "sdc_msgs/msg/detail/arr__struct.hpp"

#ifndef _WIN32
# define DEPRECATED__sdc_msgs__msg__Arrofarr __attribute__((deprecated))
#else
# define DEPRECATED__sdc_msgs__msg__Arrofarr __declspec(deprecated)
#endif

namespace sdc_msgs
{

namespace msg
{

// message struct
template<class ContainerAllocator>
struct Arrofarr_
{
  using Type = Arrofarr_<ContainerAllocator>;

  explicit Arrofarr_(rosidl_runtime_cpp::MessageInitialization _init = rosidl_runtime_cpp::MessageInitialization::ALL)
  : header(_init)
  {
    (void)_init;
  }

  explicit Arrofarr_(const ContainerAllocator & _alloc, rosidl_runtime_cpp::MessageInitialization _init = rosidl_runtime_cpp::MessageInitialization::ALL)
  : header(_alloc, _init)
  {
    (void)_init;
  }

  // field types and members
  using _header_type =
    std_msgs::msg::Header_<ContainerAllocator>;
  _header_type header;
  using _data_type =
    std::vector<sdc_msgs::msg::Arr_<ContainerAllocator>, typename std::allocator_traits<ContainerAllocator>::template rebind_alloc<sdc_msgs::msg::Arr_<ContainerAllocator>>>;
  _data_type data;

  // setters for named parameter idiom
  Type & set__header(
    const std_msgs::msg::Header_<ContainerAllocator> & _arg)
  {
    this->header = _arg;
    return *this;
  }
  Type & set__data(
    const std::vector<sdc_msgs::msg::Arr_<ContainerAllocator>, typename std::allocator_traits<ContainerAllocator>::template rebind_alloc<sdc_msgs::msg::Arr_<ContainerAllocator>>> & _arg)
  {
    this->data = _arg;
    return *this;
  }

  // constant declarations

  // pointer types
  using RawPtr =
    sdc_msgs::msg::Arrofarr_<ContainerAllocator> *;
  using ConstRawPtr =
    const sdc_msgs::msg::Arrofarr_<ContainerAllocator> *;
  using SharedPtr =
    std::shared_ptr<sdc_msgs::msg::Arrofarr_<ContainerAllocator>>;
  using ConstSharedPtr =
    std::shared_ptr<sdc_msgs::msg::Arrofarr_<ContainerAllocator> const>;

  template<typename Deleter = std::default_delete<
      sdc_msgs::msg::Arrofarr_<ContainerAllocator>>>
  using UniquePtrWithDeleter =
    std::unique_ptr<sdc_msgs::msg::Arrofarr_<ContainerAllocator>, Deleter>;

  using UniquePtr = UniquePtrWithDeleter<>;

  template<typename Deleter = std::default_delete<
      sdc_msgs::msg::Arrofarr_<ContainerAllocator>>>
  using ConstUniquePtrWithDeleter =
    std::unique_ptr<sdc_msgs::msg::Arrofarr_<ContainerAllocator> const, Deleter>;
  using ConstUniquePtr = ConstUniquePtrWithDeleter<>;

  using WeakPtr =
    std::weak_ptr<sdc_msgs::msg::Arrofarr_<ContainerAllocator>>;
  using ConstWeakPtr =
    std::weak_ptr<sdc_msgs::msg::Arrofarr_<ContainerAllocator> const>;

  // pointer types similar to ROS 1, use SharedPtr / ConstSharedPtr instead
  // NOTE: Can't use 'using' here because GNU C++ can't parse attributes properly
  typedef DEPRECATED__sdc_msgs__msg__Arrofarr
    std::shared_ptr<sdc_msgs::msg::Arrofarr_<ContainerAllocator>>
    Ptr;
  typedef DEPRECATED__sdc_msgs__msg__Arrofarr
    std::shared_ptr<sdc_msgs::msg::Arrofarr_<ContainerAllocator> const>
    ConstPtr;

  // comparison operators
  bool operator==(const Arrofarr_ & other) const
  {
    if (this->header != other.header) {
      return false;
    }
    if (this->data != other.data) {
      return false;
    }
    return true;
  }
  bool operator!=(const Arrofarr_ & other) const
  {
    return !this->operator==(other);
  }
};  // struct Arrofarr_

// alias to use template instance with default allocator
using Arrofarr =
  sdc_msgs::msg::Arrofarr_<std::allocator<void>>;

// constant definitions

}  // namespace msg

}  // namespace sdc_msgs

#endif  // SDC_MSGS__MSG__DETAIL__ARROFARR__STRUCT_HPP_
