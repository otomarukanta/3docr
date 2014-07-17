#include "mqdf.h"

void MQDF::read_dic(std::string dicname) {
	std::ifstream ifs( dicname.c_str(), std::ios::in | std::ios::binary);
	if (!ifs) {
		std::cerr << "Can'n open mqdf dictionaey file." << dicname << std::endl;
	}
	unsigned int dimension;
	ifs.read((char*)&dimension, sizeof(unsigned int));
	std::cout << dimension << std::endl;
}

