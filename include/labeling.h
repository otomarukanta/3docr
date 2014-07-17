#include <opencv2/core/core.hpp>

class Labeling {
public:
	Labeling(){};
	Labeling(unsigned int w, unsigned int h) : width(w), height(h) {
		rl_table = new unsigned int[w*h/4];
		next_label = new int[w*h/4];
		tail_label = new unsigned int[w*h/4];
	}
 
	unsigned int exec(const cv::Mat &src_img, unsigned int *pixels) ;
private:
	inline void resolve(unsigned int u, unsigned int v);
	unsigned int width;
	unsigned int height;
	unsigned int *rl_table;
	int *next_label;
	unsigned int *tail_label;
};
