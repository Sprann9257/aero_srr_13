/* Auto-generated by genmsg_cpp for file /home/parallels/fuerte_workspace/oryxssr/action_test/msg/TestFeedback.msg */
#ifndef ACTION_TEST_MESSAGE_TESTFEEDBACK_H
#define ACTION_TEST_MESSAGE_TESTFEEDBACK_H
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


namespace action_test
{
template <class ContainerAllocator>
struct TestFeedback_ {
  typedef TestFeedback_<ContainerAllocator> Type;

  TestFeedback_()
  : progress(0)
  {
  }

  TestFeedback_(const ContainerAllocator& _alloc)
  : progress(0)
  {
  }

  typedef int32_t _progress_type;
  int32_t progress;


  typedef boost::shared_ptr< ::action_test::TestFeedback_<ContainerAllocator> > Ptr;
  typedef boost::shared_ptr< ::action_test::TestFeedback_<ContainerAllocator>  const> ConstPtr;
  boost::shared_ptr<std::map<std::string, std::string> > __connection_header;
}; // struct TestFeedback
typedef  ::action_test::TestFeedback_<std::allocator<void> > TestFeedback;

typedef boost::shared_ptr< ::action_test::TestFeedback> TestFeedbackPtr;
typedef boost::shared_ptr< ::action_test::TestFeedback const> TestFeedbackConstPtr;


template<typename ContainerAllocator>
std::ostream& operator<<(std::ostream& s, const  ::action_test::TestFeedback_<ContainerAllocator> & v)
{
  ros::message_operations::Printer< ::action_test::TestFeedback_<ContainerAllocator> >::stream(s, "", v);
  return s;}

} // namespace action_test

namespace ros
{
namespace message_traits
{
template<class ContainerAllocator> struct IsMessage< ::action_test::TestFeedback_<ContainerAllocator> > : public TrueType {};
template<class ContainerAllocator> struct IsMessage< ::action_test::TestFeedback_<ContainerAllocator>  const> : public TrueType {};
template<class ContainerAllocator>
struct MD5Sum< ::action_test::TestFeedback_<ContainerAllocator> > {
  static const char* value() 
  {
    return "3068c2ff8fa67c9565c90bbf9331baeb";
  }

  static const char* value(const  ::action_test::TestFeedback_<ContainerAllocator> &) { return value(); } 
  static const uint64_t static_value1 = 0x3068c2ff8fa67c95ULL;
  static const uint64_t static_value2 = 0x65c90bbf9331baebULL;
};

template<class ContainerAllocator>
struct DataType< ::action_test::TestFeedback_<ContainerAllocator> > {
  static const char* value() 
  {
    return "action_test/TestFeedback";
  }

  static const char* value(const  ::action_test::TestFeedback_<ContainerAllocator> &) { return value(); } 
};

template<class ContainerAllocator>
struct Definition< ::action_test::TestFeedback_<ContainerAllocator> > {
  static const char* value() 
  {
    return "# ====== DO NOT MODIFY! AUTOGENERATED FROM AN ACTION DEFINITION ======\n\
#feedback\n\
int32 progress\n\
\n\
";
  }

  static const char* value(const  ::action_test::TestFeedback_<ContainerAllocator> &) { return value(); } 
};

template<class ContainerAllocator> struct IsFixedSize< ::action_test::TestFeedback_<ContainerAllocator> > : public TrueType {};
} // namespace message_traits
} // namespace ros

namespace ros
{
namespace serialization
{

template<class ContainerAllocator> struct Serializer< ::action_test::TestFeedback_<ContainerAllocator> >
{
  template<typename Stream, typename T> inline static void allInOne(Stream& stream, T m)
  {
    stream.next(m.progress);
  }

  ROS_DECLARE_ALLINONE_SERIALIZER;
}; // struct TestFeedback_
} // namespace serialization
} // namespace ros

namespace ros
{
namespace message_operations
{

template<class ContainerAllocator>
struct Printer< ::action_test::TestFeedback_<ContainerAllocator> >
{
  template<typename Stream> static void stream(Stream& s, const std::string& indent, const  ::action_test::TestFeedback_<ContainerAllocator> & v) 
  {
    s << indent << "progress: ";
    Printer<int32_t>::stream(s, indent + "  ", v.progress);
  }
};


} // namespace message_operations
} // namespace ros

#endif // ACTION_TEST_MESSAGE_TESTFEEDBACK_H

