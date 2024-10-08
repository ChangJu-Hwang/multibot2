#pragma once

#include <memory>

#include <nav2_util/lifecycle_node.hpp>

#include <tf2_ros/buffer.h>
#include <tf2_ros/transform_listener.h>

#include <tf2_eigen/tf2_eigen.h>

#include <tf2/LinearMath/Quaternion.h>
#include <tf2/LinearMath/Matrix3x3.h>

#include <nav2_costmap_2d/costmap_2d_ros.hpp>
#include <nav2_costmap_2d/cost_values.hpp>

#include "multibot2_util/base_robot_info.h"
#include "multibot2_util/panel_util.h"

#include <geometry_msgs/msg/twist.hpp>
#include <nav_msgs/msg/odometry.hpp>
#include <nav_msgs/msg/path.hpp>

#include "costmap_converter/costmap_to_polygons.h"

#include <Eigen/Geometry>

#include "multibot2_msgs/msg/robot_state.hpp"
#include "multibot2_msgs/msg/task.hpp"
#include "multibot2_msgs/msg/neighbors.hpp"
#include "multibot2_msgs/msg/obstacle_array_msg.hpp"
#include "multibot2_msgs/srv/queue_rivision.hpp"

namespace multibot2_robot
{
    typedef multibot2_util::BaseRobotInfo Robot;

    struct Robot_ROS
    {
    public:
        typedef multibot2_util::Pose Pose;

        typedef multibot2_msgs::msg::RobotState RobotState;
        typedef multibot2_msgs::msg::Task Task;
        typedef multibot2_msgs::msg::Neighbor Neighbor;
        typedef multibot2_msgs::msg::Neighbors Neighbors;
        typedef multibot2_msgs::msg::ObstacleArrayMsg ObstacleArrayMsg;
        typedef multibot2_msgs::srv::QueueRivision QueueRivision;
        typedef multibot2_util::PanelUtil::ModeSelection ModeSelection;
        typedef multibot2_util::PanelUtil::Mode Mode;

    public:
        Robot_ROS() {}

        Robot_ROS(const Robot &_robot) : robot_(_robot) {}

        Robot_ROS(const Robot_ROS &_robot_ros);

        ~Robot_ROS() {}

    public:
        Robot_ROS &operator=(const Robot_ROS &_rhs);

    public:
        inline Robot &robot() { return robot_; }
        inline const Robot &robot() const { return robot_; }

        inline Pose &subgoal() { return subgoal_; }
        inline const Pose &subgoal() const { return subgoal_; }

        inline Neighbors &neighbors() { return neighbors_; }
        inline const Neighbors &neighbors() const { return neighbors_; }

        inline double &task_duration() { return task_duration_; }
        inline const double &task_duration() const { return task_duration_; }

        inline Mode &mode() { return mode_; }
        inline const Mode &mode() const { return mode_; }

        inline rclcpp_lifecycle::LifecyclePublisher<RobotState>::SharedPtr &state_pub() { return state_pub_; }
        inline const rclcpp_lifecycle::LifecyclePublisher<RobotState>::SharedPtr &state_pub() const { return state_pub_; }

        inline rclcpp_lifecycle::LifecyclePublisher<geometry_msgs::msg::Twist>::SharedPtr &cmd_vel_pub() { return cmd_vel_pub_; }
        inline const rclcpp_lifecycle::LifecyclePublisher<geometry_msgs::msg::Twist>::SharedPtr &cmd_vel_pub() const { return cmd_vel_pub_; }

        inline rclcpp::Subscription<nav_msgs::msg::Odometry>::SharedPtr &odom_sub() { return odom_sub_; }
        inline const rclcpp::Subscription<nav_msgs::msg::Odometry>::SharedPtr &odom_sub() const { return odom_sub_; }

        inline rclcpp::Subscription<Task>::SharedPtr &task_sub() { return task_sub_; }
        inline const rclcpp::Subscription<Task>::SharedPtr &task_sub() const { return task_sub_; }

        inline rclcpp::Subscription<geometry_msgs::msg::PoseStamped>::SharedPtr &goal_sub() { return goal_sub_; }
        inline const rclcpp::Subscription<geometry_msgs::msg::PoseStamped>::SharedPtr &goal_sub() const { return goal_sub_; }

        inline rclcpp::Subscription<geometry_msgs::msg::PoseStamped>::SharedPtr &subgoal_sub() { return subgoal_sub_; }
        inline const rclcpp::Subscription<geometry_msgs::msg::PoseStamped>::SharedPtr &subgoal_sub() const { return subgoal_sub_; }

        inline rclcpp::Subscription<Neighbors>::SharedPtr &neighbors_sub() { return neighbors_sub_; }
        inline const rclcpp::Subscription<Neighbors>::SharedPtr &neighbors_sub() const { return neighbors_sub_; }

        inline rclcpp::Client<Robot_ROS::QueueRivision>::SharedPtr &queue_revision() { return queue_revision_; }
        inline const rclcpp::Client<Robot_ROS::QueueRivision>::SharedPtr &queue_revision() const { return queue_revision_; }

        inline rclcpp_lifecycle::LifecyclePublisher<nav_msgs::msg::Path>::SharedPtr &rviz_path_pub() { return rviz_path_pub_; }
        inline const rclcpp_lifecycle::LifecyclePublisher<nav_msgs::msg::Path>::SharedPtr &rviz_path_pub() const { return rviz_path_pub_; }

        inline rclcpp_lifecycle::LifecyclePublisher<ObstacleArrayMsg>::SharedPtr &local_obstacles_pub() { return local_obstacles_pub_; }
        inline const rclcpp_lifecycle::LifecyclePublisher<ObstacleArrayMsg>::SharedPtr &local_obstacles_pub() const { return local_obstacles_pub_; }

    protected:
        Robot robot_;
        Pose subgoal_;

        Neighbors neighbors_;

        double task_duration_{0.0};

        Mode mode_{Mode::MANUAL};

    protected:
        rclcpp_lifecycle::LifecyclePublisher<RobotState>::SharedPtr state_pub_;

        rclcpp_lifecycle::LifecyclePublisher<geometry_msgs::msg::Twist>::SharedPtr cmd_vel_pub_;

        rclcpp::Subscription<nav_msgs::msg::Odometry>::SharedPtr odom_sub_;

        rclcpp::Subscription<Task>::SharedPtr task_sub_;

        rclcpp::Subscription<geometry_msgs::msg::PoseStamped>::SharedPtr goal_sub_;

        rclcpp::Subscription<geometry_msgs::msg::PoseStamped>::SharedPtr subgoal_sub_;

        rclcpp::Subscription<Neighbors>::SharedPtr neighbors_sub_;

        rclcpp::Client<Robot_ROS::QueueRivision>::SharedPtr queue_revision_;

        rclcpp_lifecycle::LifecyclePublisher<nav_msgs::msg::Path>::SharedPtr rviz_path_pub_;

        rclcpp_lifecycle::LifecyclePublisher<ObstacleArrayMsg>::SharedPtr local_obstacles_pub_;
    }; // struct Robot_ROS

    class Instance_Manager
    {
    public:
        typedef std::unique_ptr<Instance_Manager> UniquePtr;
        typedef std::shared_ptr<Instance_Manager> SharedPtr;

    public:
        Instance_Manager() {}

        Instance_Manager(nav2_util::LifecycleNode::SharedPtr &_nh);

        ~Instance_Manager();

    public:
        inline Robot &robot() { return robot_ros_.robot(); }
        inline const Robot &robot() const { return robot_ros_.robot(); }

        inline Robot_ROS &robot_ros() { return robot_ros_; }
        inline const Robot_ROS &robot_ros() const { return robot_ros_; }

        inline std::shared_ptr<nav2_costmap_2d::Costmap2DROS> &global_costmap_ros() { return global_costmap_ros_; }
        inline const std::shared_ptr<nav2_costmap_2d::Costmap2DROS> &global_costmap_ros() const { return global_costmap_ros_; }

        inline std::shared_ptr<nav2_costmap_2d::Costmap2DROS> &local_costmap_ros() { return local_costmap_ros_; }
        inline const std::shared_ptr<nav2_costmap_2d::Costmap2DROS> &local_costmap_ros() const { return local_costmap_ros_; }

    public:
        void init_variables();
        void init_parameters();

        const nav2_costmap_2d::Costmap2D get_fake_local_costmap() const { return get_fake_costmap(*local_costmap_ros_->getCostmap()); };

    protected:
        void update_state()
        {
            update_pose();
            report_state();
        }

        void update_pose();

        void report_state();

        void report_local_obstacles();

    protected:
        void odom_callback(const nav_msgs::msg::Odometry::SharedPtr _odom_msg);

        void task_callback(const Robot_ROS::Task::SharedPtr _task_msg);

        void goal_callback(const geometry_msgs::msg::PoseStamped::SharedPtr _goal_msg);

        void subgoal_callback(const geometry_msgs::msg::PoseStamped::SharedPtr _subgoal_msg);

        void neighbors_callback(const Robot_ROS::Neighbors::SharedPtr _neighbors_msg) { robot_ros_.neighbors() = *_neighbors_msg; }

    protected:
        const nav2_costmap_2d::Costmap2D get_fake_costmap(const nav2_costmap_2d::Costmap2D _original_costmap) const;

    protected:
        nav2_util::LifecycleNode::SharedPtr nh_;
        rclcpp::TimerBase::SharedPtr update_timer_;
        rclcpp::TimerBase::SharedPtr local_obstacles_update_timer_;

        std::shared_ptr<tf2_ros::TransformListener> tf_listener_{nullptr};
        std::unique_ptr<tf2_ros::Buffer> tf_buffer_;

    protected:
        Robot_ROS robot_ros_;

    protected:
        std::shared_ptr<nav2_costmap_2d::Costmap2DROS> global_costmap_ros_;
        std::unique_ptr<nav2_util::NodeThread> global_costmap_thread_;

        std::shared_ptr<nav2_costmap_2d::Costmap2DROS> local_costmap_ros_;
        std::unique_ptr<nav2_util::NodeThread> local_costmap_thread_;

    }; // class Instance_Manager
} // namespace multibot2_robot