#include <iostream>
#include <memory>
#include "rclcpp/rclcpp.hpp"
#include "rclcpp/time.hpp"
#include "rclcpp/clock.hpp"
#include "std_msgs/msg/string.hpp"
#include "std_msgs/msg/float64_multi_array.hpp"
#include "geometry_msgs/msg/twist_stamped.hpp"
#include "nav_msgs/msg/odometry.hpp"
#include <sstream>
#include <math.h>
#include <fstream>


static geometry_msgs::msg::TwistStamped velocity;
static nav_msgs::msg::Odometry init_odom;
static nav_msgs::msg::Odometry send_odom;
static std_msgs::msg::Float64MultiArray Floatmul ;




using namespace std::chrono_literals;
//auto offset = std::chrono::milliseconds(200);
auto offset = std::chrono::nanoseconds(200000000) ; //0.2 second
const float conv=pow(10,-9);

class navigation : public rclcpp::Node
{
public:
  navigation()
      : Node("navigation"), count_(0), _logger(_logger)
  {
    publisher_ = this->create_publisher<geometry_msgs::msg::TwistStamped>("actual_velocity");
    subscribe_odom = this->create_subscription<nav_msgs::msg::Odometry>(
        "odom",
        rclcpp::QoS(1).best_effort(),
        [this](nav_msgs::msg::Odometry::UniquePtr get_odom) {
          send_odom.pose.pose.position.x = get_odom->pose.pose.position.x;
          send_odom.pose.pose.position.y = get_odom->pose.pose.position.y;
          send_odom.pose.pose.position.z = get_odom->pose.pose.position.z;
          send_odom.header.stamp.sec = get_odom->header.stamp.sec;
          send_odom.header.stamp.nanosec = get_odom->header.stamp.nanosec;
        });
  }
  void handle_communication()
  {
    //float time =send_odom.header.stamp.sec-init_odom.header.stamp.sec+ (send_odom.header.stamp.nanosec - init_odom.header.stamp.nanosec) * (pow(10,-9));
    float time =0.2;
    //std::cout<<"inside function time :"<<time<<std::endl;
    float vx = (std::abs(send_odom.pose.pose.position.x - init_odom.pose.pose.position.x)) / time;
    float vy = (std::abs(send_odom.pose.pose.position.y - init_odom.pose.pose.position.y)) / time;
    velocity.twist.linear.x = vx;
    velocity.twist.linear.y = vy;
    velocity.twist.linear.z = 0.0;
    velocity.twist.angular.x = 0.0;
    velocity.twist.angular.y = 0.0;
    velocity.twist.angular.z = 0.0;
    velocity.header.stamp.sec = send_odom.header.stamp.sec;
    velocity.header.stamp.nanosec = send_odom.header.stamp.nanosec;
    this->publisher_->publish(velocity);
  }

private:
  rclcpp::Publisher<geometry_msgs::msg::TwistStamped>::SharedPtr publisher_;
  rclcpp::Subscription<nav_msgs::msg::Odometry>::SharedPtr subscribe_odom;
  size_t count_;
  rclcpp::Logger _logger;
};

int main(int argc, char *argv[])
{
  rclcpp::init(argc, argv);
  std::shared_ptr<navigation> node(std::make_shared<navigation>());
  while (rclcpp::ok())
  {
    rclcpp::spin_some(node);
    init_odom = send_odom;
    rclcpp::sleep_for(offset);
    rclcpp::spin_some(node);
    node->handle_communication();
  }
  return 0;
}
