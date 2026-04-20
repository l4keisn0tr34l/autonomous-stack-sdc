# generated from rosidl_generator_py/resource/_idl.py.em
# with input from sdc_msgs:msg/Coordinate.idl
# generated code does not contain a copyright notice


# Import statements for member types

# Member 'left'
# Member 'right'
# Member 'front'
# Member 'back'
# Member 'top'
# Member 'bottom'
import array  # noqa: E402, I100

import builtins  # noqa: E402, I100

import math  # noqa: E402, I100

import rosidl_parser.definition  # noqa: E402, I100


class Metaclass_Coordinate(type):
    """Metaclass of message 'Coordinate'."""

    _CREATE_ROS_MESSAGE = None
    _CONVERT_FROM_PY = None
    _CONVERT_TO_PY = None
    _DESTROY_ROS_MESSAGE = None
    _TYPE_SUPPORT = None

    __constants = {
    }

    @classmethod
    def __import_type_support__(cls):
        try:
            from rosidl_generator_py import import_type_support
            module = import_type_support('sdc_msgs')
        except ImportError:
            import logging
            import traceback
            logger = logging.getLogger(
                'sdc_msgs.msg.Coordinate')
            logger.debug(
                'Failed to import needed modules for type support:\n' +
                traceback.format_exc())
        else:
            cls._CREATE_ROS_MESSAGE = module.create_ros_message_msg__msg__coordinate
            cls._CONVERT_FROM_PY = module.convert_from_py_msg__msg__coordinate
            cls._CONVERT_TO_PY = module.convert_to_py_msg__msg__coordinate
            cls._TYPE_SUPPORT = module.type_support_msg__msg__coordinate
            cls._DESTROY_ROS_MESSAGE = module.destroy_ros_message_msg__msg__coordinate

    @classmethod
    def __prepare__(cls, name, bases, **kwargs):
        # list constant names here so that they appear in the help text of
        # the message class under "Data and other attributes defined here:"
        # as well as populate each message instance
        return {
        }


class Coordinate(metaclass=Metaclass_Coordinate):
    """Message class 'Coordinate'."""

    __slots__ = [
        '_size',
        '_reconsize',
        '_x',
        '_y',
        '_z',
        '_colour',
        '_left',
        '_right',
        '_front',
        '_back',
        '_top',
        '_bottom',
    ]

    _fields_and_field_types = {
        'size': 'uint8',
        'reconsize': 'uint8',
        'x': 'float',
        'y': 'float',
        'z': 'float',
        'colour': 'float',
        'left': 'sequence<float>',
        'right': 'sequence<float>',
        'front': 'sequence<float>',
        'back': 'sequence<float>',
        'top': 'sequence<float>',
        'bottom': 'sequence<float>',
    }

    SLOT_TYPES = (
        rosidl_parser.definition.BasicType('uint8'),  # noqa: E501
        rosidl_parser.definition.BasicType('uint8'),  # noqa: E501
        rosidl_parser.definition.BasicType('float'),  # noqa: E501
        rosidl_parser.definition.BasicType('float'),  # noqa: E501
        rosidl_parser.definition.BasicType('float'),  # noqa: E501
        rosidl_parser.definition.BasicType('float'),  # noqa: E501
        rosidl_parser.definition.UnboundedSequence(rosidl_parser.definition.BasicType('float')),  # noqa: E501
        rosidl_parser.definition.UnboundedSequence(rosidl_parser.definition.BasicType('float')),  # noqa: E501
        rosidl_parser.definition.UnboundedSequence(rosidl_parser.definition.BasicType('float')),  # noqa: E501
        rosidl_parser.definition.UnboundedSequence(rosidl_parser.definition.BasicType('float')),  # noqa: E501
        rosidl_parser.definition.UnboundedSequence(rosidl_parser.definition.BasicType('float')),  # noqa: E501
        rosidl_parser.definition.UnboundedSequence(rosidl_parser.definition.BasicType('float')),  # noqa: E501
    )

    def __init__(self, **kwargs):
        assert all('_' + key in self.__slots__ for key in kwargs.keys()), \
            'Invalid arguments passed to constructor: %s' % \
            ', '.join(sorted(k for k in kwargs.keys() if '_' + k not in self.__slots__))
        self.size = kwargs.get('size', int())
        self.reconsize = kwargs.get('reconsize', int())
        self.x = kwargs.get('x', float())
        self.y = kwargs.get('y', float())
        self.z = kwargs.get('z', float())
        self.colour = kwargs.get('colour', float())
        self.left = array.array('f', kwargs.get('left', []))
        self.right = array.array('f', kwargs.get('right', []))
        self.front = array.array('f', kwargs.get('front', []))
        self.back = array.array('f', kwargs.get('back', []))
        self.top = array.array('f', kwargs.get('top', []))
        self.bottom = array.array('f', kwargs.get('bottom', []))

    def __repr__(self):
        typename = self.__class__.__module__.split('.')
        typename.pop()
        typename.append(self.__class__.__name__)
        args = []
        for s, t in zip(self.__slots__, self.SLOT_TYPES):
            field = getattr(self, s)
            fieldstr = repr(field)
            # We use Python array type for fields that can be directly stored
            # in them, and "normal" sequences for everything else.  If it is
            # a type that we store in an array, strip off the 'array' portion.
            if (
                isinstance(t, rosidl_parser.definition.AbstractSequence) and
                isinstance(t.value_type, rosidl_parser.definition.BasicType) and
                t.value_type.typename in ['float', 'double', 'int8', 'uint8', 'int16', 'uint16', 'int32', 'uint32', 'int64', 'uint64']
            ):
                if len(field) == 0:
                    fieldstr = '[]'
                else:
                    assert fieldstr.startswith('array(')
                    prefix = "array('X', "
                    suffix = ')'
                    fieldstr = fieldstr[len(prefix):-len(suffix)]
            args.append(s[1:] + '=' + fieldstr)
        return '%s(%s)' % ('.'.join(typename), ', '.join(args))

    def __eq__(self, other):
        if not isinstance(other, self.__class__):
            return False
        if self.size != other.size:
            return False
        if self.reconsize != other.reconsize:
            return False
        if self.x != other.x:
            return False
        if self.y != other.y:
            return False
        if self.z != other.z:
            return False
        if self.colour != other.colour:
            return False
        if self.left != other.left:
            return False
        if self.right != other.right:
            return False
        if self.front != other.front:
            return False
        if self.back != other.back:
            return False
        if self.top != other.top:
            return False
        if self.bottom != other.bottom:
            return False
        return True

    @classmethod
    def get_fields_and_field_types(cls):
        from copy import copy
        return copy(cls._fields_and_field_types)

    @builtins.property
    def size(self):
        """Message field 'size'."""
        return self._size

    @size.setter
    def size(self, value):
        if __debug__:
            assert \
                isinstance(value, int), \
                "The 'size' field must be of type 'int'"
            assert value >= 0 and value < 256, \
                "The 'size' field must be an unsigned integer in [0, 255]"
        self._size = value

    @builtins.property
    def reconsize(self):
        """Message field 'reconsize'."""
        return self._reconsize

    @reconsize.setter
    def reconsize(self, value):
        if __debug__:
            assert \
                isinstance(value, int), \
                "The 'reconsize' field must be of type 'int'"
            assert value >= 0 and value < 256, \
                "The 'reconsize' field must be an unsigned integer in [0, 255]"
        self._reconsize = value

    @builtins.property
    def x(self):
        """Message field 'x'."""
        return self._x

    @x.setter
    def x(self, value):
        if __debug__:
            assert \
                isinstance(value, float), \
                "The 'x' field must be of type 'float'"
            assert not (value < -3.402823466e+38 or value > 3.402823466e+38) or math.isinf(value), \
                "The 'x' field must be a float in [-3.402823466e+38, 3.402823466e+38]"
        self._x = value

    @builtins.property
    def y(self):
        """Message field 'y'."""
        return self._y

    @y.setter
    def y(self, value):
        if __debug__:
            assert \
                isinstance(value, float), \
                "The 'y' field must be of type 'float'"
            assert not (value < -3.402823466e+38 or value > 3.402823466e+38) or math.isinf(value), \
                "The 'y' field must be a float in [-3.402823466e+38, 3.402823466e+38]"
        self._y = value

    @builtins.property
    def z(self):
        """Message field 'z'."""
        return self._z

    @z.setter
    def z(self, value):
        if __debug__:
            assert \
                isinstance(value, float), \
                "The 'z' field must be of type 'float'"
            assert not (value < -3.402823466e+38 or value > 3.402823466e+38) or math.isinf(value), \
                "The 'z' field must be a float in [-3.402823466e+38, 3.402823466e+38]"
        self._z = value

    @builtins.property
    def colour(self):
        """Message field 'colour'."""
        return self._colour

    @colour.setter
    def colour(self, value):
        if __debug__:
            assert \
                isinstance(value, float), \
                "The 'colour' field must be of type 'float'"
            assert not (value < -3.402823466e+38 or value > 3.402823466e+38) or math.isinf(value), \
                "The 'colour' field must be a float in [-3.402823466e+38, 3.402823466e+38]"
        self._colour = value

    @builtins.property
    def left(self):
        """Message field 'left'."""
        return self._left

    @left.setter
    def left(self, value):
        if isinstance(value, array.array):
            assert value.typecode == 'f', \
                "The 'left' array.array() must have the type code of 'f'"
            self._left = value
            return
        if __debug__:
            from collections.abc import Sequence
            from collections.abc import Set
            from collections import UserList
            from collections import UserString
            assert \
                ((isinstance(value, Sequence) or
                  isinstance(value, Set) or
                  isinstance(value, UserList)) and
                 not isinstance(value, str) and
                 not isinstance(value, UserString) and
                 all(isinstance(v, float) for v in value) and
                 all(not (val < -3.402823466e+38 or val > 3.402823466e+38) or math.isinf(val) for val in value)), \
                "The 'left' field must be a set or sequence and each value of type 'float' and each float in [-340282346600000016151267322115014000640.000000, 340282346600000016151267322115014000640.000000]"
        self._left = array.array('f', value)

    @builtins.property
    def right(self):
        """Message field 'right'."""
        return self._right

    @right.setter
    def right(self, value):
        if isinstance(value, array.array):
            assert value.typecode == 'f', \
                "The 'right' array.array() must have the type code of 'f'"
            self._right = value
            return
        if __debug__:
            from collections.abc import Sequence
            from collections.abc import Set
            from collections import UserList
            from collections import UserString
            assert \
                ((isinstance(value, Sequence) or
                  isinstance(value, Set) or
                  isinstance(value, UserList)) and
                 not isinstance(value, str) and
                 not isinstance(value, UserString) and
                 all(isinstance(v, float) for v in value) and
                 all(not (val < -3.402823466e+38 or val > 3.402823466e+38) or math.isinf(val) for val in value)), \
                "The 'right' field must be a set or sequence and each value of type 'float' and each float in [-340282346600000016151267322115014000640.000000, 340282346600000016151267322115014000640.000000]"
        self._right = array.array('f', value)

    @builtins.property
    def front(self):
        """Message field 'front'."""
        return self._front

    @front.setter
    def front(self, value):
        if isinstance(value, array.array):
            assert value.typecode == 'f', \
                "The 'front' array.array() must have the type code of 'f'"
            self._front = value
            return
        if __debug__:
            from collections.abc import Sequence
            from collections.abc import Set
            from collections import UserList
            from collections import UserString
            assert \
                ((isinstance(value, Sequence) or
                  isinstance(value, Set) or
                  isinstance(value, UserList)) and
                 not isinstance(value, str) and
                 not isinstance(value, UserString) and
                 all(isinstance(v, float) for v in value) and
                 all(not (val < -3.402823466e+38 or val > 3.402823466e+38) or math.isinf(val) for val in value)), \
                "The 'front' field must be a set or sequence and each value of type 'float' and each float in [-340282346600000016151267322115014000640.000000, 340282346600000016151267322115014000640.000000]"
        self._front = array.array('f', value)

    @builtins.property
    def back(self):
        """Message field 'back'."""
        return self._back

    @back.setter
    def back(self, value):
        if isinstance(value, array.array):
            assert value.typecode == 'f', \
                "The 'back' array.array() must have the type code of 'f'"
            self._back = value
            return
        if __debug__:
            from collections.abc import Sequence
            from collections.abc import Set
            from collections import UserList
            from collections import UserString
            assert \
                ((isinstance(value, Sequence) or
                  isinstance(value, Set) or
                  isinstance(value, UserList)) and
                 not isinstance(value, str) and
                 not isinstance(value, UserString) and
                 all(isinstance(v, float) for v in value) and
                 all(not (val < -3.402823466e+38 or val > 3.402823466e+38) or math.isinf(val) for val in value)), \
                "The 'back' field must be a set or sequence and each value of type 'float' and each float in [-340282346600000016151267322115014000640.000000, 340282346600000016151267322115014000640.000000]"
        self._back = array.array('f', value)

    @builtins.property
    def top(self):
        """Message field 'top'."""
        return self._top

    @top.setter
    def top(self, value):
        if isinstance(value, array.array):
            assert value.typecode == 'f', \
                "The 'top' array.array() must have the type code of 'f'"
            self._top = value
            return
        if __debug__:
            from collections.abc import Sequence
            from collections.abc import Set
            from collections import UserList
            from collections import UserString
            assert \
                ((isinstance(value, Sequence) or
                  isinstance(value, Set) or
                  isinstance(value, UserList)) and
                 not isinstance(value, str) and
                 not isinstance(value, UserString) and
                 all(isinstance(v, float) for v in value) and
                 all(not (val < -3.402823466e+38 or val > 3.402823466e+38) or math.isinf(val) for val in value)), \
                "The 'top' field must be a set or sequence and each value of type 'float' and each float in [-340282346600000016151267322115014000640.000000, 340282346600000016151267322115014000640.000000]"
        self._top = array.array('f', value)

    @builtins.property
    def bottom(self):
        """Message field 'bottom'."""
        return self._bottom

    @bottom.setter
    def bottom(self, value):
        if isinstance(value, array.array):
            assert value.typecode == 'f', \
                "The 'bottom' array.array() must have the type code of 'f'"
            self._bottom = value
            return
        if __debug__:
            from collections.abc import Sequence
            from collections.abc import Set
            from collections import UserList
            from collections import UserString
            assert \
                ((isinstance(value, Sequence) or
                  isinstance(value, Set) or
                  isinstance(value, UserList)) and
                 not isinstance(value, str) and
                 not isinstance(value, UserString) and
                 all(isinstance(v, float) for v in value) and
                 all(not (val < -3.402823466e+38 or val > 3.402823466e+38) or math.isinf(val) for val in value)), \
                "The 'bottom' field must be a set or sequence and each value of type 'float' and each float in [-340282346600000016151267322115014000640.000000, 340282346600000016151267322115014000640.000000]"
        self._bottom = array.array('f', value)
