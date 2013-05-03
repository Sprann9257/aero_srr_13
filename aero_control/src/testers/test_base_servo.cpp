//============================================================================
// Name        : Jaco.cpp
// Author      : 
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

/**
 * @file jaco_arm_control.cpp
 *
 * @date   Feb 20, 2013
 * @author parallels
 * @brief \todo
 */

//License File
#include <aero_control/base_servo_control.h>

using namespace std;
ros::Publisher pub;
tf::TransformListener *listenerptr;

float x_pos = 3;
float y_pos = 0;
float z_pos = 0;
float rx_pos = 0;
float ry_pos = 0;
float rz_pos = 0;

geometry_msgs::PoseStamped test_msg_world;


void TimerCallback(const ros::TimerEvent&) {
	geometry_msgs::PoseStamped test_msg;

	test_msg_world.header.stamp = ros::Time::now();

	listenerptr->waitForTransform("/base_footprint", test_msg_world.header.frame_id, test_msg_world.header.stamp, ros::Duration(0.5));

	listenerptr->transformPose("/base_footprint", test_msg_world, test_msg);


	pub.publish(test_msg);

}


int main(int argc, char **argv) {

	/* Set up ROS */
	ros::init(argc, argv, "test_base_servo");
	ros::NodeHandle nh;
	ros::NodeHandle param_nh("~");

	std::string ObjectLocation("ObjectLocation"); ///String containing the topic name for cartesian commands
	pub = nh.advertise<geometry_msgs::PoseStamped>(ObjectLocation, 2);
	tf::TransformListener listener;
	listenerptr = &listener;

	ros::Timer timer = nh.createTimer(ros::Duration(0.1), TimerCallback);

	geometry_msgs::PoseStamped test_msg;


	test_msg.pose.position.x = x_pos;
	test_msg.pose.position.y = y_pos;
	test_msg.pose.position.z = z_pos;

	tf::Quaternion q;

	q.setRPY(rx_pos, ry_pos, rz_pos);

	tf::quaternionTFToMsg(q, test_msg.pose.orientation);
	test_msg.header.frame_id = "/base_footprint";
	test_msg.header.stamp = ros::Time::now();

	listenerptr->waitForTransform("/world", test_msg.header.frame_id, test_msg.header.stamp, ros::Duration(0.5));

	listenerptr->transformPose("/world", test_msg, test_msg_world);




	ros::spin();
}

