#include <ros/ros.h>
#include <image_transport/image_transport.h>
#include <opencv2/highgui/highgui.hpp>
#include <cv_bridge/cv_bridge.h>
#include "std_msgs/String.h"
#include <sstream>
#include "common.h"
#include <sensor_msgs/CameraInfo.h>
#include <sensor_msgs/Image.h>
using namespace std;
using namespace cv;
 
static bool interrupt;
 cv::Mat img;
SapientTime t;
  //Capturing video
  string firstFrameTime = "unknown";
  string machine_name = "ASM";
  VideoCapture capture;
  VideoWriter captureRecord;
  std::string prefix;
  std::string dataDir = "~/catkin_ws1/src/beginner_tutorials/data";
  int closingTime = 20; //After that many empty frames recording will cease.
  int emptyFrameCounter = 0;

  //Timing the loop
  double lastFrameTime = getTickCount(); // time keeping
  int how_much_sleep = 0;
  char key = '0';
  bool keepProcessing = true;
  float fps = 25;
  int frameNo = 0;

 
bool check=true;
sensor_msgs::ImageConstPtr msg=NULL;
sensor_msgs::CameraInfo msginfo;
void chatterCallback(const std_msgs::String::ConstPtr& msg1,image_transport::Publisher *pub,ros::Publisher *pub_cam_info)
{
 
ROS_INFO("I heard: [%s]", msg1->data.c_str());
std::cout << "takeMessage" << endl;

std::cout << "keep Processing" << endl;
    capture >> img;
   
 msg = cv_bridge::CvImage(std_msgs::Header(), "bgr8", img).toImageMsg() ;

  sensor_msgs::Image image1(*msg);
  
    //handle empty frames
    if(img.empty()){
      emptyFrameCounter++;
      if(emptyFrameCounter==closingTime){
        keepProcessing = false;
      }else{
        lastFrameTime = 0;//try to get next frame ASAP
        keepProcessing = true;
      }
    }
if(keepProcessing)
{
    //save time of a first frame
    frameNo++;
    if(frameNo==1){
      firstFrameTime = t.Now();
    }

   // cv::imshow("PUBLISHER IMAGE", img);
    captureRecord.write(img);
std::cout <<  "write video" << endl;
    // compute time to wait

    how_much_sleep = FpsToInterval(fps) - MsTimeSinceLast(lastFrameTime);
 cout << "FPSTOINTERVAL="<<FpsToInterval(fps) << endl;
 cout << "MSTIMESINCELAST"<<MsTimeSinceLast(lastFrameTime)<< endl;
    cout << how_much_sleep << endl;


    lastFrameTime = getTickCount();

    key = cvWaitKey(std::max(2,how_much_sleep));
}
    if (interrupt){
     keepProcessing = false;
    }
if(keepProcessing)
{
msginfo.header.stamp=image1.header.stamp;
     pub->publish(msg);
      pub_cam_info->publish(msginfo);
//msginfo.header.stamp=ros::Time::now();
    
}
}




int main(int argc, char** argv)
{
 lastFrameTime = getTickCount();
  ros::init(argc, argv, "image_publisher1");
////////////////////////////////////////////////////////////////////
  int count = 0;
ros::NodeHandle n;
 ros::NodeHandle nh;
//Initialising variables
 image_transport::ImageTransport it(n);
  image_transport::Publisher pub = it.advertise("image1", 1);

  ros::Publisher pub_cam_info= n.advertise<sensor_msgs::CameraInfo>("info1",1);
      ros::Subscriber sub= n.subscribe<std_msgs::String>("chatter",1,boost::bind(&chatterCallback,_1,&pub,&pub_cam_info));
        std::cout << "Get Message" << endl;
   
  // initialising time
  std::string timestamp;
  interrupt = false;

//signal(SIGINT, closeMe);
  if(argc>1){
    machine_name = argv[1];
  }

  // read this instance timestamp
  timestamp = t.instanceTimestamp;
  string workingDir = dataDir + "/";
  prefix = workingDir + timestamp + machine_name;
  string videoName = prefix + "-input.avi";
  string fileName = prefix + "-timeOfFirstFrame.txt";

  // Open default camera
  capture.open(0);
  capture >> img;

  cout << videoName << endl;

  captureRecord.open("/home/capture/test/test.avi",
                     CV_FOURCC('H', 'F', 'Y', 'U'), (double)fps,
                     //CV_FOURCC('X', 'V', 'I', 'D'), (double)fps,
                     Size(img.cols, img.rows), true);

 // captureRecord.set(CAP_PROP_FPS,1.0);

  // get time now

///////////////////////////////////////////////////////////////////
  
  
  
        ros::spin();
     
   

  imwrite( "/home/capture/test/berke.jpg", img );
  captureRecord.release();
  capture.release();
  ofstream fileTimeInfo;
  std::cout << "close everything" << "\n";
   // ros::spinOnce();
  //  loop_rate.sleep();
//  }
  

  return 0;
}

