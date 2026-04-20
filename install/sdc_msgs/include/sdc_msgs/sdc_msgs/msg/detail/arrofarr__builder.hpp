// generated from rosidl_generator_cpp/resource/idl__builder.hpp.em
// with input from sdc_msgs:msg/Arrofarr.idl
// generated code does not contain a copyright notice

#ifndef SDC_MSGS__MSG__DETAIL__ARROFARR__BUILDER_HPP_
#define SDC_MSGS__MSG__DETAIL__ARROFARR__BUILDER_HPP_

#include <algorithm>
#include <utility>

#include "sdc_msgs/msg/detail/arrofarr__struct.hpp"
#include "rosidl_runtime_cpp/message_initialization.hpp"


namespace sdc_msgs
{

namespace msg
{

namespace builder
{

class Init_Arrofarr_data
{
public:
  explicit Init_Arrofarr_data(::sdc_msgs::msg::Arrofarr & msg)
  : msg_(msg)
  {}
  ::sdc_msgs::msg::Arrofarr data(::sdc_msgs::msg::Arrofarr::_data_type arg)
  {
    msg_.data = std::move(arg);
    return std::move(msg_);
  }

private:
  ::sdc_msgs::msg::Arrofarr msg_;
};

class Init_Arrofarr_header
{
public:
  Init_Arrofarr_header()
  : msg_(::rosidl_runtime_cpp::MessageInitialization::SKIP)
  {}
  Init_Arrofarr_data header(::sdc_msgs::msg::Arrofarr::_header_type arg)
  {
    msg_.header = std::move(arg);
    return Init_Arrofarr_data(msg_);
  }

private:
  ::sdc_msgs::msg::Arrofarr msg_;
};

}  // namespace builder

}  // namespace msg

template<typename MessageType>
auto build();

template<>
inline
auto build<::sdc_msgs::msg::Arrofarr>()
{
  return sdc_msgs::msg::builder::Init_Arrofarr_header();
}

}  // namespace sdc_msgs

#endif  // SDC_MSGS__MSG__DETAIL__ARROFARR__BUILDER_HPP_
