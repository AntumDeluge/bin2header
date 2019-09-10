
#include "paths.h"

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <iomanip>
using namespace std;

#include <cstdio>


const string version = "0.1.2";

#define len(a) (sizeof(a)/sizeof(*a))


int PrintUsage(string exe="bin2header") {
	cout << "\nbin2header version " << version << endl;
	cout << "2019 Jordan Irwin <antumdeluge@gmail.com>\n\n\tUsage:\t" << exe << " <file>\n" << endl;
	return 1;
}


int main(int argc, char** argv) {
	string executable = NormalizePath(argv[0]);
	executable = GetBaseName(executable);

	if (argc < 2) {
		cout << "\nERROR: Missing <file> argument\n";
		return PrintUsage(executable);
	}

	string source_file = NormalizePath(argv[1]);

	// Check if file exists
	FILE* test;
	test = fopen(source_file.c_str(), "r");
	fclose(test);

	if (!test) {
		cout << "\nFile: \"" << source_file << "\" does not exist\n";
		return PrintUsage(executable);
	}

	/* Get filenames and target directory */
	string filename = GetBaseName(source_file);
	string hname = filename;
	const string target_dir = GetDirName(source_file);

	/* START Remove Unwanted Characters */
	int hname_len = hname.length();
	char badchars[6] = {'\\', '+', '-', '*', ' '};
	int current;
	int x;
	for (current = 0; current < hname_len; current++) {
		for (x = 0; x < len(badchars); x++) {
			if ((hname[current] == badchars[x]) or (hname[current] == '.'))
				hname.replace(current, 1, "_");
			if (filename[current] == badchars[x])
				filename.replace(current, 1, "_");
		}
	}
	/* END Remove Unwanted Characters */

	const string target_file = JoinPath(target_dir, filename).append(".h");

	/* START Uppercase Name for Header */
	char hname_upper[hname_len + 2];
	for (current = 0; current < len(hname_upper); current++) {
		hname_upper[current] = hname[current];
		hname_upper[current] = toupper(hname_upper[current]);
	}

	string name_upper_h = hname_upper;
	name_upper_h.append("_H");
	/* END Uppercase Name for Header */

	/* START Read Data In */
	ifstream infile;
	infile.open(source_file.c_str(), ifstream::binary);

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

	ofstream outfile(target_file.c_str(), ofstream::binary); // currently only support LF line endings output
	outfile << "#ifndef " << name_upper_h.c_str() << "\n#define " << name_upper_h.c_str() << "\n";
	if (store_vector) {
		outfile << "\n#ifdef __cplusplus\n#include <vector>\n#endif\n";
	}
	outfile << "\nstatic const unsigned char " << hname << "[] = {\n";

	for (current = 0; current < data_length; current++) {
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
	outfile << "\n#endif /* " << name_upper_h << " */\n";

	outfile.close();

	/* END Read Data Out to Header */

	cout << "Exported to: " << target_file << endl;

	return 0;
}
