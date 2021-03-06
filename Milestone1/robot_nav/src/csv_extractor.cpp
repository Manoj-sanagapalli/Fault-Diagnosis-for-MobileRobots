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

#include <fstream>
int hit=0;
static geometry_msgs::msg::Twist get_Cmdvelocity;
float x,y,z;
int serial=0,label;
std::ofstream fout_cmdVel;

static geometry_msgs::msg::TwistStamped get_Actualvelocity; 
float x_act,y_act,z_act;
int serial_act =0;
std::ofstream fout_actVel;

static ros2_msg::msg::CurrentMsg get_Current;
float right,left,bumper,wheel_drop;
int serial_current= 0;
std::ofstream fout_I;

static sensor_msgs::msg::Imu get_Imudata;
float x_acc,x_ori,y_ori,z_ori;
int serial_imu =0;
std::ofstream fout_IMU;

static nav_msgs::msg::Odometry get_Odom;
int32_t seconds,nanoseconds;



using namespace std::chrono_literals;

class csv_extractor : public rclcpp::Node
{
public:
  csv_extractor()
  : Node("csv_extractor"), count_(0), _logger(_logger)
  {
      subsribe_odom= this->create_subscription<nav_msgs::msg::Odometry>(
          "odom",
          [this](nav_msgs::msg::Odometry::UniquePtr get_Odom) {
              seconds=get_Odom->header.stamp.sec;
              nanoseconds=get_Odom->header.stamp.nanosec;
              x_act = get_Odom->twist.twist.linear.x;
              y_act = get_Odom->twist.twist.linear.y;
              z_act = get_Odom->twist.twist.angular.z;
              if (seconds!=0)
              {
                  fout_actVel.open("output_Avel.xlsx", std::ios::out | std::ios::app);
                  std::cout<<"open"<<std::endl;
                  if (serial_act==0)
                  {
                      fout_actVel << "S.No" << "," << "Time sec" << "," << "Time Nsec" << "," << "linear x" << "," << "linear y" << "," << "angular z" << "\n";
                  }
                  else
                  {
                      fout_actVel << serial_act << "," << seconds << "," << nanoseconds << "," << x_act << "," << y_act << "," << z_act << "\n";
                  }
                  fout_actVel.close();
                  serial_act+=1;
              }
          }
      );
      subsribe_cmdVel= this->create_subscription<geometry_msgs::msg::Twist>(
          "cmd_vel",
          [this](geometry_msgs::msg::Twist::UniquePtr get_Cmdvelocity) {
              x= get_Cmdvelocity->linear.x;
              y= get_Cmdvelocity->linear.y;
              z= get_Cmdvelocity->angular.z;
              if (seconds!=0)
              {
                  fout_cmdVel.open("output_vel.xlsx", std::ios::out | std::ios::app);
                  if (serial==0)
                    {
                        fout_cmdVel << "S.No" << "," << "Time sec" << "," << "Time Nsec" << "," << "linear x" << "," << "linear y" << "," << "angular z" << "\n";
                    }
                  else
                    {
                        fout_cmdVel << serial << "," << seconds << "," << nanoseconds << "," << x << "," << y << "," << z << "\n";
                    }
                fout_cmdVel.close();
                serial+=1;
              }
              hit+=1;
          }
      );
    //   subsribe_actualVel= this->create_subscription<geometry_msgs::msg::TwistStamped>(
    //       "actual_velocity",
    //       [this](geometry_msgs::msg::TwistStamped::UniquePtr get_Actualvelocity) {
    //           x_act= get_Actualvelocity->twist.linear.x;
    //           y_act= get_Actualvelocity->twist.linear.y;
    //           z_act= get_Actualvelocity->twist.angular.z;
              
    //           if (seconds!=0)
    //           {
    //               fout_actVel.open("output_Avel.xlsx", std::ios::out | std::ios::app);
    //               std::cout<<"open"<<std::endl;
    //               if (serial_act==0)
    //               {
    //                   fout_actVel << "S.No" << "," << "Time sec" << "," << "Time Nsec" << "," << "linear x" << "," << "linear y" << "," << "angular z" << "\n";
    //               }
    //               else
    //               {
    //                   fout_actVel << serial_act << "," << seconds << "," << nanoseconds << "," << x_act << "," << y_act << "," << z_act << "\n";
    //               }
    //               fout_actVel.close();
    //               serial_act+=1;
    //           }
    //       }
    //   );
      subsribe_current= this->create_subscription<ros2_msg::msg::CurrentMsg>(
          "current",
          [this](ros2_msg::msg::CurrentMsg::UniquePtr get_Current) {
              right=get_Current->right;
              left=get_Current->left;
              bumper=get_Current->bumper;
              wheel_drop=get_Current->wheel_drop;
              if ((right > 15)|| (left > 15))
              {
                  label=1;
              }
              else
              {
                  label=0;
              }
              if(seconds!=0)
              {
                  fout_I.open("output_current.xlsx", std::ios::out | std::ios::app);
                  if (serial_current==0)
                  {
                      fout_I<<"S.No" << "," << "Time sec" << "," << "Time Nsec" 
                      << "," << "Right current" << "," << "Left current" << ","<< "Bumper"<<","<<"Wheeldrop"<<"\n";
                  }
                  else
                  {
                      fout_I<<serial_current<< "," << seconds << "," << nanoseconds
                       << "," << right<< "," << left << "," << bumper <<","<< wheel_drop <<"\n";
                  }
                  fout_I.close();
                  serial_current+=1;
              }
          }
      );
      subsribe_imu= this->create_subscription<sensor_msgs::msg::Imu>(
          "imu",
          [this](sensor_msgs::msg::Imu::UniquePtr get_Imudata) {
              x_acc=get_Imudata->linear_acceleration.x;
              x_ori=get_Imudata->orientation.x;
              y_ori=get_Imudata->orientation.y;
              z_ori=get_Imudata->orientation.z;
              std::cout<<x_acc<<std::endl;
              if (seconds!=0)
              {
                  fout_IMU.open("output_IMU.xlsx", std::ios::out | std::ios::app);
                  std::cout<<"open"<<std::endl;
                  if (serial_imu==0)
                  {
                      fout_IMU << "S.No" << "," << "Time sec" << "," << "Time Nsec" << "," << "linear x" << "," << "angular x" << "," << "angular y" << "," << "angular z"<< "\n";
                  }
                  else
                  {
                      fout_IMU << serial_imu << "," << seconds << "," << nanoseconds << "," << x_acc << "," << x_ori << "," << y_ori << ","<<z_ori << "\n";
                  }
                  fout_IMU.close();
                  serial_imu+=1;
              }
          }

      );
  }

private:
    rclcpp::TimerBase::SharedPtr timer_;
    rclcpp::Subscription<nav_msgs::msg::Odometry>::SharedPtr subsribe_odom;
    rclcpp::Subscription<geometry_msgs::msg::TwistStamped>::SharedPtr subsribe_actualVel;
    rclcpp::Subscription<geometry_msgs::msg::Twist>::SharedPtr subsribe_cmdVel;
    rclcpp::Subscription<ros2_msg::msg::CurrentMsg>::SharedPtr subsribe_current;
    rclcpp::Subscription<sensor_msgs::msg::Imu>::SharedPtr subsribe_imu;
    size_t count_;
    rclcpp::Logger _logger;
};

int main(int argc, char * argv[])
{
  
  rclcpp::init(argc, argv);
  //rclcpp::WallRate rate(1);
  std::shared_ptr<csv_extractor> node(std::make_shared<csv_extractor>());
  while (rclcpp::ok())
  {
      rclcpp::spin_some(node);
    //  rate.sleep();
      
  }
  rclcpp::shutdown();
  return 0;
}