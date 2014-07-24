#pragma once
#include <string>
#include <sstream>
#include <vector>
#include "blob.h"
#include "mqdf.h"
#include "feature_extracter.h"

class CharacterRecognizer {
public:
	CharacterRecognizer(std::string dicname);
	void recognize(std::vector<Blob> &blobs);
	char category2string(unsigned int category);
    unsigned int getCode(unsigned int ci) { return codes[ci]; };
	int num_category;
private:

	std::vector<unsigned int> codes;
	FeatureExtracter feature_extracter;
	MQDF mqdf;
};
