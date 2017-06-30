cmake_minimum_required(VERSION 2.8.3)
project(synchronization)
find_package(catkin REQUIRED COMPONENTS cv_bridge image_transport message_generation sensor_msgs)
set ( CMAKE_PREFIX_PATH /opt/ros/kinetic/share/OpenCV-3.2.0-dev/)
set ( CXX_FLAGS "-g -W -Wall -Wextra -pedantic -march=native" )
#set ( CMAKE_CXX_COMPILER clang++ )
ADD_DEFINITIONS (-std=c++11)


find_package(OpenCV)
find_package( Boost COMPONENTS date_time REQUIRED )
include_directories(include ${catkin_INCLUDE_DIRS} ${OpenCV_INCLUDE_DIRS})
# add the resized image message
add_message_files(DIRECTORY msg
   FILES Messages.msg
)
generate_messages(DEPENDENCIES sensor_msgs)

#catkin_package(CATKIN_DEPENDS message_runtime sensor_msgs)


# add the publisher example
add_executable(my_publisher src/my_publisher.cpp src/common.cpp src/common.h )
add_dependencies(my_publisher ${catkin_EXPORTED_TARGETS} ${${PROJECT_NAME}_EXPORTED_TARGETS})
target_link_libraries(my_publisher ${catkin_LIBRARIES} ${OpenCV_LIBRARIES} ${Boost_LIBRARIES})

# add the subscriber example
add_executable(my_subscriber src/my_subscriber.cpp src/common.cpp src/common.h )
add_dependencies(my_subscriber ${catkin_EXPORTED_TARGETS} ${${PROJECT_NAME}_EXPORTED_TARGETS})
target_link_libraries(my_subscriber ${catkin_LIBRARIES} ${OpenCV_LIBRARIES} ${Boost_LIBRARIES})


add_executable(talker src/talker.cpp src/common.cpp src/common.h )
add_dependencies(talker ${catkin_EXPORTED_TARGETS} ${${PROJECT_NAME}_EXPORTED_TARGETS})
target_link_libraries(talker ${catkin_LIBRARIES} ${OpenCV_LIBRARIES} ${Boost_LIBRARIES})
# add the plugin example



# Mark executables and/or libraries for installation
install(TARGETS my_publisher my_subscriber
  ARCHIVE DESTINATION ${CATKIN_PACKAGE_LIB_DESTINATION}
  LIBRARY DESTINATION ${CATKIN_PACKAGE_LIB_DESTINATION}
  RUNTIME DESTINATION ${CATKIN_PACKAGE_BIN_DESTINATION}
)

