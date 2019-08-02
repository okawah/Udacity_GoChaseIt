#include "ros/ros.h"
#include "geometry_msgs/Twist.h"
#include "ball_chaser/DriveToTarget.h"

ros::Publisher motor_command_publisher;

//handle_drive_request callback function
bool handle_drive_request(ball_chaser::DriveToTarget::Request& req, ball_chaser::DriveToTarget::Response& res)
{
	ROS_INFO("DriveToTargetRequest received - linear_x:%1.2f, angular_z:%1.2f", (float)req.linear_x, (float)req.angular_z);
	
	//publish to robot wheel joints
	geometry_msgs::Twist motor_command;
	motor_command.linear.x = req.linear_x;
	motor_command.angular.z = req.angular_z;
	
	motor_command_publisher.publish(motor_command);

	//feedback
	res.msg_feedback = "Wheel velocities set - l_x: " + std::to_string(req.linear_x) + ", a_z: " + std::to_string(req.angular_z);
	ROS_INFO_STREAM(res.msg_feedback);

	return true;
}

int main(int argc, char** argv)
{
	ros::init(argc, argv, "drive_bot");

	ros::NodeHandle n;

	motor_command_publisher = n.advertise<geometry_msgs::Twist>("/cmd_vel", 10);

	ros::ServiceServer service = n.advertiseService("/ball_chaser/command_robot", handle_drive_request);

	ROS_INFO("Ready to send velocities!");

	ros::spin();

	return 0;
}


