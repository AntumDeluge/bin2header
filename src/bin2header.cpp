
#include "convert.h"
#include "cxxopts.hpp"
#include "paths.h"

#include <cctype> // toupper
#include <iostream>
#include <sstream>
#include <string>

using namespace std;


const string version = "0.1.3";
const string appname = "Binary to Header";
string executable;


/** Prints app name & version */
void showVersion() {
	cout << "\n" << appname << " version " << version << endl;
	cout << "2019 Jordan Irwin <antumdeluge@gmail.com>" << endl;
}


/** Prints usage info */
void showUsage() {
	showVersion();
	cout << "\n  Usage:\n\t" << executable << " [options] <file>" << endl;
	cout << "\n  Options:" << endl;
	cout << "\t-h|--help\tPrint help information & exit." << endl;
	cout << "\t-v|--version\tPrint version information & exit." << endl;
}


/** Prints a message to stderr & exits program.
 *
 * @tparam string msg Message to print.
 * @tparam int ecode Error code to exit with.
 * @tparam bool show_usage If `true` will print usage info
 */
void exitWithError(const string msg, const int ecode, const bool show_usage) {
	cerr << "\nERROR: " << msg << endl;
	if (show_usage) showUsage();
	exit(errno);
}

/** Overloaded function defaulting to not show usage info */
void exitWithError(const string msg, const int ecode) {
	exitWithError(msg, ecode, false);
}


int main(int argc, char** argv) {
	// get executable name
	executable = GetBaseName(NormalizePath(argv[0]));

	// remove extension from executable name
	const unsigned int ext_idx = executable.find_last_of(".");
	executable = executable.substr(0, ext_idx);

	cxxopts::Options options(executable, "help_string");
	options.add_options()
			("h,help", "help")
			("v,version", "version");

	auto args = options.parse(argc, argv);

	if (args["help"].as<bool>()) {
		showUsage();
		return 0;
	} else if (args["version"].as<bool>()) {
		showVersion();
		return 0;
	}

	// too many input files
	// XXX: should we allow multiple inputs?
	if (argc > 2) {
		// FIXME: correct error return code?
		exitWithError("Too many input files specified", 1, true);
	}

	if (argc < 2) {
		// FIXME: correct error return code
		exitWithError("Missing <file> argument", 1, true);
	}

	// only remaining argument should be input file
	string source_file = NormalizePath(argv[1]);

	// Check if file exists
	FILE* test;
	test = fopen(source_file.c_str(), "r");
	fclose(test);

	if (!test) {
		stringstream ss;
		ss << "File: \"" << source_file << "\" does not exist";

		// FIXME: correct error return code?
		exitWithError(ss.str(), 1, true);
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

	// set SIGINT (Ctrl+C) handler
	signal(SIGINT, sigintHandler);

	int ret = convert(source_file, target_file, hname);

	if (ret == 0)
		cout << "Exported to: " << target_file << endl;
	else
		cout << "An error occured. Error code: " << ret << endl;

	return ret;
}
