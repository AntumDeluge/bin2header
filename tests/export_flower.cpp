
#include "flower.png.h"

#include <fstream>

using namespace std;


int main(int argc, char** argv) {
	// export copy of flower.png

	ofstream ofs;
	ofs.open("flower_copy.png", ofstream::binary);

	if (!ofs.is_open()) exit(1);

	for (int byte_idx = 0; byte_idx < sizeof(flower_png); byte_idx++) {
		ofs << flower_png[byte_idx];
	}

	ofs.close();

	return 0;
}
