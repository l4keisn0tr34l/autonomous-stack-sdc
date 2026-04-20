// generated from rosidl_generator_cpp/resource/idl__builder.hpp.em
// with input from sdc_msgs:msg/Arr.idl
// generated code does not contain a copyright notice

#ifndef SDC_MSGS__MSG__DETAIL__ARR__BUILDER_HPP_
#define SDC_MSGS__MSG__DETAIL__ARR__BUILDER_HPP_

#include <algorithm>
#include <utility>

#include "sdc_msgs/msg/detail/arr__struct.hpp"
#include "rosidl_runtime_cpp/message_initialization.hpp"


namespace sdc_msgs
{

namespace msg
{

namespace builder
{

class Init_Arr_data
{
public:
  Init_Arr_data()
  : msg_(::rosidl_runtime_cpp::MessageInitialization::SKIP)
  {}
  ::sdc_msgs::msg::Arr data(::sdc_msgs::msg::Arr::_data_type arg)
  {
    msg_.data = std::move(arg);
    return std::move(msg_);
  }

private:
  ::sdc_msgs::msg::Arr msg_;
};

}  // namespace builder

}  // namespace msg

template<typename MessageType>
auto build();

template<>
inline
auto build<::sdc_msgs::msg::Arr>()
{
  return sdc_msgs::msg::builder::Init_Arr_data();
}

}  // namespace sdc_msgs

#endif  // SDC_MSGS__MSG__DETAIL__ARR__BUILDER_HPP_
