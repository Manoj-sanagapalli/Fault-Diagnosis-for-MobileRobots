#include <iostream>
#include <memory>
#include "rclcpp/rclcpp.hpp"
#include "rclcpp/time.hpp"
#include "rclcpp/clock.hpp"
#include "std_msgs/msg/string.hpp"
#include "geometry_msgs/msg/twist.hpp"
#include "nav_msgs/msg/odometry.hpp"
#include "ros2_msg/msg/current_msg.hpp"
#include <sstream>
#include <math.h>
#include <fstream>
#include "kobuki.hpp"
#include <ecl/containers.hpp>

static kobuki::Current * current_data;
static rcutils_time_point_value_t reference_time;
using namespace std::chrono_literals;
class motor_current :public rclcpp::Node
{
  public:
    motor_current()
    : Node("motor_current"), count_(0), _logger(_logger)
    {

    }
  private:
  rclcpp::Logger _logger;
  size_t count_;
};

int main (int argc, char * argv[])
{
  rclcpp::init(argc, argv);
  auto node = rclcpp::Node::make_shared("motor_current");
  auto current_pub = node->create_publisher<ros2_msg::msg::CurrentMsg>("current");
  // float g_max_vx,g_max_vyaw;
  kobuki::Kobuki * getCurrent;
//   kobuki::Parameters params;
//   #ifndef _WIN32
//   params.device_port = "/dev/kobuki";
// #else
//   //
//   // \\?\FTDIBUS#VID_0403+PID_6001+kobuki_AH02B8WIA#0000#{86e0d1e0-8089-11d0-9ce4-08003e301f73}
//   //
//   params.device_port = "\\\\.\\COM1";
// #endif
//   node->get_parameter("device_port", params.device_port);
//   std::string odom_frame = "odom";
//   node->get_parameter("odom_frame", odom_frame);
//   std::string gyro_link_frame = "gyro_link";
//   node->get_parameter("gyro_link_frame", gyro_link_frame);
//   std::string base_link_frame = "base_link";
//   node->get_parameter("base_link_frame", base_link_frame);
//   g_max_vx = 0.5;
//   node->get_parameter("max_vx", g_max_vx);
//   g_max_vyaw = 1.0;
//   node->get_parameter("max_vyaw", g_max_vyaw);

//   RCLCPP_DEBUG(node->get_logger(), "device_port: %s", params.device_port.c_str());
//   RCLCPP_DEBUG(node->get_logger(), "max_vx: %f", g_max_vx);
//   RCLCPP_DEBUG(node->get_logger(), "max_vyaw: %f", g_max_vyaw);

//   params.sigslots_namespace = "/kobuki";
//   params.enable_acceleration_limiter = true;
  getCurrent = new kobuki::Kobuki();
  // getCurrent->init(params);
   getCurrent->enable();
  rclcpp::WallRate loop_rate(5);
  while (rclcpp::ok())
  {
    rcutils_time_point_value_t now;
    if (rcutils_system_time_now(&now) != RCUTILS_RET_OK) {
        RCLCPP_ERROR(node->get_logger(), "Failed to get system time");
      }
      if ((now - reference_time) > 200) {
        reference_time = now;
      }
    //getting current data from bytestream and updating it to the custum topic current with custom message type
    getCurrent->unlockDataAccess();
    int i =getCurrent->getCurrentData().current[0];
    int j =getCurrent->getCurrentData().current[1];
    auto current_msg = std::make_shared<ros2_msg::msg::CurrentMsg>();
    current_msg->header.stamp.sec = RCL_NS_TO_S(now);
    current_msg->header.stamp.nanosec = now - RCL_S_TO_NS(current_msg->header.stamp.sec);
    current_msg->left = getCurrent->getCurrentData().current[1];
    current_msg->right = getCurrent->getCurrentData().current[0];
    current_pub->publish(current_msg);
    rclcpp::spin_some(node);
    loop_rate.sleep();
  }
  getCurrent->disable();
  delete getCurrent;
  return 0;
}

