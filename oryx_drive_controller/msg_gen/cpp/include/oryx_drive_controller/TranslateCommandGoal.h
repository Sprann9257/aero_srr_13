/* Auto-generated by genmsg_cpp for file /home/parallels/fuerte_workspace/oryxssr/oryx_drive_controller/msg/TranslateCommandGoal.msg */
#ifndef ORYX_DRIVE_CONTROLLER_MESSAGE_TRANSLATECOMMANDGOAL_H
#define ORYX_DRIVE_CONTROLLER_MESSAGE_TRANSLATECOMMANDGOAL_H
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


namespace oryx_drive_controller
{
template <class ContainerAllocator>
struct TranslateCommandGoal_ {
  typedef TranslateCommandGoal_<ContainerAllocator> Type;

  TranslateCommandGoal_()
  : x_velocity(0.0)
  , y_velocity(0.0)
  {
  }

  TranslateCommandGoal_(const ContainerAllocator& _alloc)
  : x_velocity(0.0)
  , y_velocity(0.0)
  {
  }

  typedef double _x_velocity_type;
  double x_velocity;

  typedef double _y_velocity_type;
  double y_velocity;


  typedef boost::shared_ptr< ::oryx_drive_controller::TranslateCommandGoal_<ContainerAllocator> > Ptr;
  typedef boost::shared_ptr< ::oryx_drive_controller::TranslateCommandGoal_<ContainerAllocator>  const> ConstPtr;
  boost::shared_ptr<std::map<std::string, std::string> > __connection_header;
}; // struct TranslateCommandGoal
typedef  ::oryx_drive_controller::TranslateCommandGoal_<std::allocator<void> > TranslateCommandGoal;

typedef boost::shared_ptr< ::oryx_drive_controller::TranslateCommandGoal> TranslateCommandGoalPtr;
typedef boost::shared_ptr< ::oryx_drive_controller::TranslateCommandGoal const> TranslateCommandGoalConstPtr;


template<typename ContainerAllocator>
std::ostream& operator<<(std::ostream& s, const  ::oryx_drive_controller::TranslateCommandGoal_<ContainerAllocator> & v)
{
  ros::message_operations::Printer< ::oryx_drive_controller::TranslateCommandGoal_<ContainerAllocator> >::stream(s, "", v);
  return s;}

} // namespace oryx_drive_controller

namespace ros
{
namespace message_traits
{
template<class ContainerAllocator> struct IsMessage< ::oryx_drive_controller::TranslateCommandGoal_<ContainerAllocator> > : public TrueType {};
template<class ContainerAllocator> struct IsMessage< ::oryx_drive_controller::TranslateCommandGoal_<ContainerAllocator>  const> : public TrueType {};
template<class ContainerAllocator>
struct MD5Sum< ::oryx_drive_controller::TranslateCommandGoal_<ContainerAllocator> > {
  static const char* value() 
  {
    return "7ee4a9a1d2944426c31584cd7f0b5821";
  }

  static const char* value(const  ::oryx_drive_controller::TranslateCommandGoal_<ContainerAllocator> &) { return value(); } 
  static const uint64_t static_value1 = 0x7ee4a9a1d2944426ULL;
  static const uint64_t static_value2 = 0xc31584cd7f0b5821ULL;
};

template<class ContainerAllocator>
struct DataType< ::oryx_drive_controller::TranslateCommandGoal_<ContainerAllocator> > {
  static const char* value() 
  {
    return "oryx_drive_controller/TranslateCommandGoal";
  }

  static const char* value(const  ::oryx_drive_controller::TranslateCommandGoal_<ContainerAllocator> &) { return value(); } 
};

template<class ContainerAllocator>
struct Definition< ::oryx_drive_controller::TranslateCommandGoal_<ContainerAllocator> > {
  static const char* value() 
  {
    return "# ====== DO NOT MODIFY! AUTOGENERATED FROM AN ACTION DEFINITION ======\n\
#goal definition\n\
float64 x_velocity\n\
float64 y_velocity\n\
#float64	omega\n\
\n\
";
  }

  static const char* value(const  ::oryx_drive_controller::TranslateCommandGoal_<ContainerAllocator> &) { return value(); } 
};

template<class ContainerAllocator> struct IsFixedSize< ::oryx_drive_controller::TranslateCommandGoal_<ContainerAllocator> > : public TrueType {};
} // namespace message_traits
} // namespace ros

namespace ros
{
namespace serialization
{

template<class ContainerAllocator> struct Serializer< ::oryx_drive_controller::TranslateCommandGoal_<ContainerAllocator> >
{
  template<typename Stream, typename T> inline static void allInOne(Stream& stream, T m)
  {
    stream.next(m.x_velocity);
    stream.next(m.y_velocity);
  }

  ROS_DECLARE_ALLINONE_SERIALIZER;
}; // struct TranslateCommandGoal_
} // namespace serialization
} // namespace ros

namespace ros
{
namespace message_operations
{

template<class ContainerAllocator>
struct Printer< ::oryx_drive_controller::TranslateCommandGoal_<ContainerAllocator> >
{
  template<typename Stream> static void stream(Stream& s, const std::string& indent, const  ::oryx_drive_controller::TranslateCommandGoal_<ContainerAllocator> & v) 
  {
    s << indent << "x_velocity: ";
    Printer<double>::stream(s, indent + "  ", v.x_velocity);
    s << indent << "y_velocity: ";
    Printer<double>::stream(s, indent + "  ", v.y_velocity);
  }
};


} // namespace message_operations
} // namespace ros

#endif // ORYX_DRIVE_CONTROLLER_MESSAGE_TRANSLATECOMMANDGOAL_H

