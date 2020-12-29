from launch import LaunchDescription
import launch_ros.actions

def generate_launch_description():
    return LaunchDescription([
        #launch_ros.actions.Node(
         #   package='robot_nav', node_executable='vel_publisher', output='screen'
        #),
        launch_ros.actions.Node(
            package='turtlebot2_drivers', node_executable='kobuki_node', output='screen'
        ),
        launch_ros.actions.Node(
            package='robot_nav', node_executable='navigation_node', output='screen'
        ),
    ]
    )


