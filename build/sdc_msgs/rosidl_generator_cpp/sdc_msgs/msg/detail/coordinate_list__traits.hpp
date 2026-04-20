// generated from rosidl_generator_cpp/resource/idl__traits.hpp.em
// with input from sdc_msgs:msg/CoordinateList.idl
// generated code does not contain a copyright notice

#ifndef SDC_MSGS__MSG__DETAIL__COORDINATE_LIST__TRAITS_HPP_
#define SDC_MSGS__MSG__DETAIL__COORDINATE_LIST__TRAITS_HPP_

#include <stdint.h>

#include <sstream>
#include <string>
#include <type_traits>

#include "sdc_msgs/msg/detail/coordinate_list__struct.hpp"
#include "rosidl_runtime_cpp/traits.hpp"

// Include directives for member types
// Member 'header'
#include "std_msgs/msg/detail/header__traits.hpp"
// Member 'cone_coordinates'
#include "sdc_msgs/msg/detail/coordinate__traits.hpp"

namespace sdc_msgs
{

namespace msg
{

inline void to_flow_style_yaml(
  const CoordinateList & msg,
  std::ostream & out)
{
  out << "{";
  // member: header
  {
    out << "header: ";
    to_flow_style_yaml(msg.header, out);
    out << ", ";
  }

  // member: size
  {
    out << "size: ";
    rosidl_generator_traits::value_to_yaml(msg.size, out);
    out << ", ";
  }

  // member: cone_coordinates
  {
    if (msg.cone_coordinates.size() == 0) {
      out << "cone_coordinates: []";
    } else {
      out << "cone_coordinates: [";
      size_t pending_items = msg.cone_coordinates.size();
      for (auto item : msg.cone_coordinates) {
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
  const CoordinateList & msg,
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

  // member: size
  {
    if (indentation > 0) {
      out << std::string(indentation, ' ');
    }
    out << "size: ";
    rosidl_generator_traits::value_to_yaml(msg.size, out);
    out << "\n";
  }

  // member: cone_coordinates
  {
    if (indentation > 0) {
      out << std::string(indentation, ' ');
    }
    if (msg.cone_coordinates.size() == 0) {
      out << "cone_coordinates: []\n";
    } else {
      out << "cone_coordinates:\n";
      for (auto item : msg.cone_coordinates) {
        if (indentation > 0) {
          out << std::string(indentation, ' ');
        }
        out << "-\n";
        to_block_style_yaml(item, out, indentation + 2);
      }
    }
  }
}  // NOLINT(readability/fn_size)

inline std::string to_yaml(const CoordinateList & msg, bool use_flow_style = false)
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
  const sdc_msgs::msg::CoordinateList & msg,
  std::ostream & out, size_t indentation = 0)
{
  sdc_msgs::msg::to_block_style_yaml(msg, out, indentation);
}

[[deprecated("use sdc_msgs::msg::to_yaml() instead")]]
inline std::string to_yaml(const sdc_msgs::msg::CoordinateList & msg)
{
  return sdc_msgs::msg::to_yaml(msg);
}

template<>
inline const char * data_type<sdc_msgs::msg::CoordinateList>()
{
  return "sdc_msgs::msg::CoordinateList";
}

template<>
inline const char * name<sdc_msgs::msg::CoordinateList>()
{
  return "sdc_msgs/msg/CoordinateList";
}

template<>
struct has_fixed_size<sdc_msgs::msg::CoordinateList>
  : std::integral_constant<bool, false> {};

template<>
struct has_bounded_size<sdc_msgs::msg::CoordinateList>
  : std::integral_constant<bool, false> {};

template<>
struct is_message<sdc_msgs::msg::CoordinateList>
  : std::true_type {};

}  // namespace rosidl_generator_traits

#endif  // SDC_MSGS__MSG__DETAIL__COORDINATE_LIST__TRAITS_HPP_
