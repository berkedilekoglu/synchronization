#include "common.h"

using namespace cv;
using namespace std;

SapientTime::SapientTime(){
	//initialise program timestamp
	now = boost::posix_time::microsec_clock::universal_time();
	instanceTimestamp = to_iso_extended_string(now);
	instanceTimestamp.erase(instanceTimestamp.size()-7);
	instanceTimestamp.replace(13,1,"."); //(position size)
	instanceTimestamp.replace(16,1,"."); //(position size)
}

string SapientTime::Now(){
	now = boost::posix_time::microsec_clock::universal_time();
	std::string timeUTC = to_iso_extended_string(now);
	timeUTC.erase(timeUTC.size()-3);
	timeUTC += "Z";
	return timeUTC;
}
