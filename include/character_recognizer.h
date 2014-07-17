#include <string>
#include <vector>
#include "blob.h"
#include "mqdf.h"

class CharacterRecognizer {
public:
	CharacterRecognizer(std::string dicname);
	void recognize(std::vector<Blob> blobs);
private:
	MQDF mqdf;
};
