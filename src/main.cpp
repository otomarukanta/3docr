#include <iostream>
#include <sstream>
#include <string>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#include "blob.h"
#include "blob_extracter.h"
#include "character_recognizer.h"
#include "angle_estimater.h"
#include "word_cluster.h"
using namespace std;

int main(int argc, char *argv[]) {
	const unsigned int WIDTH = 960;
	const unsigned int HEIGHT = 720;
	
	cv::VideoCapture cap(0);

	cap.set(CV_CAP_PROP_FRAME_WIDTH, WIDTH);
	cap.set(CV_CAP_PROP_FRAME_HEIGHT, HEIGHT);

	if(!cap.isOpened()) {
		cerr << "Can't open camera." << endl;
		return -1;
	}

	cv::namedWindow("Capture", CV_WINDOW_AUTOSIZE|CV_WINDOW_FREERATIO);
	int64 start_time = cv::getTickCount();
	unsigned int fps_cnt = 0;
	string fps_str = "0";

	BlobExtracter blob_extracter(WIDTH, HEIGHT);
	CharacterRecognizer character_recognizer("dic/chara.dic");
	AngleEstimater angle_estimater("dic/angle/", character_recognizer);
	WordCluster word_cluster;
	vector<Blob> blobs(0);

	std::cout << "Start capturing." << endl;
	while(1) {
		double f = 1000.0 / cv::getTickFrequency();
		int64 time = cv::getTickCount();  
		
		// capture image
		cv::Mat frame;
		cap >> frame;

		// extract blob from captured image.
		blob_extracter.extract(frame, blobs);
		for (auto it = blobs.begin(); it != blobs.end(); ++it) {
			if (it->sum >= 50) {
				cv::rectangle(frame, it->pt1, it->pt2, cv::Scalar(0,0,200), 2, 4);
			}
		}

		// recognize character using mqdf
		character_recognizer.recognize(blobs);
		for (auto it = blobs.begin(); it != blobs.end(); ++it) {
			cv::putText(frame, it->character, it->pt1 , cv::FONT_HERSHEY_SIMPLEX, 1.2, cv::Scalar(0,200,0), 2, CV_AA);
		}

		// estimate rotation angle of characters
		angle_estimater.estimate(blobs);
		for (auto it = blobs.begin(); it != blobs.end(); ++it) {
            cv::Point_<unsigned int> xy(it->normal_vector.coeff(0)*100,it->normal_vector.coeff(1)*100);
			cv::line(frame, it->center -xy, it->center + xy, cv::Scalar(200,0,0),2,8);
		}

		// clustering blobs
		word_cluster.cluster(blobs);
 		for (auto it = blobs.begin(); it != blobs.end(); ++it) {
			ostringstream os;
			os << it->cluster;
			cv::putText(frame, os.str(), it->pt2, cv::FONT_HERSHEY_SIMPLEX, 1.2, cv::Scalar(0,200,0), 2, CV_AA);
		}
    	// display fps 
		if ((cv::getTickCount() - start_time)*f >= 1000) {
			ostringstream os;
			os << "fps : " << fps_cnt;
			fps_str = os.str();
			start_time = cv::getTickCount();
			cout << fps_str << endl;
			fps_cnt = 0;
		}else{
			fps_cnt++;
		}

		cv::putText(frame, fps_str, cv::Point(0, 30), cv::FONT_HERSHEY_SIMPLEX, 1.2, cv::Scalar(0,200,0), 2, CV_AA);
		cv::imshow("Capture", frame); // 
		if(cv::waitKey(10) >= 0) 
		{
			break;
		}
	}
}

