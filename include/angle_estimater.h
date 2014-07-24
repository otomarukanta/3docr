#include <string>
#include <iomanip>
#include <sstream>
#include <vector>
#include <cmath>
#include "blob.h"
#include "ldf.h"
#include "feature_extracter.h"
#include "character_recognizer.h"

class AngleEstimater {
public:
	AngleEstimater(std::string dicname, CharacterRecognizer cr);
	void estimate(std::vector<Blob> &blobs);
private:
	Eigen::Matrix<short, 3, 1> category2angle(unsigned int category);

	FeatureExtracter feature_extracter;
	std::vector<LDF> ldfs;
};
