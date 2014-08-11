#include <vector>
#include <algorithm>
#include <opencv2/opencv.hpp>
#include "blob.h"
class WordCluster { 
	class ClusterNum
	{
	public:
		ClusterNum():cluster_num(0),cluster_max(20) {};
		int cluster_num;
		const int cluster_max;
		void onTrackbar_impl(int val) {};
		static void onTrackbar(int val, void* data);
	};
public:
	WordCluster() : num_cluster(3) {
		ClusterNum c;
//		cv::createTrackbar("Cluster", "Capture", &num_cluster, 20, NULL, &c);

	};
	void cluster(std::vector<Blob> &blobs);

	int num_cluster;
};
