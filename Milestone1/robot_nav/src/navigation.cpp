#include <iostream>
#include <memory>
#include "rclcpp/rclcpp.hpp"
#include "rclcpp/time.hpp"
#include "rclcpp/clock.hpp"
#include "std_msgs/msg/string.hpp"
#include "geometry_msgs/msg/twist.hpp"
#include "nav_msgs/msg/odometry.hpp"
#include <sstream>
#include <math.h>

#include <fstream>


std::ofstream fout;

static rcutils_time_point_value_t reference_time;
static geometry_msgs::msg::Twist velocity;
static nav_msgs::msg::Odometry init_odom;
static nav_msgs::msg::Odometry send_odom;
//time now,conditon;

float X,Y,Z,fake_value=0.0;
float od;

using namespace std::chrono_literals;

class navigation : public rclcpp::Node
{
public:
  navigation()
  : Node("navigation"), count_(0), _logger(_logger)
  {
      publisher_ = this->create_publisher<geometry_msgs::msg::Twist>("cmd_vel");
      publish_record = this->create_publisher<nav_msgs::msg::Odometry>("test");
      
      subscribe_odom= this->create_subscription<nav_msgs::msg::Odometry>(
      "odom",
      rclcpp::QoS(1).best_effort(),
      [this](nav_msgs::msg::Odometry::UniquePtr get_odom) {
      send_odom.pose.pose.orientation.z = get_odom->pose.pose.orientation.z;
      });
      
  }
  void set_linear_vel() {
    velocity.linear.x=0.15 ;
    velocity.linear.y=0.0;
    velocity.linear.z=0.0;
    velocity.angular.x=0.0;
    velocity.angular.y=0.0;
    velocity.angular.z=0.0;
  }
  void set_angular_vel() {
    velocity.linear.x=0.0;
    velocity.linear.y=0.0;
    velocity.linear.z=0.0;
    velocity.angular.x=0.0;
    velocity.angular.y=0.0;
    velocity.angular.z=0.15;
  }


  void handle_communication() {
    //  to move in rectangle
    // rcutils_time_point_value_t now;
    // if (rcutils_system_time_now(&now) != RCUTILS_RET_OK) {
    //   }
    // if(std::abs(Z-send_odom.pose.pose.orientation.z)>=0.5)
    // {
    //   reference_time=now;
    // }
    // float time =(RCL_NS_TO_S(now)-RCL_NS_TO_S(reference_time));

    // if(time < 4)
    // {
    //   set_linear_vel();
    //   Z=send_odom.pose.pose.orientation.z;
    // }
    // else if (std::abs(Z-send_odom.pose.pose.orientation.z)<=0.5)
    // {
    //   set_angular_vel();
    // }
    
  rcutils_time_point_value_t now;
    if (rcutils_system_time_now(&now) != RCUTILS_RET_OK) {
      }
  float time =(RCL_NS_TO_S(now)-RCL_NS_TO_S(reference_time));
  // if (time < 6)
  // {
  //   set_linear_vel();
  // }
  // else if (6<=time<15)
  // {
  //   set_angular_vel();
  // }
  
  if (time>=10)
  {
    std::cout<<"shutting down"<<std::endl;
    rclcpp::shutdown();
  }
  set_linear_vel();
  this->publisher_->publish(velocity);
  }

private:
  rclcpp::TimerBase::SharedPtr timer_;
  rclcpp::Publisher<geometry_msgs::msg::Twist>::SharedPtr publisher_;
  rclcpp::Publisher<nav_msgs::msg::Odometry>::SharedPtr publish_record;
  rclcpp::Subscription<nav_msgs::msg::Odometry>::SharedPtr subscribe_odom;
  rclcpp::Publisher<nav_msgs::msg::Odometry>::SharedPtr pub_fakeodom_;
  size_t count_;
  rclcpp::Logger _logger;
};

int main(int argc, char * argv[])
{
  
  rclcpp::init(argc, argv);
  rclcpp::WallRate Rate(20);
  if (rcutils_system_time_now(&reference_time) != RCUTILS_RET_OK) {
  }
  
  std::shared_ptr<navigation> node(std::make_shared<navigation>());
  init_odom = send_odom;
  while (rclcpp::ok())
  {
    rclcpp::spin_some(node);
    node->handle_communication();
    Rate.sleep();
  }
  return 0;
}

