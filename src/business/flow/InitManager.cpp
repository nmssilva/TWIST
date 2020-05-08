/**
 * \file
 * \brief       Application node
 *
 * \project     TWIST
 * \copyright   FIKALAB
 */

#include "business/flow/InitManager.h"

#include <ros/ros.h>
#include <turtlesim/Color.h>
#include <turtlesim/Pose.h>
#include <geometry_msgs/Twist.h>

/**
 * @ingroup twist
 *
 * Namespace for all TWIST modules
 */
namespace twist {

bool hit{false};

void pose_callback(const turtlesim::Pose& pose) {
    ROS_INFO("Position update. x=[%f], y=[%f]", pose.x, pose.y);
    if(pose.x <= 0 || pose.y <= 0 || pose.x >= 11 || pose.y >= 11) {
        ROS_INFO("HIT");
        hit = true;
    } else {
        hit = false;
    }
}

InitManager::InitManager(ros::NodeHandle& nodeHandle)
    : nodeHandle(nodeHandle) {
    ROS_INFO("Initializing Init Manager");

    // Interaction DEMO
    {
        const auto pub{nodeHandle.advertise<geometry_msgs::Twist>("/cmd_vel", 1000)};
        // TODO - replace with laserscan
        const auto sub{nodeHandle.subscribe("/turtle1/pose", 1000, pose_callback)};

        ros::Rate loop_rate(100);

        const auto goCurve{[&]() {
            geometry_msgs::Twist curve;
            curve.linear.x = 2;
            curve.angular.x = 2;
            curve.angular.y = 2;
            curve.angular.z = 2;

            pub.publish(curve);
            ros::spinOnce();
            loop_rate.sleep();
            loop_rate.sleep();
        }};

        const auto goLine{[&]() {
            geometry_msgs::Twist line;
            line.linear.x = 3;

            pub.publish(line);
            ros::spinOnce();
            loop_rate.sleep();
        }};

        for(int i = 0; i < 2000000; i++) {
            goLine();
            if(hit) {
                goCurve();
            }
        }
    }
}

}  // namespace twist
