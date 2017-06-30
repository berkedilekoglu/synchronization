#include <ros/ros.h>
#include <image_transport/image_transport.h>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/core/types.hpp>
#include <message_filters/subscriber.h>
#include <message_filters/synchronizer.h>
#include <message_filters/time_synchronizer.h>
#include <message_filters/sync_policies/approximate_time.h>
#include <sensor_msgs/Image.h>
#include <sensor_msgs/image_encodings.h>
#include <cv_bridge/cv_bridge.h>
#include "common.h"
#include <sensor_msgs/CameraInfo.h>
#include "std_msgs/String.h"
#include <sstream>
using namespace sensor_msgs;
using namespace message_filters;
cv::Mat img3;
cv::Mat img4;
//typedef sync_policies::ApproximateTime<sensor_msgs::Image,sensor_msgs::CameraInfo,sensor_msgs::Image,sensor_msgs::CameraInfo> MySyncPolicy;
 
typedef sync_policies::ApproximateTime<sensor_msgs::Image,sensor_msgs::Image> MySyncPolicy;


//cv::VideoWriter captureRecord;
//static const std::string OPENCV_WINDOW = "Image window";
//void callback(const sensor_msgs::ImageConstPtr& msg1, const sensor_msgs::ImageConstPtr& msg2)
//void callback(const sensor_msgs::Image::ConstPtr& msg1,const sensor_msgs::CameraInfoConstPtr& left_camera, const sensor_msgs::Image::ConstPtr& msg2,const sensor_msgs::CameraInfoConstPtr& right_camera)
void callback(const sensor_msgs::Image::ConstPtr& msg1, const sensor_msgs::Image::ConstPtr& msg2)
{

 cv_bridge::CvImagePtr cv_ptr1;
 cv_bridge::CvImagePtr cv_ptr2;
//sensor_msgs::CameraInfo left_camera_(*left_camera);
//sensor_msgs::CameraInfo right_camera_(*right_camera);
  sensor_msgs::Image left_(*msg1);
sensor_msgs::Image right_(*msg2);
std::cout << "keep Processing" << "\n";
//cv_bridge::CvImagePtr cv_ptr2;
  try
  {

    cv_ptr1 = cv_bridge::toCvCopy(msg1, sensor_msgs::image_encodings::BGR8);
cv_ptr2 = cv_bridge::toCvCopy(msg2, sensor_msgs::image_encodings::BGR8);
 //left_camera_.header.stamp = left_.header.stamp;
//right_camera_.header.stamp = left_.header.stamp;
right_.header.stamp = left_.header.stamp;

 // cv::namedWindow("view");
 //cv_ptr2 = cv_bridge::toCvCopy(msg2, sensor_msgs::image_encodings::BGR8);
   // cv::imshow("view1", cv_bridge::toCvShare(msg1, "bgr8")->image);
   // cv::imshow("view2", cv_bridge::toCvShare(msg2, "bgr8")->image);
  }
  catch (cv_bridge::Exception& e)
  {ROS_ERROR("cv_bridge exception: %s", e.what());
    ROS_ERROR("Could not convert from '%s' to 'bgr8'.", msg1->encoding.c_str());
  }


 //if (cv_ptr1->image.rows > 60 && cv_ptr1->image.cols > 60)
     // cv::circle(cv_ptr1->image, cv::Point(50, 50), 10, CV_RGB(255,0,0));
  
    // Update GUI Window
img3=cv_ptr1->image;
img4=cv_ptr2->image;

//captureRecord.write(img3);
std::cout <<  "update" <<  "\n";
   // cv::imshow(OPENCV_WINDOW, cv_ptr1->image);
  //  cv::imshow("COMPUTER1_image", img3);
    //cv::imshow("COMPUTER2_image", img4);
    cv::waitKey(3);

 }


int main(int argc, char **argv)
{
 ros::init(argc, argv, "image_listener");
//captureRecord.open("/home/capture/test/test.avi",CV_FOURCC('H', 'F', 'Y', 'U'), 25.0,Size(img3.cols, img3.rows), true);
 ros::Time::init();
 ros::NodeHandle nh;

 // cv::namedWindow("view");
 // cv::startWindowThread();
  image_transport::ImageTransport it(nh);
   message_filters::Subscriber<sensor_msgs::Image> image1_sub(nh, "image1", 1);
//message_filters::Subscriber<sensor_msgs::CameraInfo> info1_sub(nh, "info1", 1);
  message_filters::Subscriber<sensor_msgs::Image> image2_sub(nh, "image2", 1);
//message_filters::Subscriber<sensor_msgs::CameraInfo> info2_sub(nh, "info2", 1);


  // ApproximateTime takes a queue size as its constructor argument, hence MySyncPolicy(10)
  //Synchronizer<MySyncPolicy> sync(MySyncPolicy(10), image1_sub, image2_sub);
//TimeSynchronizer<Image,CameraInfo, Image,CameraInfo> sync( image1_sub,info1_sub, image2_sub,info2_sub, 1000);
  //message_filters::Synchronizer<MySyncPolicy> sync(MySyncPolicy(1), image1_sub,info1_sub, image2_sub,info2_sub);
//sync.registerCallback(boost::bind(&callback, _1, _2, _3, _4));
message_filters::Synchronizer<MySyncPolicy> sync(MySyncPolicy(1), image1_sub,image2_sub);
sync.registerCallback(boost::bind(&callback, _1, _2));

  ros::spin();
  imwrite( "/home/capture/test/computer1.jpg", img3 );
 imwrite( "/home/capture/test/computer2.jpg", img4 );
//captureRecord.release();
 // cv::destroyWindow("view");

  return 0;
}
