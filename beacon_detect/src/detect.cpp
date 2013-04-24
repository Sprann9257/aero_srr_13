/*
 * detect.cpp
 *
 *  Created on: Feb 14, 2013
 *      Author: bpwiselybabu
 */

#include <vision_comm/RosBridge.h>
#include <stdlib.h>
#include <AprilTags/TagDetector.h>
#include <AprilTags/Tag36h11.h>
#include <Eigen/Geometry>
#include <tf/transform_broadcaster.h>

using namespace cv;
using namespace std;

const char* window_name = "apriltags_demo";

// draw April tag detection on actual image
void draw_detection(cv::Mat& image, const AprilTags::TagDetection& detection) {
  // use corner points detected by line intersection
  std::pair<float, float> p1 = detection.p[0];
  std::pair<float, float> p2 = detection.p[1];
  std::pair<float, float> p3 = detection.p[2];
  std::pair<float, float> p4 = detection.p[3];

  // plot outline
  cv::line(image, cv::Point2f(p1.first, p1.second), cv::Point2f(p2.first, p2.second), cv::Scalar(255,0,0,0) );
  cv::line(image, cv::Point2f(p2.first, p2.second), cv::Point2f(p3.first, p3.second), cv::Scalar(0,255,0,0) );
  cv::line(image, cv::Point2f(p3.first, p3.second), cv::Point2f(p4.first, p4.second), cv::Scalar(0,0,255,0) );
  cv::line(image, cv::Point2f(p4.first, p4.second), cv::Point2f(p1.first, p1.second), cv::Scalar(255,0,255,0) );

  // mark center
  cv::circle(image, cv::Point2f(detection.cxy.first, detection.cxy.second), 8, cv::Scalar(0,0,255,0), 2);

  // print ID
  std::ostringstream strSt;
  strSt << "#" << detection.id;
  cv::putText(image, strSt.str(),
              cv::Point2f(detection.cxy.first + 10, detection.cxy.second + 10),
              cv::FONT_HERSHEY_PLAIN, 1, cv::Scalar(0,0,255));
}

int main(int argc, char **argv)
{
	ros::init(argc, argv, "roscamera");
	ros::NodeHandle nh;
	ros::NodeHandle pnh("~");

	Mat *frame,gray;
	double fps;

	string base_frame;
	string tag_frame;
	string topic;
	char frameid[40];
	if(!pnh.getParam("cam_topic", topic))
	{
		ROS_ERROR("cam_topic not set");
		return -1;
	}

	RosBridge roscamera(topic.c_str(),frame);
	AprilTags::TagDetector tag_detector(AprilTags::tagCodes36h11);
	static tf::TransformBroadcaster br;
	//initialize the camera
	double tag_size = 0.166; // real side length in meters of square black frame

	if(!pnh.getParam("tag_size", tag_size))
	{
		ROS_ERROR("tag_size not set, will use 0.166");
		tag_size = 0.166;
	}
	if(!pnh.getParam("tag_frame", tag_frame))
	{
		ROS_ERROR("tag_size not set, will use Tag_");
		tag_frame = "Tag_";
	}

	double fx = 620.036171; // camera focal length
	double fy = 620.688638;
	double px = 317.387991; // camera principal point
	double py = 242.043315;

	ros::Rate loop(10);
	while(ros::ok())
	{
		roscamera.startTime();
		frame=roscamera.getNextFrame();
		Mat K=roscamera.intrinsic();
		if(!K.empty())
		{

			fx=double(K.at<float>(0,0));
			fy=double(K.at<float>(1,1));
			px=double(K.at<float>(0,2));
			py=double(K.at<float>(1,2));
		}
		base_frame=roscamera.frame_id();
		if(frame!=NULL)
		{
			cv::cvtColor(*frame, gray, CV_BGR2GRAY);
			std::vector<AprilTags::TagDetection> detections = tag_detector.extractTags(gray);
			cout << detections.size() << " tags detected:" << endl;
			for (int i=0; i<detections.size(); i++)
			{
			      cout << "  Id: " << detections[i].id << " -- "
			           << "  Hamming distance: " << detections[i].hammingDistance << endl;

			      // also highlight in the image
			      draw_detection(*frame, detections[i]);

			      // recovering the relative pose requires camera calibration;

			      Eigen::Matrix4d T = detections[i].getRelativeTransform(tag_size, fx, fy, px, py);

			      tf::Transform transform;
			      transform.setOrigin( tf::Vector3(T(0,3),T(1,3), T(3,3)) );
			      Eigen::Matrix3d rot = T.block(0,0,3,3);
			      Eigen::Quaternion<double> final = Eigen::Quaternion<double>(rot);
			      transform.setRotation( tf::Quaternion(final.x(), final.y(), final.z(),final.w()) );
			      cout<<rot<<endl;
			      sprintf(frameid,"%s%d",tag_frame.c_str(),detections[i].id );
			      br.sendTransform(tf::StampedTransform(transform, ros::Time::now(), base_frame.c_str(), frameid));

			}
			roscamera.showImage(window_name);
			fps=roscamera.endTime();
			waitKey(30);
		}
		ros::spinOnce();
		loop.sleep();
	}
	return 0;
}