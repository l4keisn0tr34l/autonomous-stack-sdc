// generated from rosidl_generator_cpp/resource/idl__traits.hpp.em
// with input from sdc_msgs:msg/Coordinate.idl
// generated code does not contain a copyright notice

#ifndef SDC_MSGS__MSG__DETAIL__COORDINATE__TRAITS_HPP_
#define SDC_MSGS__MSG__DETAIL__COORDINATE__TRAITS_HPP_

#include <stdint.h>

#include <sstream>
#include <string>
#include <type_traits>

#include "sdc_msgs/msg/detail/coordinate__struct.hpp"
#include "rosidl_runtime_cpp/traits.hpp"

namespace sdc_msgs
{

namespace msg
{

inline void to_flow_style_yaml(
  const Coordinate & msg,
  std::ostream & out)
{
  out << "{";
  // member: size
  {
    out << "size: ";
    rosidl_generator_traits::value_to_yaml(msg.size, out);
    out << ", ";
  }

  // member: reconsize
  {
    out << "reconsize: ";
    rosidl_generator_traits::value_to_yaml(msg.reconsize, out);
    out << ", ";
  }

  // member: x
  {
    out << "x: ";
    rosidl_generator_traits::value_to_yaml(msg.x, out);
    out << ", ";
  }

  // member: y
  {
    out << "y: ";
    rosidl_generator_traits::value_to_yaml(msg.y, out);
    out << ", ";
  }

  // member: z
  {
    out << "z: ";
    rosidl_generator_traits::value_to_yaml(msg.z, out);
    out << ", ";
  }

  // member: colour
  {
    out << "colour: ";
    rosidl_generator_traits::value_to_yaml(msg.colour, out);
    out << ", ";
  }

  // member: left
  {
    if (msg.left.size() == 0) {
      out << "left: []";
    } else {
      out << "left: [";
      size_t pending_items = msg.left.size();
      for (auto item : msg.left) {
        rosidl_generator_traits::value_to_yaml(item, out);
        if (--pending_items > 0) {
          out << ", ";
        }
      }
      out << "]";
    }
    out << ", ";
  }

  // member: right
  {
    if (msg.right.size() == 0) {
      out << "right: []";
    } else {
      out << "right: [";
      size_t pending_items = msg.right.size();
      for (auto item : msg.right) {
        rosidl_generator_traits::value_to_yaml(item, out);
        if (--pending_items > 0) {
          out << ", ";
        }
      }
      out << "]";
    }
    out << ", ";
  }

  // member: front
  {
    if (msg.front.size() == 0) {
      out << "front: []";
    } else {
      out << "front: [";
      size_t pending_items = msg.front.size();
      for (auto item : msg.front) {
        rosidl_generator_traits::value_to_yaml(item, out);
        if (--pending_items > 0) {
          out << ", ";
        }
      }
      out << "]";
    }
    out << ", ";
  }

  // member: back
  {
    if (msg.back.size() == 0) {
      out << "back: []";
    } else {
      out << "back: [";
      size_t pending_items = msg.back.size();
      for (auto item : msg.back) {
        rosidl_generator_traits::value_to_yaml(item, out);
        if (--pending_items > 0) {
          out << ", ";
        }
      }
      out << "]";
    }
    out << ", ";
  }

  // member: top
  {
    if (msg.top.size() == 0) {
      out << "top: []";
    } else {
      out << "top: [";
      size_t pending_items = msg.top.size();
      for (auto item : msg.top) {
        rosidl_generator_traits::value_to_yaml(item, out);
        if (--pending_items > 0) {
          out << ", ";
        }
      }
      out << "]";
    }
    out << ", ";
  }

  // member: bottom
  {
    if (msg.bottom.size() == 0) {
      out << "bottom: []";
    } else {
      out << "bottom: [";
      size_t pending_items = msg.bottom.size();
      for (auto item : msg.bottom) {
        rosidl_generator_traits::value_to_yaml(item, out);
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
  const Coordinate & msg,
  std::ostream & out, size_t indentation = 0)
{
  // member: size
  {
    if (indentation > 0) {
      out << std::string(indentation, ' ');
    }
    out << "size: ";
    rosidl_generator_traits::value_to_yaml(msg.size, out);
    out << "\n";
  }

  // member: reconsize
  {
    if (indentation > 0) {
      out << std::string(indentation, ' ');
    }
    out << "reconsize: ";
    rosidl_generator_traits::value_to_yaml(msg.reconsize, out);
    out << "\n";
  }

  // member: x
  {
    if (indentation > 0) {
      out << std::string(indentation, ' ');
    }
    out << "x: ";
    rosidl_generator_traits::value_to_yaml(msg.x, out);
    out << "\n";
  }

  // member: y
  {
    if (indentation > 0) {
      out << std::string(indentation, ' ');
    }
    out << "y: ";
    rosidl_generator_traits::value_to_yaml(msg.y, out);
    out << "\n";
  }

  // member: z
  {
    if (indentation > 0) {
      out << std::string(indentation, ' ');
    }
    out << "z: ";
    rosidl_generator_traits::value_to_yaml(msg.z, out);
    out << "\n";
  }

  // member: colour
  {
    if (indentation > 0) {
      out << std::string(indentation, ' ');
    }
    out << "colour: ";
    rosidl_generator_traits::value_to_yaml(msg.colour, out);
    out << "\n";
  }

  // member: left
  {
    if (indentation > 0) {
      out << std::string(indentation, ' ');
    }
    if (msg.left.size() == 0) {
      out << "left: []\n";
    } else {
      out << "left:\n";
      for (auto item : msg.left) {
        if (indentation > 0) {
          out << std::string(indentation, ' ');
        }
        out << "- ";
        rosidl_generator_traits::value_to_yaml(item, out);
        out << "\n";
      }
    }
  }

  // member: right
  {
    if (indentation > 0) {
      out << std::string(indentation, ' ');
    }
    if (msg.right.size() == 0) {
      out << "right: []\n";
    } else {
      out << "right:\n";
      for (auto item : msg.right) {
        if (indentation > 0) {
          out << std::string(indentation, ' ');
        }
        out << "- ";
        rosidl_generator_traits::value_to_yaml(item, out);
        out << "\n";
      }
    }
  }

  // member: front
  {
    if (indentation > 0) {
      out << std::string(indentation, ' ');
    }
    if (msg.front.size() == 0) {
      out << "front: []\n";
    } else {
      out << "front:\n";
      for (auto item : msg.front) {
        if (indentation > 0) {
          out << std::string(indentation, ' ');
        }
        out << "- ";
        rosidl_generator_traits::value_to_yaml(item, out);
        out << "\n";
      }
    }
  }

  // member: back
  {
    if (indentation > 0) {
      out << std::string(indentation, ' ');
    }
    if (msg.back.size() == 0) {
      out << "back: []\n";
    } else {
      out << "back:\n";
      for (auto item : msg.back) {
        if (indentation > 0) {
          out << std::string(indentation, ' ');
        }
        out << "- ";
        rosidl_generator_traits::value_to_yaml(item, out);
        out << "\n";
      }
    }
  }

  // member: top
  {
    if (indentation > 0) {
      out << std::string(indentation, ' ');
    }
    if (msg.top.size() == 0) {
      out << "top: []\n";
    } else {
      out << "top:\n";
      for (auto item : msg.top) {
        if (indentation > 0) {
          out << std::string(indentation, ' ');
        }
        out << "- ";
        rosidl_generator_traits::value_to_yaml(item, out);
        out << "\n";
      }
    }
  }

  // member: bottom
  {
    if (indentation > 0) {
      out << std::string(indentation, ' ');
    }
    if (msg.bottom.size() == 0) {
      out << "bottom: []\n";
    } else {
      out << "bottom:\n";
      for (auto item : msg.bottom) {
        if (indentation > 0) {
          out << std::string(indentation, ' ');
        }
        out << "- ";
        rosidl_generator_traits::value_to_yaml(item, out);
        out << "\n";
      }
    }
  }
}  // NOLINT(readability/fn_size)

inline std::string to_yaml(const Coordinate & msg, bool use_flow_style = false)
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
  const sdc_msgs::msg::Coordinate & msg,
  std::ostream & out, size_t indentation = 0)
{
  sdc_msgs::msg::to_block_style_yaml(msg, out, indentation);
}

[[deprecated("use sdc_msgs::msg::to_yaml() instead")]]
inline std::string to_yaml(const sdc_msgs::msg::Coordinate & msg)
{
  return sdc_msgs::msg::to_yaml(msg);
}

template<>
inline const char * data_type<sdc_msgs::msg::Coordinate>()
{
  return "sdc_msgs::msg::Coordinate";
}

template<>
inline const char * name<sdc_msgs::msg::Coordinate>()
{
  return "sdc_msgs/msg/Coordinate";
}

template<>
struct has_fixed_size<sdc_msgs::msg::Coordinate>
  : std::integral_constant<bool, false> {};

template<>
struct has_bounded_size<sdc_msgs::msg::Coordinate>
  : std::integral_constant<bool, false> {};

template<>
struct is_message<sdc_msgs::msg::Coordinate>
  : std::true_type {};

}  // namespace rosidl_generator_traits

#endif  // SDC_MSGS__MSG__DETAIL__COORDINATE__TRAITS_HPP_
