#include <iostream>
#include "labeling.h"


const unsigned int BACKGROUND = 0;

inline void Labeling::resolve(unsigned int u, unsigned int v){
	u = rl_table[u];
	v = rl_table[v];
	if (u > v){
		unsigned int t = u;
		u = v;
	    v = t;
	}

	next_label[tail_label[u]] = v;
	tail_label[u] = tail_label[v];

	int i = v;
	while(i != -1){
		rl_table[i] = u;
		i = next_label[i];
	}
}
 
unsigned int Labeling::exec(const cv::Mat &src_img, unsigned int *pixels) {

	unsigned int w = width;
	unsigned int h = height;
	for (unsigned int p = 0; p < w * h; ++p) {
		pixels[p] = static_cast<int>(src_img.data[p]);
	}
	for (unsigned int p = 0; p < w * h/4; ++p) {
		rl_table[p] = 0;
		next_label[p] = 0;
		tail_label[p]=0;
	}
	unsigned int m = 1;
	
	for(unsigned int y = 0; y < h; y++){
		for(unsigned int x = 0; x < w; x++){
			if(pixels[y*w + x] != BACKGROUND){
				if(y != 0 && pixels[(y-1)*w + x] != BACKGROUND){ // v3
					pixels[y*w + x] = pixels[(y-1)*w + x];
				}else if( x != 0 && pixels[y*w + x-1] != BACKGROUND){ // v1
					pixels[y*w + x] = pixels[y*w + x-1];
					if( y != 0 && x != w-1 && 
						pixels[(y-1)*w + x+1] != BACKGROUND &&
						rl_table[pixels[y*w + x-1]] != 
						rl_table[pixels[(y-1)*w + x+1]]){

						resolve(pixels[y*w + x-1], pixels[(y-1)*w + x+1]); // v1 v4
					}
				}else if(y != 0 && x != 0 && pixels[(y-1)*w + x-1] != BACKGROUND){ // v2
					pixels[y*w + x] = pixels[(y-1)*w + x-1];
					if( y != 0 && x != w-1 && pixels[(y-1)*w + x+1] != BACKGROUND &&
							rl_table[pixels[(y-1)*w + x-1]] != rl_table[pixels[(y-1)*w+x+1]]){
						resolve(pixels[(y-1)*w + x-1], pixels[(y-1)*w + x+1]); // v2 v4
					}
				}else if( y != 0 && x != w-1 && pixels[(y-1)*w + x+1] != BACKGROUND){ // v4
					pixels[y*w + x] = pixels[(y-1)*w + x+1];
				}else{
					pixels[y*w + x] = m;
					// T[m] = m
					rl_table[m] = m;
					// S(m) = {m}
					next_label[m] = -1;
					tail_label[m] = m;
					m = m + 1;
				}
			}
		}
	}

	// seconde scan
	rl_table[BACKGROUND] = 0;

	for(unsigned int y = 0; y < h; y++){
		for(unsigned int x = 0; x < w; x++){
			pixels[y*w + x] = rl_table[pixels[y*w + x]];
		}
	}
	//for(int i = 1; i < 100; i++) cout << rl_table[i] << endl;
	return m;
 
}
