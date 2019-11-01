#ifndef SPEED_PLANNER_NODE_ROS_H
#define SPEED_PLANNER_NODE_ROS_H

#include <autoware_msgs/Lane.h>
#include <autoware_msgs/VehicleStatus.h>
#include <autoware_msgs/DetectedObjectArray.h>
#include <ros/ros.h>
#include <Eigen/Eigen>
#include <vector>
#include <iostream>
#include <random>
#include <chrono>
#include <cstring>
#include <memory>
#include <visualization_msgs/MarkerArray.h>
#include <visualization_msgs/Marker.h>
#include <tf/transform_datatypes.h>
#include <tf2_ros/transform_listener.h>
#include <tf2_geometry_msgs/tf2_geometry_msgs.h>
#include "speed_planner/reference_trajectory.h"
#include "speed_planner/convex_speed_optimizer.h"

class SpeedPlannerNode
{
    public:
        SpeedPlannerNode();
        ~SpeedPlannerNode() = default;

    private:
        ros::NodeHandle nh_, private_nh_;
        ros::Publisher optimized_waypoints_pub_;
        ros::Publisher optimized_waypoints_debug_;
        ros::Subscriber current_status_sub_;
        ros::Subscriber current_pose_sub_;
        ros::Subscriber current_velocity_sub_;
        ros::Subscriber final_waypoints_sub_;
        ros::Subscriber nav_goal_sub_;
        ros::Subscriber objects_sub_;

        ros::Timer timer_;

        std::unique_ptr<tf2_ros::Buffer> tf2_buffer_ptr_;
        std::unique_ptr<tf2_ros::TransformListener> tf2_listener_ptr_;
  
        std::unique_ptr<autoware_msgs::Lane> in_lane_ptr_;
        std::unique_ptr<autoware_msgs::VehicleStatus> in_status_ptr_;
        std::unique_ptr<geometry_msgs::PoseStamped> in_pose_ptr_;
        std::unique_ptr<geometry_msgs::TwistStamped> in_twist_ptr_;
        std::unique_ptr<geometry_msgs::PoseStamped> in_nav_goal_ptr_;
        std::unique_ptr<autoware_msgs::DetectedObjectArray> in_objects_ptr_;
  
        std::unique_ptr<ConvexSpeedOptimizer> speedOptimizer_;

        void waypointsCallback(const autoware_msgs::Lane& msg);
        void objectsCallback(const autoware_msgs::DetectedObjectArray& msg);
        void currentStatusCallback(const autoware_msgs::VehicleStatus& msg);
        void currentPoseCallback(const geometry_msgs::PoseStamped& msg);
        void currentVelocityCallback(const geometry_msgs::TwistStamped& msg);
        void navGoalCallback(const geometry_msgs::PoseStamped& msg);
        void timerCallback(const ros::TimerEvent &e);

        double curvatureWeight_;
        double decayFactor_;
        bool isInitialize_;
        double previousVelocity_;
        double timer_callback_dt_;
};

#endif