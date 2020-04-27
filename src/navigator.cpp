#include <ros/ros.h>
#include<iostream>
#include <geometry_msgs/Twist.h>

class RobotDriver
{
private:
  //! The node handle we'll be using
  ros::NodeHandle nh_;
  //! We will be publishing to the "/base_controller/command" topic to issue commands
  ros::Publisher cmd_vel_pub_;

public:
  //! ROS node initialization
  RobotDriver(ros::NodeHandle &nh)
  {
    nh_ = nh;
    //set up the publisher for the cmd_vel topic
    cmd_vel_pub_ = nh_.advertise<geometry_msgs::Twist>("cmd_vel", 0.5);
  }

  //! Loop forever while sending drive commands based on keyboard input
  bool driveKeyboard()
  {
    std::cout << "Type a command and then press enter.  "
      "Use '8' to move forward, '4' to turn left, "
      "'6' to turn right,'5' to go back ,'0' to exit.\n";

    //We will be sending commands of type "twist"
    geometry_msgs::Twist base_cmd;

    int cmd;
    base_cmd.linear.x = base_cmd.linear.y = base_cmd.angular.z = 0;
    while(nh_.ok())
    {

      std::cout<<"Please enter the command = ";
      std::cin>>cmd;
      
      //Check if the input is valid or not
      if(cmd!=8 && cmd!=2 && cmd!=0 && cmd!=4 && cmd!=5 && cmd!=6)
      {
        std::cout << "unknown command:" << cmd << "\n";
        continue;
      }
      //Move forward
      if(cmd == 8)
      {
      	base_cmd.linear.y = base_cmd.angular.z = 0;
        base_cmd.linear.x = 0.5;
      }
      //Turn left (yaw) and drive forward at the same time
      else if(cmd == 4)
      {
        base_cmd.angular.z = 0.5;
      }
      //Turn right (yaw) and drive forward at the same time
      else if(cmd == 6)
      {
        base_cmd.angular.z = -0.5;
      }
      //Go in reverse direction 
      else if(cmd == 5)
      {
      	base_cmd.angular.z = 0;
      	base_cmd.linear.x = -0.5;
      	base_cmd.linear.y = 0;
      }
      //Stop
      else if(cmd == 2)
      {
      	base_cmd.linear.x = base_cmd.linear.y = base_cmd.angular.z = 0;
      }
      //Exit the program
      else if(cmd== 0)
      {
      	std::cout<<"Stoping the game !!";
      	base_cmd.linear.x = base_cmd.linear.y = base_cmd.angular.z = 0;
        cmd_vel_pub_.publish(base_cmd);
      	break;
      }

      //Publish the assembled command
      cmd_vel_pub_.publish(base_cmd);
    }
    return true;
  }

};

int main(int argc, char** argv)
{
  //init the ROS node
  ros::init(argc, argv, "robot_driver");
  ros::NodeHandle nh;

  RobotDriver driver(nh);
  driver.driveKeyboard();
}