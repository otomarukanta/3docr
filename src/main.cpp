#include <iostream>
#include <sstream>
#include <string>
#include <sys/time.h>

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#include "blob.h"
#include "blob_extracter.h"
#include "character_recognizer.h"
#include "angle_estimater.h"
#include "word_cluster.h"
using namespace std;

double gettimeofday_sec()
{
	struct timeval tv;
	gettimeofday(&tv, NULL);
 	return tv.tv_sec + (double)tv.tv_usec*1e-6;
}

int main(int argc, char *argv[]) {
	const unsigned int WIDTH = 960;
	const unsigned int HEIGHT = 720;
	
	cv::VideoCapture cap(0);

	cap.set(CV_CAP_PROP_FPS, 60);
	cap.set(CV_CAP_PROP_FRAME_WIDTH, WIDTH);
	cap.set(CV_CAP_PROP_FRAME_HEIGHT, HEIGHT);
	double fps = cap.get(CV_CAP_PROP_FPS);
	std::cout << fps << std::endl;
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
		double start_sec,end_sec;
		double f = 1000.0 / cv::getTickFrequency();
		int64 time = cv::getTickCount();  
		
		// capture image
		cv::Mat frame;
		cap >> frame;

		// extract blob from captured image.
		blob_extracter.extract(frame, blobs);
		for (auto it = blobs.begin(); it != blobs.end(); ++it) {
			if (it->sum >= 50) {
				cv::rectangle(frame, it->pt1, it->pt2, cv::Scalar(0,0,200), 1, 4);
			}
		}

		// recognize character using mqdf
// 		std::cout << "recognize character." << std::endl;
		character_recognizer.recognize(blobs);
		for (auto it = blobs.begin(); it != blobs.end(); ++it) {
			cv::putText(frame, it->character, it->pt1 , cv::FONT_HERSHEY_SIMPLEX, 1.2, cv::Scalar(0,200,0), 2, CV_AA);
		}

		// estimate rotation angle of characters
// 		std::cout << "estimate rotation angle." << std::endl;
// 		std::cout << blobs.size() << std::endl;
		angle_estimater.estimate(blobs);
		for (auto it = blobs.begin(); it != blobs.end(); ++it) {
            cv::Point_<unsigned int> xy(it->normal_vector.coeff(0)*100,it->normal_vector.coeff(1)*100);
			cv::line(frame, it->center - xy, it->center + xy, cv::Scalar(200,0,0), 1, 8);
		}

		// clustering blobs
// 		std::cout << "clustering word." << std::endl;
		if( blobs.size() != 0) { 
			word_cluster.cluster(blobs);
		}
		std::vector<cv::RotatedRect> rects(100); // !!!!!!!!!!!!!!todo
		std::vector<std::vector<cv::Point> > points(100);
//  		for (auto it = blobs.begin(); it != blobs.end(); ++it) {
// 			points[it->cluster].push_back(cv::Point(it->pt1.x, it->pt1.y));
// 			points[it->cluster].push_back(cv::Point(it->pt1.x, it->pt2.y));
// 			points[it->cluster].push_back(cv::Point(it->pt2.x, it->pt1.y));
// 			points[it->cluster].push_back(cv::Point(it->pt2.x, it->pt2.y));
// 			ostringstream os;
// 			os << it->cluster;
// 		}    
		int ri = 0;
 		for (auto it = points.begin(); it != points.end(); ++it) {
			if (it->size() != 0) {
				rects[ri] = cv::minAreaRect(*it);
// 				std::cout << it->size() << std::endl;
				cv::Point2f rect_points[4]; rects[ri].points( rect_points);
				for (int j = 0; j < 4; ++j) {
					cv::line( frame, rect_points[j], rect_points[(j+1)%4], cv::Scalar(200,0,0),2,8);
				}
			}
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
		if(cv::waitKey(1) >= 0) 
		{
			break;
		}
	}
}

