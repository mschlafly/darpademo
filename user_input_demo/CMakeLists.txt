cmake_minimum_required(VERSION 2.8.3)
project(user_input_demo)


find_package(catkin REQUIRED COMPONENTS
  roscpp
  rospy
  std_msgs
  message_generation
)

add_message_files(
  FILES
  input_array.msg
)

generate_messages(
  DEPENDENCIES
  std_msgs
)


catkin_package( CATKIN_DEPENDS message_runtime )

include_directories(include ${catkin_INCLUDE_DIRS})

add_executable(client src/client.cpp)
add_executable(test_publish src/test_publish.cpp)
target_link_libraries(client ${catkin_LIBRARIES})
target_link_libraries(test_publish ${catkin_LIBRARIES})
add_dependencies(client ${catkin_EXPORTED_TARGETS})
add_dependencies(test_publish ${catkin_EXPORTED_TARGETS})
add_dependencies(client user_input_generate_messages_cpp)
add_dependencies(test_publish user_input_generate_messages_cpp)
