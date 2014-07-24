#include "mqdf.h"

void MQDF::read_dic(std::string dicname) {

	std::cout << "---- Character dictionary infomation ----" << std::endl;
	std::ifstream ifs( dicname.c_str(), std::ios::in | std::ios::binary);
	if (!ifs) {
		std::cerr << "Can'n open mqdf dictionaey file." << dicname << std::endl;
	}
	ifs.read((char*)&dimension, sizeof(unsigned int));
	ifs.read((char*)&num_category, sizeof(unsigned int));
	ifs.read((char*)&power, sizeof(float));
	ifs.read((char*)&max_k, sizeof(unsigned short));

	
	codes.resize(num_category);
	mean_vectors.resize(num_category);
	eigenvalues.resize(num_category);
	eigenvectors.resize(num_category);

	for (unsigned int ci = 0; ci < num_category; ++ci) {
		Eigen::VectorXf Mf(dimension);
		Eigen::VectorXf Df(dimension);
		Eigen::MatrixXf Vf(dimension,dimension);
		ifs.read((char*)&codes[ci], sizeof(unsigned short));
		ifs.read((char*) Mf.data(), sizeof(float) * dimension);
		ifs.read((char*) Df.data(), sizeof(float) * dimension);
		ifs.read((char*) Vf.data(), sizeof(float) * dimension * max_k);

		// convert float to double
		mean_vectors[ci] = Mf.cast<double>();
		eigenvalues[ci]  = Df.cast<double>();
		eigenvectors[ci] = Vf.cast<double>();
	}

	// calculate sigma
	for(unsigned int ci = 0; ci < num_category; ci++){
		sigma += eigenvalues[ci].mean();
	}
	sigma /= num_category;

	std::cout << "Dimension : " << dimension << std::endl;
	std::cout << "Category  : " << num_category << std::endl;
	std::cout << "Power v   : " << power << std::endl;
	std::cout << "Max_k     : " << max_k << std::endl;
	std::cout << "Sigma : " << sigma << std::endl;
	use_k = 80;
	alpha = 0.1;

}

unsigned short MQDF::classfy(Eigen::VectorXd X) {
	
	X = X.array().pow(power);

	std::vector<double> func_value(num_category);

	for (unsigned int ci = 0; ci < num_category; ++ci) {
		double first_term = (X - mean_vectors[ci]).squaredNorm();
		// 第2項の計算
		double second_term = 0;
		double third_term = 0;
		for (unsigned int i = 0; i < use_k; i++){
			second_term += (((1 - alpha) * eigenvalues[ci].coeff(i)) / 
					((1 - alpha) * eigenvalues[ci].coeff(i) + alpha * sigma)) *
					(eigenvectors[ci].col(i).transpose() * (X - mean_vectors[ci])) * 
					(eigenvectors[ci].col(i).transpose() * (X - mean_vectors[ci]));


			third_term += log((1 - alpha) * eigenvalues[ci].coeff(i) + alpha * sigma);

		}
		func_value[ci] =  (1.0 / (alpha * sigma)) * (first_term - second_term) + third_term;
	} 
	return std::distance(func_value.begin(), std::min_element(func_value.begin(),func_value.end()));
}

