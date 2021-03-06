/**
 * @file   MissionPlanner.h
 *
 * @date   May 10, 2013
 * @author parallels
 * @brief  \TODO
 */

#ifndef MISSIONPLANNER_H_
#define MISSIONPLANNER_H_

//*********** SYSTEM DEPENDANCIES ****************//
#include <ros/ros.h>
#include <nav_msgs/Path.h>
#include <tf/tf.h>
#include <tf/transform_listener.h>
#include <geometry_msgs/PoseStamped.h>
#include <aero_srr_msgs/AeroState.h>
#include <deque>
#include <dynamic_reconfigure/server.h>
#include <geometry_msgs/PoseArray.h>
//************ LOCAL DEPENDANCIES ****************//
#include <aero_path_planning/MissionPlannerConfig.h>
#include <aero_path_planning/utilities/AeroPathPlanningUtilities.h>
//***********    NAMESPACES     ****************//

namespace aero_path_planning
{

/**
 * @author Adam Panzica
 * @brief Class for managing mission level tasks
 *
 * MissionPlanner is responsable for generating/monitoring mission level tasks. It generates mission goals for GlobalPlanner to use when re-planning. It consumes the CarrotPath produced by GlobalPlanenr and ensures that the goal point
 * used by LocalPlanner is updated correctly based on the current location of the robot. It also handles managing object of interest detections.
 */
class MissionPlanner
{
private:

	/**
	 * @author Adam Panzica
	 * @brief  Loads parameters from the ROS system
	 */
	void loadParam();

	/**
	 * @author Adam Panzica
	 * @brief  Registers topics with the ROS system
	 */
	void registerTopics();

	/**
	 * @author Adam Panzica
	 * @brief  Register timers with the ROS system
	 */
	void registerTimers();

	/**
	 * @author Adam Panzica
	 * @brief Callback for handling changes in robot state
	 * @param message
	 */
	void stateCB(const aero_srr_msgs::AeroStateConstPtr& message);

	/**
	 * @author Adam Panzica
	 * @brief Callback for hadleing new CarrotPaths
	 * @param message
	 */
	void pathCB(const nav_msgs::PathConstPtr& message);

	/**
	 * @author Adam Panzica
	 * @brief Updates the global goal
	 * @param event
	 */
	void goalCB(const ros::TimerEvent& event);

	/**
	 * @author Adam Panzica
	 * @brief callback for handling dynamic reconfigure callbacks
	 * @param config
	 * @param level
	 */
	void drCB(const MissionPlannerConfig& config, uint32_t level);

	/**
	 * @author Adam Panzica
	 * @brief Causes the robot to return home after a set timeout period
	 * @param event
	 */
	void timeoutCB(const ros::TimerEvent& event);

	/**
	 * @author Adam Panzica
	 * @brief  Publishes the next goal on the carrot path
	 */
	void updateGoal() const;

	/**
	 * @author Adam Panzica
	 * @brief looks up the robot's current location in the world frame
	 * @param [in] point Point to fill with the robot's location (in meters, not grid coordinates)
	 * @return true if the transform was sucessful, else false
	 */
	bool calcRobotPointWorld(geometry_msgs::PoseStamped& point) const;

	/**
	 * @author Adam Panzica
	 * @brief Calculates if the next goal point in carrot path has been reached
	 * @param [in] worldLocation The location of the robot in world coodinates
	 * @param [in] threshold The threshold to consider the goal reached (meters, not grid coordinates)
	 * @return True if reached, else false
	 */
	bool reachedNextGoal(const geometry_msgs::PoseStamped& worldLocation, const double threshold) const;

	/**
	 * @author Adam Panzica
	 * @brief Updates the current mission goal
	 */
	void updateMissionGoal();

	void ooiCB(const geometry_msgs::PoseArrayConstPtr& message);

	void generateDetectionGoalList();

	void requestCollect();

	void requestNavObj();

	void requestHome();

	void requestStateTransition(aero_srr_msgs::AeroState& requested_state);

	void pause(bool enable);

	std::string state_topic_;           ///Topic name for receiving robot state from the supervisor
	std::string path_topic_;            ///Topic name for receiving new carrot paths
	std::string ooi_topic_;             ///Topic name for receiving Object of Interest detections
	std::string path_goal_topic_;       ///Topic name to publish to to update the goal used by the local planner
	std::string mission_goal_topic_;    ///Topic name to publish noew mission goals to be used by the global planner
	std::string global_frame_;			///frame_id of the global scale frame
	std::string local_frame_;           ///frame_id of the local scale frame
	std::string state_request_topic_;   ///Topic name to request state transition requests

	std::deque<geometry_msgs::Pose>        mission_goals_; ///The chain of mission-goal points to fallow
	std::deque<geometry_msgs::PoseStamped> carrot_path_;   ///The current carrot path

	aero_path_planning::ObjectOfInterestManager OoI_manager_;

	double                path_threshold_;///The threshold for determining we've gotten to a point on the path in search mode, in meters
	double                nav_threshold_; ///The threshold for determining ew've gotten to an object of interest, in meters
	double                dist_threshold_;///The threshold for determining we've gotten to a mission goal
	bool                  searching_;     ///Flag to signal if the robot is searching or not
	bool                  naving_;        ///Flag to singal if the robot is naving to obj
	bool                  homeing_;        ///Flag to signal if the robot is homing
	bool                  recieved_path_; ///Flag to signal if the robot has ever recieved a carrot path
	bool                  last_collect_;  ///Flag to singla if we've attempted to collect the last OoI

	ros::NodeHandle       nh_;            ///Global NodeHandle into the ROS system
	ros::NodeHandle       p_nh_;          ///Private NodeHandle into the ROS system

	tf::TransformListener transformer_;   ///Hook into the tf system

	ros::Subscriber       state_sub_;       ///Subscriber for the supervisor state
	ros::Subscriber       path_sub_;        ///Subscriber to new carrot paths
	ros::Subscriber       ooi_sub_;         ///Subscriber to new Objects of Interest
	ros::Publisher        path_goal_pub_;   ///Publisher to update the current global goal used by LocalPlanner
	ros::Publisher        mission_goal_pub_;///Publisher to update the current mission goal used by GlobalPlanner
	ros::ServiceClient    state_request_client_;///Publisher to request state changes
	ros::Timer            goal_timer_;      ///Timer to update the goal point
	ros::Timer            mission_timeout_;///One shot timer to make the robot come home after the alloted time
	mutable ros::Time     time_out_start_;  ///Timeout to prevent getting stuck without ever reaching a hoal

	dynamic_reconfigure::Server<MissionPlannerConfig> dr_server_; ///Server for dynamic_reconfigure requests

public:
	/**
	 * @author Adam Panzica
	 * @param nh global node handle
	 * @param p_nh private node handle
	 */
	MissionPlanner(ros::NodeHandle& nh, ros::NodeHandle& p_nh);

};

};

#endif /* MISSIONPLANNER_H_ */
