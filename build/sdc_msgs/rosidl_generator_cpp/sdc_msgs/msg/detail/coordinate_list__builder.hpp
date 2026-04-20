// generated from rosidl_generator_cpp/resource/idl__builder.hpp.em
// with input from sdc_msgs:msg/CoordinateList.idl
// generated code does not contain a copyright notice

#ifndef SDC_MSGS__MSG__DETAIL__COORDINATE_LIST__BUILDER_HPP_
#define SDC_MSGS__MSG__DETAIL__COORDINATE_LIST__BUILDER_HPP_

#include <algorithm>
#include <utility>

#include "sdc_msgs/msg/detail/coordinate_list__struct.hpp"
#include "rosidl_runtime_cpp/message_initialization.hpp"


namespace sdc_msgs
{

namespace msg
{

namespace builder
{

class Init_CoordinateList_cone_coordinates
{
public:
  explicit Init_CoordinateList_cone_coordinates(::sdc_msgs::msg::CoordinateList & msg)
  : msg_(msg)
  {}
  ::sdc_msgs::msg::CoordinateList cone_coordinates(::sdc_msgs::msg::CoordinateList::_cone_coordinates_type arg)
  {
    msg_.cone_coordinates = std::move(arg);
    return std::move(msg_);
  }

private:
  ::sdc_msgs::msg::CoordinateList msg_;
};

class Init_CoordinateList_size
{
public:
  explicit Init_CoordinateList_size(::sdc_msgs::msg::CoordinateList & msg)
  : msg_(msg)
  {}
  Init_CoordinateList_cone_coordinates size(::sdc_msgs::msg::CoordinateList::_size_type arg)
  {
    msg_.size = std::move(arg);
    return Init_CoordinateList_cone_coordinates(msg_);
  }

private:
  ::sdc_msgs::msg::CoordinateList msg_;
};

class Init_CoordinateList_header
{
public:
  Init_CoordinateList_header()
  : msg_(::rosidl_runtime_cpp::MessageInitialization::SKIP)
  {}
  Init_CoordinateList_size header(::sdc_msgs::msg::CoordinateList::_header_type arg)
  {
    msg_.header = std::move(arg);
    return Init_CoordinateList_size(msg_);
  }

private:
  ::sdc_msgs::msg::CoordinateList msg_;
};

}  // namespace builder

}  // namespace msg

template<typename MessageType>
auto build();

template<>
inline
auto build<::sdc_msgs::msg::CoordinateList>()
{
  return sdc_msgs::msg::builder::Init_CoordinateList_header();
}

}  // namespace sdc_msgs

#endif  // SDC_MSGS__MSG__DETAIL__COORDINATE_LIST__BUILDER_HPP_
