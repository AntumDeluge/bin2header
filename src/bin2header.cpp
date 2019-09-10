
#include "convert.h"
#include "paths.h"

#include <cctype> // toupper
#include <iostream>
#include <string>

using namespace std;


const string version = "0.1.3";


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
	string basename = GetBaseName(source_file);
	string hname = basename;
	const string target_dir = GetDirName(source_file);

	/* START Remove Unwanted Characters */
	char badchars[6] = {'\\', '+', '-', '*', ' '};
	int x;
	for (int current = 0; current < hname.length(); current++) {
		for (x = 0; x < len(badchars); x++) {
			if ((hname[current] == badchars[x]) or (hname[current] == '.'))
				hname.replace(current, 1, "_");
			if (basename[current] == badchars[x])
				basename.replace(current, 1, "_");
		}
	}
	/* END Remove Unwanted Characters */

	const string target_file = JoinPath(target_dir, basename).append(".h");

	int ret = convert(source_file, target_file, hname);

	if (ret == 0)
		cout << "Exported to: " << target_file << endl;
	else
		cout << "An error occured. Error code: " << ret << endl;

	return ret;
}
