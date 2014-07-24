#include <vector>
#include <algorithm>
#include <opencv2/opencv.hpp>
#include "blob.h"
class WordCluster {
public:
	WordCluster() {};
	void cluster(std::vector<Blob> &blobs);

};
