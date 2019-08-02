#include "ros/ros.h"
#include "ball_chaser/DriveToTarget.h"
#include <sensor_msgs/Image.h>

ros::ServiceClient client;

void drive_robot(float lin_x, float ang_z)
{
	ROS_INFO_STREAM("Driving Robot!!(^_^)");

	ball_chaser::DriveToTarget srv;
	srv.request.linear_x = lin_x;
	srv.request.angular_z = ang_z;

	if (!client.call(srv))
		ROS_ERROR("Failed to call service drive_robot");
}

void process_image_callback(const sensor_msgs::Image img)
{
	int white_pixel = 255;
	for (int i = 0; i < img.height * img.step; i++)
	{
		if(img.data[i] == white_pixel)
		{
			if(i / img.height < img.step / 3)
			{
				drive_robot(0.5, 0.5);
				break;
			}
			else if (i / img.height < img.step * 2 / 3)
			{
				drive_robot(0.5, 0);
				break;
			}
			else
			{
				drive_robot(0.5, -0.5);
				break;
			}
		}
		if(i == img.height * img.step - 1)
			drive_robot(0, 0);
	}
}

int main(int argc, char** argv)
{
	ros::init(argc, argv, "process_image");
	ros::NodeHandle n;

	client = n.serviceClient<ball_chaser::DriveToTarget>("/ball_chaser/command_robot");

	ros::Subscriber sub1 = n.subscribe("/camera/rgb/image_raw", 10, process_image_callback);

	ros::spin();

	return 0;
}
