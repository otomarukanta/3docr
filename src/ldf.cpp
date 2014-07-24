#include "ldf.h"

void LDF::read_dic(std::string dicname) {
 
	std::cout << "---- LDF initinize ----" << std::endl;
	std::ifstream ifs( dicname.c_str(), std::ios::in | std::ios::binary);
	if (!ifs) {
		std::cerr << "Can'n open ldf dictionaey file." << dicname << std::endl;
	}
	ifs.read((char*)&dimension, sizeof(unsigned int));
	ifs.read((char*)&num_category, sizeof(unsigned int));
	ifs.read((char*)&power, sizeof(float));
	
	angles.resize(num_category);
	W.resize(num_category);
	w0.resize(num_category);

	for (unsigned int ci = 0; ci < num_category; ++ci) {
		Eigen::VectorXf Wf(dimension);
		float w0f;
		ifs.read((char*) &angles[ci], sizeof(short) * 3);
		ifs.read((char*) Wf.data(), sizeof(float) * dimension);
		ifs.read((char*) &w0f, sizeof(w0f));

		// convert float to double
		W[ci] = Wf.cast<double>();
		w0[ci] = static_cast<double>(w0f);
	}

	std::cout << "Dimension : " << dimension << std::endl;
	std::cout << "Category  : " << num_category << std::endl;
	std::cout << "Power v   : " << power << std::endl;
} 

unsigned int LDF::classfy(Eigen::VectorXd X) {
	
	X = X.array().pow(power);
	std::vector<double> func_value(num_category);

	for (unsigned int ci = 0; ci < num_category; ++ci) {
		func_value[ci] = W[ci].transpose() * X + w0[ci];
	}
	return std::distance(func_value.begin(), std::min_element(func_value.begin(), func_value.end()));
}
