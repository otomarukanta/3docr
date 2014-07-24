#include "angle_estimater.h"

AngleEstimater::AngleEstimater(std::string dicdir, CharacterRecognizer cr) : feature_extracter(2,2), ldfs(63) {
	ldfs.resize(cr.num_category);
	std::cout << ldfs.size() << std::endl;
	int ci = 0;
	for (auto it = ldfs.begin(); it != ldfs.end(); ++it) {
		std::ostringstream os;
		os << dicdir <<  std::hex << cr.getCode(ci) << ".dic";
		it->read_dic(os.str());
		std::cout << os.str() << std::endl;
		ci++;
	}
}

void AngleEstimater::estimate(std::vector<Blob> &blobs) {
	for (auto it = blobs.begin(); it != blobs.end(); ++it){
		// extract feature vector
 		Eigen::VectorXd feature_vector(32);  //!MN
 		feature_extracter.extract(it->image, feature_vector);

		unsigned int category = ldfs[it->category].classfy(feature_vector);
		it->angle = ldfs[it->category].angles[category];
		double x = it->angle.coeff(0) * M_PI / 180.0;
		double y = it->angle.coeff(1) * M_PI / 180.0;
		double z = it->angle.coeff(2) * M_PI / 180.0;
		it->normal_vector << cos(x)*sin(y)*cos(z) - sin(x)*sin(z),
							-cos(x)*sin(y)*sin(z) - sin(x)*cos(z),
							-cos(x)*cos(y);
	}
}

Eigen::Matrix<short, 3, 1> AngleEstimater::category2angle(unsigned int category) {
}
 
