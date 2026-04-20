// generated from rosidl_generator_cpp/resource/idl__traits.hpp.em
// with input from sdc_msgs:msg/Arrofarr.idl
// generated code does not contain a copyright notice

#ifndef SDC_MSGS__MSG__DETAIL__ARROFARR__TRAITS_HPP_
#define SDC_MSGS__MSG__DETAIL__ARROFARR__TRAITS_HPP_

#include <stdint.h>

#include <sstream>
#include <string>
#include <type_traits>

#include "sdc_msgs/msg/detail/arrofarr__struct.hpp"
#include "rosidl_runtime_cpp/traits.hpp"

// Include directives for member types
// Member 'header'
#include "std_msgs/msg/detail/header__traits.hpp"
// Member 'data'
#include "sdc_msgs/msg/detail/arr__traits.hpp"

namespace sdc_msgs
{

namespace msg
{

inline void to_flow_style_yaml(
  const Arrofarr & msg,
  std::ostream & out)
{
  out << "{";
  // member: header
  {
    out << "header: ";
    to_flow_style_yaml(msg.header, out);
    out << ", ";
  }

  // member: data
  {
    if (msg.data.size() == 0) {
      out << "data: []";
    } else {
      out << "data: [";
      size_t pending_items = msg.data.size();
      for (auto item : msg.data) {
        to_flow_style_yaml(item, out);
        if (--pending_items > 0) {
          out << ", ";
        }
      }
      out << "]";
    }
  }
  out << "}";
}  // NOLINT(readability/fn_size)

inline void to_block_style_yaml(
  const Arrofarr & msg,
  std::ostream & out, size_t indentation = 0)
{
  // member: header
  {
    if (indentation > 0) {
      out << std::string(indentation, ' ');
    }
    out << "header:\n";
    to_block_style_yaml(msg.header, out, indentation + 2);
  }

  // member: data
  {
    if (indentation > 0) {
      out << std::string(indentation, ' ');
    }
    if (msg.data.size() == 0) {
      out << "data: []\n";
    } else {
      out << "data:\n";
      for (auto item : msg.data) {
        if (indentation > 0) {
          out << std::string(indentation, ' ');
        }
        out << "-\n";
        to_block_style_yaml(item, out, indentation + 2);
      }
    }
  }
}  // NOLINT(readability/fn_size)

inline std::string to_yaml(const Arrofarr & msg, bool use_flow_style = false)
{
  std::ostringstream out;
  if (use_flow_style) {
    to_flow_style_yaml(msg, out);
  } else {
    to_block_style_yaml(msg, out);
  }
  return out.str();
}

}  // namespace msg

}  // namespace sdc_msgs

namespace rosidl_generator_traits
{

[[deprecated("use sdc_msgs::msg::to_block_style_yaml() instead")]]
inline void to_yaml(
  const sdc_msgs::msg::Arrofarr & msg,
  std::ostream & out, size_t indentation = 0)
{
  sdc_msgs::msg::to_block_style_yaml(msg, out, indentation);
}

[[deprecated("use sdc_msgs::msg::to_yaml() instead")]]
inline std::string to_yaml(const sdc_msgs::msg::Arrofarr & msg)
{
  return sdc_msgs::msg::to_yaml(msg);
}

template<>
inline const char * data_type<sdc_msgs::msg::Arrofarr>()
{
  return "sdc_msgs::msg::Arrofarr";
}

template<>
inline const char * name<sdc_msgs::msg::Arrofarr>()
{
  return "sdc_msgs/msg/Arrofarr";
}

template<>
struct has_fixed_size<sdc_msgs::msg::Arrofarr>
  : std::integral_constant<bool, false> {};

template<>
struct has_bounded_size<sdc_msgs::msg::Arrofarr>
  : std::integral_constant<bool, false> {};

template<>
struct is_message<sdc_msgs::msg::Arrofarr>
  : std::true_type {};

}  // namespace rosidl_generator_traits

#endif  // SDC_MSGS__MSG__DETAIL__ARROFARR__TRAITS_HPP_
