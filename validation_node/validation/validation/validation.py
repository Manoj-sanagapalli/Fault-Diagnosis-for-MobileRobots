#!/usr/bin/python3.7

import rclpy
import time
from rclpy.node import Node

import pickle
import sklearn
from sklearn.externals import joblib
from sklearn.preprocessing import StandardScaler
from sklearn.neighbors import KNeighborsClassifier


from std_msgs.msg import String
from std_msgs.msg import Float64MultiArray
from geometry_msgs.msg import Twist
from nav_msgs.msg import Odometry
from ros2_msg.msg import CurrentMsg

command_vel = 10
actual_vel = 10
actual_angularvel = 10
right_current = 1000
left_current = 1000
bumper = 10
wheeldrop = 1000
class Validation(Node):

    def __init__(self):
        super().__init__('validation')
        self.subscription_cmdvel = self.create_subscription(
            Twist,
            'cmd_vel',
            self.velocity_listner,
        )

        self.publisher_ = self.create_publisher(String, 'prediction', 10)

        self.subscription_odom = self.create_subscription(
            Odometry,
            'odom',
            self.odom_listner,
        )

        self.subscription_current = self.create_subscription(
            CurrentMsg,
            'current',
            self.current_listner,
        )
    def velocity_listner(self,msg):
        global command_vel
        command_vel = msg.linear.x
    def odom_listner(self,msg):
        global actual_vel
        global actual_angularvel
        actual_vel = msg.twist.twist.linear.x
        actual_angularvel = msg.twist.twist.angular.x
    def current_listner(self,msg):
        global right_current
        global left_current
        global bumper
        global wheeldrop
        right_current = msg.right
        left_current = msg.left
        bumper = msg.bumper
        wheeldrop = msg.wheel_drop
        #from platform import python_version
        #print(python_version())
        #import sys
        #print(sys.path)
        
    def testing(self):
        self.get_logger().info('actual velocity : "%f"' % actual_vel)
        self.get_logger().info('actual angular velocity : "%f"' % actual_angularvel)
        datapoint = [command_vel,actual_vel,actual_angularvel,right_current,left_current,bumper,wheeldrop]
        sc = joblib.load('/home/roscon/testing/src/parameters/sc_X.pkl')
        classifier = joblib.load('/home/roscon/testing/src/parameters/clas.pkl')
        #print(sklearn.__version__)
        
        #prediction = classifier.predict([[0.15,0.14,10,30,0,3]])
        prediction = classifier.predict([[command_vel,actual_vel,actual_angularvel,right_current,left_current,bumper,wheeldrop]])
        message = String()
        message.data = 'The prediction class is "%d"' %prediction
        self.publisher_.publish(message)
        print(datapoint)
        print('prediction is "%f"' %prediction)



def main(args=None):
    rclpy.init(args=args)
    validation = Validation()

    while rclpy.ok():
        rclpy.spin_once(validation)
        Validation.testing(validation)
        time.sleep(1)



if __name__ == '__main__':
    main()
    '''
    classifier.predict(sc.transform([[actual_vel,actual_angularvel,right_current,left_current,bumper,wheeldrop]]))
    '''