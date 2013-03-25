/**
 * @file   Supervisor.h
 *
 * @date   Mar 22, 2013
 * @author parallels
 * @brief  \TODO
 */

#ifndef SUPERVISOR_H_
#define SUPERVISOR_H_

//*********** SYSTEM DEPENDANCIES ****************//
#include <ros/ros.h>
#include <tf/transform_listener.h>
//************ LOCAL DEPENDANCIES ****************//
#include <aero_srr_supervisor/SupervisorUtilities.h>
#include <aero_srr_supervisor/SetControlMode.h>
//***********    NAMESPACES     ****************//
namespace aero_srr_supervisor
{

class Supervisor
{
public:
	Supervisor(ros::NodeHandle& nh, ros::NodeHandle& p_nh);

private:

	/**
	 * @author Adam Panzica
	 * @brief  Enum over the states that Supervisor can be in
	 */
	enum State
	{
		ERROR,    //!< ERROR The robot has encountered an error
		STARTUP,  //!< STARTUP The robot is currently in its startup sequence
		MANUAL,   //!< MANUAL The robot is currently in manual control mode
		SEARCHING,//!< SEARCHING The robot is currently exploring the map
		NAVOBJ,   //!< NAVOBJ The robot is currently navigating to an object
		COLLECT,  //!< COLLECT The robot is currently collecting an object of interest
		HOME,     //!< HOME The robot is currently returning to the home platform
		PAUSE,    //!< PAUSE The robot is currently paused
		SHUTDOWN, //!< SHUTDOWN The robot is currently in its shutdown sequence
		SAFESTOP  //!< SAFESTOP The robot is currently in a safe-stop configuration
	};

	/**
	 * @author Adam Panzica
	 * @brief  Initialization function, Loads parameters from the ROS param server
	 */
	void loadParams();
	/**
	 * @author Adam Panzica
	 * @brief  Initialization function, Registers topics with the ROS system
	 */
	void registerTopics();
	/**
	 * @author Adam Panzica
	 * @brief  Initialization function, Registers timers with the ROS system
	 */
	void registerTimers();

	/**
	 * @author Adam Panzica
	 * @brief  Callback for processing SetControlMode messages
	 * @param message
	 */
	void setCtrlMdCB(const aero_srr_supervisor::SetControlModeConstPtr& message);

	/**
	 * @author Adam Panzica
	 * @brief  Callback for handling state updates
	 * @param event
	 */
	void stateUptdCb(const ros::TimerEvent& event);

	State state_;

	std::string ctrlmd_topic_;
	std::string aero_state_topic_;

	ros::NodeHandle nh_;
	ros::NodeHandle p_nh_;

	ros::Subscriber ctrlmd_sub_;
	ros::Publisher  aero_state_pub_;

	ros::Timer      state_update_timer_;

	tf::TransformListener transformer_;
};

};

#endif /* SUPERVISOR_H_ */
