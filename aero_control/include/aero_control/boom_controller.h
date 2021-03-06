/*
 * boom_controller.h
 *
 *  Created on: Feb 26, 2013
 *      Author: mdedonato
 */

#ifndef BOOM_CONTROLLER_H_
#define BOOM_CONTROLLER_H_

/* Define to debug without arm */
//#define DEBUG_WITHOUT_ARM
//#define PRINT_DEBUG_INFO
#include <ros/ros.h>
#include <std_msgs/String.h>
#include <time.h>
#include <aero_srr_msgs/AeroState.h>
#include <aero_srr_msgs/StateTransitionRequest.h>
#include <actionlib/client/simple_action_client.h>
#include <device_driver_base/SetJointPositionAction.h>

namespace aero_control
{

#define MAX_BOOM_PATH_STEPS 10

	class BoomController
	{
		public:
			BoomController(ros::NodeHandle nh, ros::NodeHandle param_nh);

		private:
			typedef actionlib::SimpleActionClient<device_driver_base::SetJointPositionAction> BoomClient;

			void GoToPosition(double angle, double velocity);
			void GoHome(void);
			void AeroStateMSG(const aero_srr_msgs::AeroStateConstPtr& aero_state);

			typedef struct
			{
					double angle;
					double velocity;
			} boom_path_step_t;

			boom_path_step_t boom_path[MAX_BOOM_PATH_STEPS];

			int boom_path_steps;

			inline bool SetBoomPathStep(double angle, double velocity)
			{

				if (this->boom_path_steps < MAX_BOOM_PATH_STEPS)
				{
					this->boom_path[boom_path_steps].angle = angle;
					this->boom_path[boom_path_steps].velocity = velocity;
					this->boom_path_steps++;
					return true;
				} else
				{
					ROS_WARN("Too many path points, you need to increase the maximum number of path points!");
					return false;

				}

			}

			/* Set path here */
			inline void PlanBoomPath(void)
			{
				boom_path_steps = 0;

				SetBoomPathStep(M_PI_2,0.1);
				SetBoomPathStep(-M_PI_2,0.1);

			}
			uint8_t boom_path_step_num;

			ros::Subscriber aero_state_sub;
			ros::ServiceClient aero_state_transition_srv_client;
			boost::shared_ptr<BoomClient> boom_control_client;
			bool active_state;
			bool pause_state;

	};

}

#endif /* BOOM_CONTROLLER_H_ */
