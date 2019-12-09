
// This ros node recieved strings with coordinate through a TCP socket and
// publishes them to ros topic /input using custom message user_input/input_array

// https://www.binarytides.com/socket-programming-c-linux-tutorial/ used for making TCP socket

// For setting up in ros - make sure to source!
// source devel/setup.bash
// echo $ROS_PACKAGE_PATH

// For compiling not in ros
//$ gcc client.c -o client
//$ ./client


#include<stdio.h>
#include<string.h>	//strlen
#include<sys/socket.h>
#include<arpa/inet.h>	//inet_addr
#include<unistd.h>

#include "ros/ros.h"
#include "user_input/person_position.h"


int main(int argc , char *argv[])
{
// Set up ros node
  ros::init(argc, argv, "test_publish");
  ros::NodeHandle n;

  ros::Publisher pub = n.advertise<user_input::person_position>("person_location", 1000);
  user_input::person_position person_pub;

  int i = 0;
  
  float x = 7.0;
  float y = 0.5;
  float th = 0.0;
  

  while (ros::ok())
  {
    person_pub.xpos = x;
    person_pub.ypos = y;
    person_pub.theta = th;
        
    //person_pub.xpos = round(.05*i);
    //person_pub.ypos = round(.02*i);
    //person_pub.theta = round(.001*i);
    pub.publish(person_pub);
    printf("Published \n");
    sleep(2);
    
    x = x + .1*i;
    y = y + .1*i;
    th = th + .01*i;
    
    i++;
  }

	return 0;
}
