#ifndef GAZEBO_ROS_JACKS_HH
#define GAZEBO_ROS_JACKS_HH

#include <map>

// Gazebo
#include <gazebo_plugins/gazebo_ros_utils.h>

// ROS
#include <ros/ros.h>
#include <tf/transform_broadcaster.h>
#include <tf/transform_listener.h>
#include <geometry_msgs/Twist.h>
#include <geometry_msgs/Pose2D.h>
#include <nav_msgs/Odometry.h>
#include <sensor_msgs/JointState.h>

// Custom Callback Queue
#include <ros/callback_queue.h>
#include <ros/advertise_options.h>

// Boost
#include <boost/thread.hpp>
#include <boost/bind.hpp>

namespace gazebo {

class Joint;
class Entity;


class GazeboRosJacks : public ModelPlugin {

public:
    GazeboRosJacks();
    ~GazeboRosJacks();
    void Load(physics::ModelPtr _parent, sdf::ElementPtr _sdf);

protected:
    virtual void UpdateChild();
    virtual void FiniChild();

private:
    GazeboRosPtr gazebo_ros_;
    physics::ModelPtr parent;
    //void publishOdometry(double step_time);
    void publishTF(); /// publishes the wheel tf's
    void publishJointState();
    void motorController(double target_jack_CSDE_height, double dt);

    event::ConnectionPtr update_connection_;

    physics::JointPtr joint_jack_CSDE_;
    physics::JointPtr joint_jack_CSNDE_;
    physics::JointPtr joint_jack_NCSDE_;
    physics::JointPtr joint_jack_NCSNDE_;

    double jack_CSDE_torque_;

    std::string robot_namespace_;
    std::string command_topic_;
    std::string robot_base_frame_;

    // ROS STUFF
    ros::Subscriber cmd_jack_CSDE_subscriber_;
    boost::shared_ptr<tf::TransformBroadcaster> transform_broadcaster_;
    sensor_msgs::JointState joint_state_;
    ros::Publisher joint_state_publisher_;

    boost::mutex lock;

    // Custom Callback Queue
    ros::CallbackQueue queue_;
    boost::thread callback_queue_thread_;
    void QueueThread();

    // DiffDrive stuff
    void cmdJackCSDECallback(const geometry_msgs::Point::ConstPtr& cmd_msg);

    /// updates the relative robot pose based on the wheel encoders
    void UpdateJackCSDEEncoder();

    double jack_CSDE_height_cmd_;

    bool alive_;
    double jack_CSDE_height_encoder_;
    common::Time last_encoder_update_;
    double jack_CSDE_height_origin_;
    common::PID  joint_pid_;

    // Update Rate
    double update_rate_;
    double update_period_;
    common::Time last_actuator_update_;

    // Flags
    bool publishTF_;
    bool publishJointState_;

    bool use_velocity_control_;
    double max_velocity_;
};

}

#endif

