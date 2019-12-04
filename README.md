# darpademo
This code is for demoing how a human input can be used in control algorithm. It is a pared down version of the code used in the experiment. It consists of three files that are all run during the experiment. 
1. TouchDetection - This program utilizes the Universal Windows Platform touch detection C# SDK for app development to record the locations of user inputs. Input recording starts and ends with a double tap. The double tap to end recording also saves the data to text file "touch_locations.txt" in the Downloads folder on the computer. This program would contain any image to be displayed in the .xaml file.
2. TCP_socket - Server side of a TCP socket using the winsock library. This code (a) creates a TCP socket for communicating from a windows OS and linux OS, (b) connects to client on linux OS and tests the socket, (c) continuously sends over coordinates for touch input (if new input) and recieves coordinates for the person's position and orientation in unity, writing them to a text file if the position has changed. The coodinates are send over in batched to avoid any potential loss of data.
3. user_input - ROS package that is run on the linux OS for communicating over the TCP socket, publishing touch input locations, and subscribing to a message containing the person's position. It contains two custom messages, input_array.msg and person_position.msg, publishes/subscribed to two topics, "input" and "person_location", and contains one rosnode client.cpp. After sourcing, using catkin_make and roscore, the node can be run with "rosrun user_input client" or from a launch file. The most up-to-date version of this code be found on Ahalya's github.

## System requirements and how to set up and run code
Programs 1-3 must be run from a Windows OS using Visual Studio (VS). It may be necessary to download additional VS packages to use Universal Windows Platform. 

For networking, both the TCP server and client must be connected to the same router and be connected to the same IP address and port defined as a variable in the top of both files. Northwestern wifi does not work; it is best to use a private network in which you have access to the network settings. If you experience problems, you may need to connect to a static IP address and/or disable the security features on the computer. user_input was created on ROS melodic. 

0. Open the tanvas engine application and calibrate monitor by pressing "Calibrate". 
1. Edit file path variables at the top of programs 1-3 and edit the IP address and port at the top of programs 3 and 4.
2. Disable firewall (on Dell laptop using Mcafee application)
3. Connect to static IP: connect to private network (in our case Netgear04-5G), turn off firewall (on Dell laptop using Mcafee application), set static IP by going to the control panel, clicking "Network settings and tasks" then "Chage adapter settings", left click on "Wi-Fi" and select "Properties", "Internet Protocol version 4 TCP/IPv4", then "Properties" again. Select "Use the following IP address" and set the IP address to the one used at the top of the TCP socket code (192.168.1.3 in our case) and the subnet mask should auto full to 255.255.255.0.
3. Open the solution file (ending in .sln) for programs 1-3 on the laptop and run in no particular order by clicking the play button in VS.
4. Run the client side of the TCP socket (program 4) within ROS.

## Important parameters
h_unity - This variable controls the amount to which the montior display zooms in on the environment. The number refers to the number of units in the Unity environment will fit vertically on the monitor. We are using 10. This number is used to determine zoom_ratio which gives the number of pixels on the monitor that is equal to a unit length in unity. This variable needs to be changed in Haptic display, TouchDetection_UWPcs, and ALL of the python scripts for generating bitmaps which will all have to be rerun to incorporate this change.

IPv4/PORT - These determine the IP address and port that the TCP socket connects to. These must be the same in the server and client.

docpath_touch/docpath_person - Both are defined in the beginning of the TCP server (program 4), docpath_person in Haptic_display, and docpath_touch in TouchDetection_UWPcs.
 
