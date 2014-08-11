#include "character_recognizer.h"

CharacterRecognizer::CharacterRecognizer(std::string dicname) : codes(0), feature_extracter(7, 7){
	mqdf.read_dic(dicname);
	codes.resize(mqdf.codes.size());
	std::copy(mqdf.codes.begin(), mqdf.codes.end(), codes.begin());
	num_category = mqdf.getNumCategory();
}

void CharacterRecognizer::recognize(std::vector<Blob> &blobs) {
	for (auto it = blobs.begin(); it != blobs.end(); ++it){
// 		std::cout << it->cluster << " " << blobs.size() << std::endl;
		// extract feature vector
		Eigen::VectorXd feature_vector(392);  //!MN
		feature_extracter.extract(it->image, feature_vector);

		unsigned short category = mqdf.classfy(feature_vector);
		if (category == num_category) {
			it = blobs.erase(it);
// 			if (it != blobs.begin()) --it;
			--it;
			if (blobs.size() == 0) break;
			continue;
		}
		it->category = category;
		it->character = category2string(category);
	}
// 	std::cout << blobs.size() <<std::endl;
	
}

char CharacterRecognizer::category2string(unsigned int category) {
	unsigned int code = codes[category];
	char str = '@';
	if ( code >= 0x824f && code <= 0x8258) {
		str = '0' + code - 0x824f;
	}else if (code >= 0x8260 && code <= 0x8279) {
		str = 'A' + code - 0x8260;
	}else if (code >= 0x8281 && code <= 0x829a) {
		str = 'a' + code - 0x8281;
	}
	return str;
}

