#include <iostream>
#include <memory>
#include "rclcpp/rclcpp.hpp"
#include "std_msgs/msg/float32_multi_array.hpp"
std_msgs::msg::Float32MultiArray msg;

using namespace std::chrono_literals;



class float_array : public rclcpp::Node
{
private:
  size_t count_;
  rclcpp::Logger _logger;
  rclcpp::Publisher<std_msgs::msg::Float32MultiArray>::SharedPtr publisher;
    
public:
    float_array()
    :Node("float_array"), count_(0), _logger(_logger)
    {
      
     // msg.layout.dim.push_back(std_msgs::MultiArrayDimension());  
      msg.layout.dim[0].size = 1;
      msg.layout.dim[0].stride = 1;
      //msg.layout.dim[0].label = strings[typ+4];
      msg.data.push_back(1.0);//Write it yourself, feasible
        
        publisher = this->create_publisher<std_msgs::msg::Float32MultiArray>("float");
        
        this->publisher->publish(msg);
    }

    
};

int main(int argc, char * argv[])
{
  
  
  
  std::shared_ptr<float_array> node(std::make_shared<float_array>());
  
  while (rclcpp::ok())
  {
    //node->handle_communication();
    rclcpp::spin(node);
    
  
  }
  return 0;
}

