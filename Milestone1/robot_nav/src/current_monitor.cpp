#include <iostream>
#include <memory>
#include "rclcpp/rclcpp.hpp"
#include "rclcpp/time.hpp"
#include "rclcpp/clock.hpp"
#include "std_msgs/msg/string.hpp"
#include "geometry_msgs/msg/twist.hpp"
#include "nav_msgs/msg/odometry.hpp"
#include "rclcpp/rate.hpp"

#include <sstream>
#include <math.h>
#include <fstream>
#include "ros2_msg/msg/current_msg.hpp"

static ros2_msg::msg::CurrentMsg get_current;

float X,Y,fake_value=0.0;

using namespace std::chrono_literals;

class current_monitor : public rclcpp::Node
{
public:
  current_monitor()
  : Node("current_monitor"), count_(0), _logger(_logger)
  {
      subscribe_odom= this->create_subscription<ros2_msg::msg::CurrentMsg>(
      "current",
      rclcpp::QoS(1).best_effort(),
      [this](ros2_msg::msg::CurrentMsg::UniquePtr msg) {
      get_current.right = msg->right;
      get_current.left = msg->left;
      });
  }
  // void handle_communication() {
  //   RCLCPP_INFO(_logger, "The current in right motor is %d", get_current.right);
  //   RCLCPP_INFO(_logger, "The current in left motor is %d", get_current.left);
  //   }

private:
  rclcpp::TimerBase::SharedPtr timer_;
  rclcpp::Subscription<ros2_msg::msg::CurrentMsg>::SharedPtr subscribe_odom;
  size_t count_;
  rclcpp::Logger _logger;
};

int main(int argc, char * argv[])
{
  rclcpp::init(argc, argv);
  std::shared_ptr<current_monitor> node(std::make_shared<current_monitor>());
  auto period = std::chrono::milliseconds(500);
  while (rclcpp::ok())
  {
    rclcpp::spin_some(node);
    //node->handle_communication();
    rclcpp::sleep_for(period);
  }
  return 0;
}
