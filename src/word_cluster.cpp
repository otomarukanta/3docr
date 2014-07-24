#include "word_cluster.h"

void WordCluster::cluster(std::vector<Blob> &blobs) {
	std::vector< std::vector<double> > distance_matrix 
		(blobs.size(), std::vector<double>(blobs.size()));
	for(unsigned int i = 0; i < blobs.size()-1; ++i) {
		for(unsigned int j = i+1; j < blobs.size(); ++j) {
			cv::Point p = blobs[i].center - blobs[j].center;
			double dist = p.ddot(p);
			distance_matrix[i][j] = dist / p.x;
		}
	}

	std::vector<double> dists(blobs.size()-1);
	for (unsigned int k = 0; k < blobs.size()-1; ++k) {
		unsigned int min = 999999;
		unsigned int min_i,min_j;
		for (unsigned int i = 0; i < blobs.size()-1; ++i) {
			for (unsigned int j = i+1; j < blobs.size(); ++j) {
				if ( blobs[i].cluster == blobs[j].cluster) continue;
				if ( min > distance_matrix[i][j]) {
					min = distance_matrix[i][j];
					min_i=i;
					min_j=j;
				}
			}
		}
		for (unsigned int i = 0; i < blobs.size(); ++i) {
			if(blobs[i].cluster == blobs[min_i].cluster) {
				blobs[i].cluster = blobs[min_j].cluster;
			}
		}
		dists[k] = min;
	}

	std::vector<double> h(blobs.size());
	for (unsigned int i = 1; i < blobs.size(); ++i) {
		double m1 = 0;
		double m2 = 0;
		for (unsigned int j = 0; j < i; ++j) {
			m1 += dists[j];
		}
		m1 /= i;
		for (unsigned int j = i; j < blobs.size(); ++j) {
			m2 += dists[j];
		}
		m2 /= blobs.size()-i;
		h[i] = i*(blobs.size()-i)*(m1-m2)*(m1-m2);
		std::cout << i << " " << h[i] << std::endl;
	}
 	unsigned int max_h = std::distance(h.begin(), max_element(h.begin(),h.end()));
	std::cout << max_h << std::endl;
	unsigned int ci = 0;
	for (auto iter = blobs.begin(); iter != blobs.end(); ++iter) {
		iter->cluster = ci;
		++ci;
	}
	for (unsigned int k = 0; k < max_h; ++k) {
		unsigned int min = 999999;
		unsigned int min_i,min_j;
		for (unsigned int i = 0; i < blobs.size()-1; ++i) {
			for (unsigned int j = i+1; j < blobs.size(); ++j) {
				if ( blobs[i].cluster == blobs[j].cluster) continue;
				if ( min > distance_matrix[i][j]) {
					min = distance_matrix[i][j];
					min_i=i;
					min_j=j;
				}
			}
		}
		for (unsigned int i = 0; i < blobs.size(); ++i) {
			if(blobs[i].cluster == blobs[min_i].cluster) {
				blobs[i].cluster = blobs[min_j].cluster;
			}
		}
		dists[k] = min;
	}
}
