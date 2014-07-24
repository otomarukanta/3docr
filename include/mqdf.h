#pragma once
#include <string>
#include <iostream>
#include <fstream>
#include <vector>

#include <Eigen/Core>
class MQDF {
public:
	MQDF() : codes(0), mean_vectors(0), eigenvalues(0), eigenvectors(0){};
	void read_dic(std::string dicname);
	unsigned short classfy(Eigen::VectorXd feature_vector);
	unsigned int getNumCategory(void) {return num_category;}
	
	std::vector<unsigned short> codes;

	
private:
	unsigned int dimension;
	unsigned int num_category;
	float power;
	short max_k;
	double alpha;
	unsigned int use_k;

	std::vector<Eigen::VectorXd> mean_vectors;
	std::vector<Eigen::VectorXd> eigenvalues;
	std::vector<Eigen::MatrixXd> eigenvectors;

	double sigma;
};
