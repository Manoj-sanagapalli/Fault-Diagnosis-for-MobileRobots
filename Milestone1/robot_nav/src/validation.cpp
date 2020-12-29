#include <iostream>
#include <memory>
#include "rclcpp/rclcpp.hpp"
#include "rclcpp/time.hpp"
#include "rclcpp/clock.hpp"
#include "std_msgs/msg/string.hpp"
#include "geometry_msgs/msg/twist.hpp"
#include "geometry_msgs/msg/twist_stamped.hpp"
#include "nav_msgs/msg/odometry.hpp"
#include "ros2_msg/msg/current_msg.hpp"
#include "sensor_msgs/msg/imu.hpp"
#include <sstream>
#include <math.h>
static nav_msgs::msg::Odometry send_odom;
class validation : public rclcpp::Node
{
  public:
  validation()
  : Node("validation_node"), count_(0), _logger(_logger)
  {
      subscribe_odom= this->create_subscription<nav_msgs::msg::Odometry>(
      "odom",
      rclcpp::QoS(1).best_effort(),
      [this](nav_msgs::msg::Odometry::UniquePtr get_odom) {
      send_odom.pose.pose.orientation.z = get_odom->pose.pose.orientation.z;
      });
  }

private:
  rclcpp::TimerBase::SharedPtr timer_;
  rclcpp::Subscription<nav_msgs::msg::Odometry>::SharedPtr subscribe_odom;
  size_t count_;
  rclcpp::Logger _logger;
};

int main(int argc, char * argv[])
{
    rclcpp::init(argc, argv);
    rclcpp::spin(std::make_shared<validation>());
    return 0;
}