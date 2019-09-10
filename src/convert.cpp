
/* Copyright © 2019 Jordan Irwin (AntumDeluge) <antumdeluge@gmail.com>
 *
 * This file is part of the bin2header project & is distributed under the
 * terms of the MIT/X11 license. See: LICENSE.txt
 */

#include "convert.h"

#include <fstream>
#include <sstream>
#include <iomanip>

using namespace std;


unsigned int chunk_size = 64;

void setChunkSize(const unsigned int sz) { chunk_size = sz; }


int convert(const string fin, const string fout, const string hname) {
	// TODO: read/write in chunks
	//char chunk[chunk_size];

	/* START Read Data In */
	ifstream infile;
	infile.open(fin.c_str(), ifstream::binary);

	int data_length;
	infile.seekg(0, ifstream::end);
	data_length = infile.tellg();
	infile.seekg(0, ifstream::beg);

	char data[data_length];

	infile.read(data, data_length);
	infile.close();
	/* END Read Data In */

	/* START Read Data Out to Header */

	// adds C++ std::vector support
	// TODO: make optional
	bool store_vector = true;

	ofstream outfile(fout.c_str(), ofstream::binary); // currently only support LF line endings output
	outfile << "#ifndef " << hname.c_str() << "\n#define " << hname.c_str() << "\n";
	if (store_vector) {
		outfile << "\n#ifdef __cplusplus\n#include <vector>\n#endif\n";
	}
	outfile << "\nstatic const unsigned char " << hname << "[] = {\n";

	for (int current = 0; current < data_length; current++) {
		if ((current % 12) == 0) outfile << "    ";

		stringstream ss;
		ss << "0x" << hex << setw(2) << setfill('0') << (int) (unsigned char) data[current];
		outfile << ss.str();

		if ((current + 1) < data_length) outfile << ", ";

		if ((current % 12) == 11) outfile << "\n";
	}

	outfile << "\n};\n";
	if (store_vector) {
		outfile << "\n#ifdef __cplusplus\nstatic const std::vector<char> "
				<< hname << "_v(" << hname << ", " << hname << " + sizeof("
				<< hname << "));\n#endif\n";
	}
	outfile << "\n#endif /* " << hname << " */\n";

	outfile.close();

	/* END Read Data Out to Header */

	return 0;
}
