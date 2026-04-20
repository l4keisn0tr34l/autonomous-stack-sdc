// generated from rosidl_generator_cpp/resource/idl__builder.hpp.em
// with input from sdc_msgs:msg/Coordinate.idl
// generated code does not contain a copyright notice

#ifndef SDC_MSGS__MSG__DETAIL__COORDINATE__BUILDER_HPP_
#define SDC_MSGS__MSG__DETAIL__COORDINATE__BUILDER_HPP_

#include <algorithm>
#include <utility>

#include "sdc_msgs/msg/detail/coordinate__struct.hpp"
#include "rosidl_runtime_cpp/message_initialization.hpp"


namespace sdc_msgs
{

namespace msg
{

namespace builder
{

class Init_Coordinate_bottom
{
public:
  explicit Init_Coordinate_bottom(::sdc_msgs::msg::Coordinate & msg)
  : msg_(msg)
  {}
  ::sdc_msgs::msg::Coordinate bottom(::sdc_msgs::msg::Coordinate::_bottom_type arg)
  {
    msg_.bottom = std::move(arg);
    return std::move(msg_);
  }

private:
  ::sdc_msgs::msg::Coordinate msg_;
};

class Init_Coordinate_top
{
public:
  explicit Init_Coordinate_top(::sdc_msgs::msg::Coordinate & msg)
  : msg_(msg)
  {}
  Init_Coordinate_bottom top(::sdc_msgs::msg::Coordinate::_top_type arg)
  {
    msg_.top = std::move(arg);
    return Init_Coordinate_bottom(msg_);
  }

private:
  ::sdc_msgs::msg::Coordinate msg_;
};

class Init_Coordinate_back
{
public:
  explicit Init_Coordinate_back(::sdc_msgs::msg::Coordinate & msg)
  : msg_(msg)
  {}
  Init_Coordinate_top back(::sdc_msgs::msg::Coordinate::_back_type arg)
  {
    msg_.back = std::move(arg);
    return Init_Coordinate_top(msg_);
  }

private:
  ::sdc_msgs::msg::Coordinate msg_;
};

class Init_Coordinate_front
{
public:
  explicit Init_Coordinate_front(::sdc_msgs::msg::Coordinate & msg)
  : msg_(msg)
  {}
  Init_Coordinate_back front(::sdc_msgs::msg::Coordinate::_front_type arg)
  {
    msg_.front = std::move(arg);
    return Init_Coordinate_back(msg_);
  }

private:
  ::sdc_msgs::msg::Coordinate msg_;
};

class Init_Coordinate_right
{
public:
  explicit Init_Coordinate_right(::sdc_msgs::msg::Coordinate & msg)
  : msg_(msg)
  {}
  Init_Coordinate_front right(::sdc_msgs::msg::Coordinate::_right_type arg)
  {
    msg_.right = std::move(arg);
    return Init_Coordinate_front(msg_);
  }

private:
  ::sdc_msgs::msg::Coordinate msg_;
};

class Init_Coordinate_left
{
public:
  explicit Init_Coordinate_left(::sdc_msgs::msg::Coordinate & msg)
  : msg_(msg)
  {}
  Init_Coordinate_right left(::sdc_msgs::msg::Coordinate::_left_type arg)
  {
    msg_.left = std::move(arg);
    return Init_Coordinate_right(msg_);
  }

private:
  ::sdc_msgs::msg::Coordinate msg_;
};

class Init_Coordinate_colour
{
public:
  explicit Init_Coordinate_colour(::sdc_msgs::msg::Coordinate & msg)
  : msg_(msg)
  {}
  Init_Coordinate_left colour(::sdc_msgs::msg::Coordinate::_colour_type arg)
  {
    msg_.colour = std::move(arg);
    return Init_Coordinate_left(msg_);
  }

private:
  ::sdc_msgs::msg::Coordinate msg_;
};

class Init_Coordinate_z
{
public:
  explicit Init_Coordinate_z(::sdc_msgs::msg::Coordinate & msg)
  : msg_(msg)
  {}
  Init_Coordinate_colour z(::sdc_msgs::msg::Coordinate::_z_type arg)
  {
    msg_.z = std::move(arg);
    return Init_Coordinate_colour(msg_);
  }

private:
  ::sdc_msgs::msg::Coordinate msg_;
};

class Init_Coordinate_y
{
public:
  explicit Init_Coordinate_y(::sdc_msgs::msg::Coordinate & msg)
  : msg_(msg)
  {}
  Init_Coordinate_z y(::sdc_msgs::msg::Coordinate::_y_type arg)
  {
    msg_.y = std::move(arg);
    return Init_Coordinate_z(msg_);
  }

private:
  ::sdc_msgs::msg::Coordinate msg_;
};

class Init_Coordinate_x
{
public:
  explicit Init_Coordinate_x(::sdc_msgs::msg::Coordinate & msg)
  : msg_(msg)
  {}
  Init_Coordinate_y x(::sdc_msgs::msg::Coordinate::_x_type arg)
  {
    msg_.x = std::move(arg);
    return Init_Coordinate_y(msg_);
  }

private:
  ::sdc_msgs::msg::Coordinate msg_;
};

class Init_Coordinate_reconsize
{
public:
  explicit Init_Coordinate_reconsize(::sdc_msgs::msg::Coordinate & msg)
  : msg_(msg)
  {}
  Init_Coordinate_x reconsize(::sdc_msgs::msg::Coordinate::_reconsize_type arg)
  {
    msg_.reconsize = std::move(arg);
    return Init_Coordinate_x(msg_);
  }

private:
  ::sdc_msgs::msg::Coordinate msg_;
};

class Init_Coordinate_size
{
public:
  Init_Coordinate_size()
  : msg_(::rosidl_runtime_cpp::MessageInitialization::SKIP)
  {}
  Init_Coordinate_reconsize size(::sdc_msgs::msg::Coordinate::_size_type arg)
  {
    msg_.size = std::move(arg);
    return Init_Coordinate_reconsize(msg_);
  }

private:
  ::sdc_msgs::msg::Coordinate msg_;
};

}  // namespace builder

}  // namespace msg

template<typename MessageType>
auto build();

template<>
inline
auto build<::sdc_msgs::msg::Coordinate>()
{
  return sdc_msgs::msg::builder::Init_Coordinate_size();
}

}  // namespace sdc_msgs

#endif  // SDC_MSGS__MSG__DETAIL__COORDINATE__BUILDER_HPP_
