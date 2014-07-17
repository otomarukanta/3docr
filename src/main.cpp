#include <iostream>
#include <sstream>
#include <string>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#include "blob.h"
#include "blob_extracter.h"
#include "character_recognizer.h"
using namespace std;

int main(int argc, char *argv[]) {
	const unsigned int WIDTH = 960;
	const unsigned int HEIGHT = 720;
	
	cv::VideoCapture cap(1);

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
	vector<Blob> blobs;

	while(1) {
		double f = 1000.0 / cv::getTickFrequency();
		int64 time = cv::getTickCount();  
		
		// capture image
		cv::Mat frame;
		cap >> frame;

		// extract blob from captured image.
		blob_extracter.extract(frame, blobs);
		for (vector<Blob>::iterator it = blobs.begin(); it != blobs.end();
				++it) {
			if (it->sum >= 50) {
				cv::rectangle(frame, it->pt1, it->pt2, cv::Scalar(0,0,200), 2, 4);
			}
		}

		// recognize character using mqdf
		character_recognizer.recognize(blobs);

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

		// display fps 
		cv::putText(frame, fps_str, cv::Point(0, 30), cv::FONT_HERSHEY_SIMPLEX, 1.2, cv::Scalar(0,200,0), 2, CV_AA);
		cv::imshow("Capture", frame); // 表示
		if(cv::waitKey(10) >= 0) 
		{
			break;
		}
	}
}

