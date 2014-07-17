#include "character_recognizer.h"

CharacterRecognizer::CharacterRecognizer(std::string dicname) {
	mqdf.read_dic(dicname);
}

void CharacterRecognizer::recognize(std::vector<Blob> blobs) {
}
