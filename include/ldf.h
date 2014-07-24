#include <string>
#include <iostream>
#include <fstream>
#include <vector>

#include <Eigen/Core>
class LDF {
public:
	LDF() : angles(0),dimension(0),num_category(0),power(0), W(0), w0(0){};
	void read_dic(std::string dicname);
	unsigned int classfy(Eigen::VectorXd X);
	std::vector<Eigen::Matrix<short, 3, 1> > angles;
	unsigned int dimension;
private:
	unsigned int num_category;
	float power;
	
	std::vector<Eigen::VectorXd> W;
	std::vector<double> w0;
}; 
