/*
 * oryx_drive_controller_action_tests.cpp
 *
 *  Created on: Oct 10, 2012
 *      Author: parallels
 */

#include<ros/ros.h>
#include<actionlib/client/simple_action_client.h>
#include<aero_drive_controller/VelocityCommandAction.h>
#include<aero_drive_controller/TranslateCommandAction.h>

void print_vel_test_info(double rad, double vel){
	ROS_INFO("Testing Velocity Control: <R=%f, V=%f>", rad, vel);
}

void print_trans_test_info(double x, double y){
	ROS_INFO("Testing Translate Control: <X_V=%f, Y_V=%f>", x, y);
}


int main(int argc, char** argv){
	//Initialize the node
	ros::init(argc, argv, "oryx_drive_controller_test");
	ros::NodeHandle nh("~");
	std::string v_action_topic("velocity_command_topic");	///String containing the topic name for velocity commands
	std::string t_action_topic("translate_command_topic");	///String containing the topic name for translate commands

	nh.getParam(v_action_topic, v_action_topic);
	nh.getParam(t_action_topic, t_action_topic);
	//Set up clients
	actionlib::SimpleActionClient<aero_drive_controller::VelocityCommandAction> ac_v(v_action_topic, true);
	actionlib::SimpleActionClient<aero_drive_controller::TranslateCommandAction> ac_t(t_action_topic, true);
	//Wait for connection
	ROS_INFO("%s is waiting for servers...", ros::this_node::getName().c_str());
	ac_v.waitForServer();
	ROS_INFO("Got connection to VelocityCommand server!");
	ac_t.waitForServer();
	ROS_INFO("Got connection to TraverseCommand server!");

	//Set up test commands
	aero_drive_controller::VelocityCommandGoal v_goal;
	aero_drive_controller::TranslateCommandGoal t_goal;


	//Test velocity straight line:
	v_goal.radius	= std::numeric_limits<double>::infinity();
	v_goal.velocity	= 1;
	print_vel_test_info(v_goal.radius, v_goal.velocity);
	ROS_INFO(ac_v.sendGoalAndWait(v_goal,ros::Duration(5000),ros::Duration(5000)).toString().c_str());
	//Test velocity straight rotation
	v_goal.radius	= 0;
	v_goal.velocity	= 1;
	print_vel_test_info(v_goal.radius, v_goal.velocity);
	ROS_INFO(ac_v.sendGoalAndWait(v_goal,ros::Duration(5000),ros::Duration(5000)).toString().c_str());
	//Test velocity arbitrary arc
	v_goal.radius	= 2.5;
	v_goal.velocity	= 1;
	print_vel_test_info(v_goal.radius, v_goal.velocity);
	ROS_INFO(ac_v.sendGoalAndWait(v_goal,ros::Duration(5000),ros::Duration(5000)).toString().c_str());
	//Test translate straight x+:
	t_goal.x_velocity	= 1;
	t_goal.y_velocity	= 0;
	print_trans_test_info(t_goal.x_velocity, t_goal.y_velocity);
	ROS_INFO(ac_t.sendGoalAndWait(t_goal,ros::Duration(5000),ros::Duration(5000)).toString().c_str());
	//Test translate straight x+:
	t_goal.x_velocity	= 0;
	t_goal.y_velocity	= 1;
	print_trans_test_info(t_goal.x_velocity, t_goal.y_velocity);
	ROS_INFO(ac_t.sendGoalAndWait(t_goal,ros::Duration(5000),ros::Duration(5000)).toString().c_str());
	//Test translate arbitrary vector:
	t_goal.x_velocity	= 1;
	t_goal.y_velocity	= 1;
	print_trans_test_info(t_goal.x_velocity, t_goal.y_velocity);
	ROS_INFO(ac_t.sendGoalAndWait(t_goal,ros::Duration(5000),ros::Duration(5000)).toString().c_str());

}
