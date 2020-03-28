# darpademo
This code was used during FX3 to demo so that a human input could be used to drive the ergodic control algorithm. It is a pared down version of the code used in the human subject experiment at mschlafly/darpahaptics.git. It consists of three files that are all run during the experiment. 

Information gets transferred between programs using txt file ``touch_locations.txt". You can ``hack" the system for testing or training by changing the values in the txt files.

1. TouchDetection - This program utilizes the Universal Windows Platform touch detection C# SDK for app development to record the locations of user inputs. Input recording starts and ends with a double tap. The double tap to end recording also saves the data to text file "touch_locations.txt" in the Downloads folder on the computer. This program would contain any image to be displayed in the MainPage.xaml file. Note, if you would like to use your own image, you must include it in the project by going to the Solution Explorer, clicking "Show all files", then right clicking the file and chose "Include in Project".
2. TCP_socket - Server side of a TCP socket using the winsock library. Multiple robots can connect and receive messages from this server. This code (a) creates a TCP socket for communicating between a windows OS and linux OS, (b) connects to client on linux OS and tests the socket, (c) continuously sends over coordinates for the touch input (read from "touch_locations.txt" in the Downloads folder). The coordinates are send over in batches to avoid any potential loss of data. The coordinates are send over in batches to avoid any potential loss of data. Our project-specific code can be found within the function EchoIncomingPackets in threaded-server.cpp.
3. tanvas_comms (In the ROS repo)  - ROS package that is run on the linux OS for communicating over the TCP socket, publishing touch input locations, and subscribing to a message containing the person's position. It contains two custom messages, input_array.msg and person_position.msg, publishes/subscribed to two topics, "input" and "person_location", and contains one rosnode client.cpp. After sourcing, using catkin_make and roscore, the node can be run with "rosrun tanvas_comms client" or from a launch file. The most up-to-date version of this code be found on Ahalya's github.

## System requirements and how to set up and run code
To use the TanvasTouch, you must use a Windows OS and Visual Studio (VS). For touch detection, we use the Universal Windows Platform API. It may be necessary to download additional VS packages to use Universal Windows Platform. For reference, we used Windows 10.0 build 18362 and Microsoft Visual Studio Community 2019. The ROS side of the TCP socket in package user_input was created on ROS melodic. 

For networking, both the TCP server and client must be connected to the same router and be connected to the same IP address and port defined as a variable in the top of both files. It is best to use a private network in which you have access to the network settings. If you experience problems, you may need to connect to a static IP address and/or disable the security features on the computer. 

When you first receive your TanvasTouch, follow all of the instructions for setting up the device provided by the company, including setting the ``Main display".

0. Connect the TanvasTouch (all 3 cords),open the tanvas engine application, and calibrate monitor by pressing "Calibrate".
1. Connect to static IP: connect to private network, turn off any and all firewalls (on XPS laptop using Mcafee application), set static IP by going to the control panel through the Start menu, clicking "Network settings and tasks" then "Change adapter settings", left click on "Wi-Fi" and select "Properties", "Internet Protocol version 4 TCP/IPv4", then "Properties" again. Select "Use the following IP address" and set the IP address to the one used at the top of the TCP socket code (192.168.1.3 in our case) and the subnet mask should auto full to 255.255.255.0.
2. Open the touch detection code in VS from the solution file, ``sharedcontrol.sln", and the TCPserver code from solution file ``TCPserver.sln". Access the source code through the Solution Explorer.
3. The TCPserver will need to be run from the command line. However, accessing the C compiler though VS can requires a couple of steps. (Instructions are also provided at the top of main.cpp.)
4. Make sure the same IP address and port are defined at the top of client.cpp. After sourcing, using catkin\_make and roscore, the node can be run with "rosrun tanvas\_comms client" or from a launch file.
5. Copy and paste the files (sound1.wav, sound2.wav, and ``touch\_locations.txt") in TouchDetection\_demo to the Downloads folder. (The Downloads folder is used so that you do not have to edit the file permission settings on your computer. It may be helpful if you emptied your Downloads folder as well.) 
6. To run the touch detection app ``sharedcontrol", find the play button at the top of the VS project and click it. Make sure that the drop down menus next to it reads ``Debug" and either ``x64" or ``x86". When this file is run, you will be asked to pick files from the Downloads folder. 
7. Make sure the app is shown on the main screen. Send commands by double tapping, shading the region of interest, and double tapping again to send the message. The locations of the taps are included in the message. You should hear a ``beep" after every recieved double-tap (if your computer sound is on).

## Things to try if code is not working properly

Problems with the TCP socket:
-Check that firewalls are off, all computers are connected to the correct network, and the right IP addresses and ports are set.
-Check that ``touch\_locations.txt" in the downloads folder does not have a ton of numbers in the file. Sometimes when the file is too large, it will take too long to read, and the TCP socket will close. The contents of this file can be deleted manually.
-Try pressing Ctr C in the TCP server terminal (if messages have stopped coming out). There is some kind of windows problem where a program will stop running and pressing Ctr C gets it to continue. 

Tips for tap detection: Getting the Tanvas to recognize a tap can sometimes be tricky. Here are some tips.
-Recalibrating using the tanvas engine.
-Pay attention to the frequency of your taps for both the double-tap and the taps. The taps are slower than the double-tap.
-Keep your finger close to the screen. 
-Even though you must tap slower than you might want to, don't leave your finger on the surface of the tanvas for too long.
-Hitting the tanvas harder doesn't help.

Tips for haptic recognition: 
-Move your finger lightly across the surface. Putting too much pressure on the surface will cause your finger to get sweaty, making the haptics more difficult to sense.
-To sense the direction a haptic object is oriented, move your finger back-and-forth along its edge.
-Visuals make it easier. Although we did not use visuals during the experiment, we did use examples for training.

Finally, try restarting the programs or the computer.

