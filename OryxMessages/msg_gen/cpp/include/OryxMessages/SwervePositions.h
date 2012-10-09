/* Auto-generated by genmsg_cpp for file /home/parallels/fuerte_workspace/oryxssr/OryxMessages/msg/SwervePositions.msg */
#ifndef ORYXMESSAGES_MESSAGE_SWERVEPOSITIONS_H
#define ORYXMESSAGES_MESSAGE_SWERVEPOSITIONS_H
#include <string>
#include <vector>
#include <map>
#include <ostream>
#include "ros/serialization.h"
#include "ros/builtin_message_traits.h"
#include "ros/message_operations.h"
#include "ros/time.h"

#include "ros/macros.h"

#include "ros/assert.h"


namespace OryxMessages
{
template <class ContainerAllocator>
struct SwervePositions_ {
  typedef SwervePositions_<ContainerAllocator> Type;

  SwervePositions_()
  : left_front_wheel(0.0)
  , right_front_wheel(0.0)
  , left_rear_wheel(0.0)
  , right_rear_wheel(0.0)
  {
  }

  SwervePositions_(const ContainerAllocator& _alloc)
  : left_front_wheel(0.0)
  , right_front_wheel(0.0)
  , left_rear_wheel(0.0)
  , right_rear_wheel(0.0)
  {
  }

  typedef double _left_front_wheel_type;
  double left_front_wheel;

  typedef double _right_front_wheel_type;
  double right_front_wheel;

  typedef double _left_rear_wheel_type;
  double left_rear_wheel;

  typedef double _right_rear_wheel_type;
  double right_rear_wheel;


  typedef boost::shared_ptr< ::OryxMessages::SwervePositions_<ContainerAllocator> > Ptr;
  typedef boost::shared_ptr< ::OryxMessages::SwervePositions_<ContainerAllocator>  const> ConstPtr;
  boost::shared_ptr<std::map<std::string, std::string> > __connection_header;
}; // struct SwervePositions
typedef  ::OryxMessages::SwervePositions_<std::allocator<void> > SwervePositions;

typedef boost::shared_ptr< ::OryxMessages::SwervePositions> SwervePositionsPtr;
typedef boost::shared_ptr< ::OryxMessages::SwervePositions const> SwervePositionsConstPtr;


template<typename ContainerAllocator>
std::ostream& operator<<(std::ostream& s, const  ::OryxMessages::SwervePositions_<ContainerAllocator> & v)
{
  ros::message_operations::Printer< ::OryxMessages::SwervePositions_<ContainerAllocator> >::stream(s, "", v);
  return s;}

} // namespace OryxMessages

namespace ros
{
namespace message_traits
{
template<class ContainerAllocator> struct IsMessage< ::OryxMessages::SwervePositions_<ContainerAllocator> > : public TrueType {};
template<class ContainerAllocator> struct IsMessage< ::OryxMessages::SwervePositions_<ContainerAllocator>  const> : public TrueType {};
template<class ContainerAllocator>
struct MD5Sum< ::OryxMessages::SwervePositions_<ContainerAllocator> > {
  static const char* value() 
  {
    return "ce91d1774d95f0a86e888d5fe76a8af6";
  }

  static const char* value(const  ::OryxMessages::SwervePositions_<ContainerAllocator> &) { return value(); } 
  static const uint64_t static_value1 = 0xce91d1774d95f0a8ULL;
  static const uint64_t static_value2 = 0x6e888d5fe76a8af6ULL;
};

template<class ContainerAllocator>
struct DataType< ::OryxMessages::SwervePositions_<ContainerAllocator> > {
  static const char* value() 
  {
    return "OryxMessages/SwervePositions";
  }

  static const char* value(const  ::OryxMessages::SwervePositions_<ContainerAllocator> &) { return value(); } 
};

template<class ContainerAllocator>
struct Definition< ::OryxMessages::SwervePositions_<ContainerAllocator> > {
  static const char* value() 
  {
    return "#Swerve positions, in radians\n\
float64 left_front_wheel\n\
float64 right_front_wheel\n\
float64 left_rear_wheel\n\
float64 right_rear_wheel\n\
";
  }

  static const char* value(const  ::OryxMessages::SwervePositions_<ContainerAllocator> &) { return value(); } 
};

template<class ContainerAllocator> struct IsFixedSize< ::OryxMessages::SwervePositions_<ContainerAllocator> > : public TrueType {};
} // namespace message_traits
} // namespace ros

namespace ros
{
namespace serialization
{

template<class ContainerAllocator> struct Serializer< ::OryxMessages::SwervePositions_<ContainerAllocator> >
{
  template<typename Stream, typename T> inline static void allInOne(Stream& stream, T m)
  {
    stream.next(m.left_front_wheel);
    stream.next(m.right_front_wheel);
    stream.next(m.left_rear_wheel);
    stream.next(m.right_rear_wheel);
  }

  ROS_DECLARE_ALLINONE_SERIALIZER;
}; // struct SwervePositions_
} // namespace serialization
} // namespace ros

namespace ros
{
namespace message_operations
{

template<class ContainerAllocator>
struct Printer< ::OryxMessages::SwervePositions_<ContainerAllocator> >
{
  template<typename Stream> static void stream(Stream& s, const std::string& indent, const  ::OryxMessages::SwervePositions_<ContainerAllocator> & v) 
  {
    s << indent << "left_front_wheel: ";
    Printer<double>::stream(s, indent + "  ", v.left_front_wheel);
    s << indent << "right_front_wheel: ";
    Printer<double>::stream(s, indent + "  ", v.right_front_wheel);
    s << indent << "left_rear_wheel: ";
    Printer<double>::stream(s, indent + "  ", v.left_rear_wheel);
    s << indent << "right_rear_wheel: ";
    Printer<double>::stream(s, indent + "  ", v.right_rear_wheel);
  }
};


} // namespace message_operations
} // namespace ros

#endif // ORYXMESSAGES_MESSAGE_SWERVEPOSITIONS_H
