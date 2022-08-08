
/* Copyright © 2017-2022 Jordan Irwin (AntumDeluge) <antumdeluge@gmail.com>
 *
 * This file is part of the bin2header project & is distributed under the
 * terms of the MIT/X11 license. See: LICENSE.txt
 */

#include "convert.h"
#include "cxxopts.hpp"
#include "paths.h"

#include <cerrno>
#include <cctype> // toupper
#include <iostream>
#include <sstream>
#include <string>

using namespace std;


const string version = "0.3.0";
const string appname = "Binary to Header";
string executable;


/** Prints app name & version */
void showVersion() {
	cout << "\n" << appname << " version " << version << endl;
#ifdef WIN32
	cout << "Copyright (C) 2017-2022 Jordan Irwin <antumdeluge@gmail.com>" << endl;
#else
	cout << "Copyright © 2017-2022 Jordan Irwin <antumdeluge@gmail.com>" << endl;
#endif
}


/** Prints usage info */
void showUsage() {
	showVersion();
	cout << "\n  Usage:\n\t" << executable << " [options] <file>" << endl;
	cout << "\n  Options:" << endl;
	cout << "\t-h, --help\t\tPrint help information & exit." << endl;
	cout << "\t-v, --version\t\tPrint version information & exit." << endl;
	cout << "\t-o, --output\t\tSet output file name." << endl;
	cout << "\t-n, --hname\t\tSet header name." << endl;
	cout << "\t-s, --chunksize\t\tSet the buffer chunk size (in bytes)." << endl;
	cout << "\t\t\t\t  Default: 1048576 (1 megabyte)" << endl;
	cout << "\t-d, --nbdata\t\tSet number of bytes per line." << endl;
	cout << "\t\t\t\t  Default: 12" << endl;
	cout << "\t-c, --datacontent\tShow data content as comment." << endl;
	cout << "\t-f  --offset\t\tStart reading at given offset from file beginning." << endl;
	cout << "\t-l  --length\t\tNumber of bytes to process." << endl;
	cout << "\t-p  --pack\t\tStore data using 8, 16, or 32 bit integer types." << endl;
	cout << "\t\t\t\t  Default: 8" << endl;
	cout << "\t-e  --endianess\t\tSwap endianess for 16 or 32 bit integer types." << endl;
	cout << "\t    --stdvector\t\tAdditionally store data in std::vector for C++." << endl;
	cout << "\t    --eol\t\tSet end of line character (cr, lf, or crlf)." << endl;
	cout << "\t\t\t\t  Default: lf" << endl;
}


/** Prints message to stderr & exits program.
 *
 *  @tparam int code
 *      Program exit code.
 *  @tparam string msg
 *      Message to print.
 *  @tparam bool show_usage
 *      If `true` will print usage info.
 */
void exitWithError(const int code, const string msg, const bool show_usage) {
	cerr << "\nERROR: " << msg << endl;
	if (show_usage) showUsage();
	exit(code);
}

/** Prints message to stderr & exits program.
 *
 *  @tparam int code
 *      Program exit code.
 *  @tparam string msg
 *      Message to print.
 */
void exitWithError(const int code, const string msg) {
	exitWithError(code, msg, false);
}


int main(int argc, char** argv) {
	// get executable name
	executable = GetBaseName(NormalizePath(argv[0]));

	// remove extension from executable name
	const unsigned int ext_idx = executable.find_last_of(".");
	executable = executable.substr(0, ext_idx);

	cxxopts::Options options(executable, "Convert binary files to C/C++ headers");
	options.add_options()
			("h,help", "")
			("v,version", "")
			("o,output", "", cxxopts::value<string>())
			("n,hname", "", cxxopts::value<string>())
			("s,chunksize", "", cxxopts::value<unsigned int>())
			("d,nbdata", "", cxxopts::value<unsigned int>())
			("c,datacontent", "")
			("f,offset", "", cxxopts::value<unsigned long>())
			("l,length", "", cxxopts::value<unsigned long>())
			("p,pack", "", cxxopts::value<unsigned int>())
			("e,swap", "")
			("stdvector", "")
			("eol", "", cxxopts::value<string>());

	cxxopts::ParseResult args;
	try {
		args = options.parse(argc, argv);
	} catch (const cxxopts::OptionParseException& e) {
		exitWithError(1, e.what(), true);
	}


	if (args["help"].as<bool>()) {
		showUsage();
		return 0;
	} else if (args["version"].as<bool>()) {
		showVersion();
		return 0;
	}

	// string stream for concatenating error messages
	stringstream ss;

	if (args.count("chunksize") > 0) {
		setChunkSize(args["chunksize"].as<unsigned int>());
	}

	if (args.count("nbdata") > 0) {
		setNumberDataPerLine(args["nbdata"].as<unsigned int>());
	}

	if (args["datacontent"].as<bool>()) {
		setShowDataContent(true);
	}

	if (args.count("offset") > 0) {
		setReadOffset(args["offset"].as<unsigned long>());
	}

	if (args.count("pack") > 0) {
		setOutputBitLength(args["pack"].as<unsigned int>());
	}

	if (args.count("length") > 0) {
		setReadLength(args["length"].as<unsigned long>());
	}

	if (args.count("swap") > 0) {
		setSwapEndianess();
	}

	if (args.count("eol") > 0) {
		setEol(args["eol"].as<string>());
	}

	// too many input files
	// XXX: should we allow multiple inputs?
	#if 0	/* Multiple switches */
	if (argc > 2) {
		// FIXME: correct error return code?
		exitWithError(1, "Too many input files specified", true);
	}
	#endif

	if (argc < 2) {
		// FIXME: correct error return code
		exitWithError(1, "Missing <file> argument", true);
	}

	// only remaining argument should be input file
	string source_file = NormalizePath(argv[argc-1]);

	// Check if file exists
	FILE* test;
	test = fopen(source_file.c_str(), "r");
	fclose(test);

	if (!test) {
		// clear stringstream
		ss.str("");
		ss << "File: \"" << source_file << "\" does not exist";

		// 2 = enoent (file or directory not found)
		exitWithError(ENOENT, ss.str(), true);
	}

	/* Get filenames and target directory */
	string basename = GetBaseName(source_file);
	string hname = basename;

	if (args.count("hname") > 0) {
		hname = args["hname"].as<string>();
	}

	/* START Remove Unwanted Characters */
	char badchars[6] = {'\\', '+', '-', '*', ' '};
	int x;
	for (int current = 0; current < hname.length(); current++) {
		for (x = 0; x < len(badchars); x++) {
			if ((hname[current] == badchars[x]) || (hname[current] == '.'))
				hname.replace(current, 1, "_");
			if (basename[current] == badchars[x])
				basename.replace(current, 1, "_");
		}
	}
	/* END Remove Unwanted Characters */

	/* Add '_' when first char is a number */
	if (isdigit(hname[0])){
		hname.insert(0, 1, '_');
	}

	string target_file;
	if (args.count("output") > 0) {
		target_file = args["output"].as<string>();
	} else {
		const string target_dir = GetDirName(source_file);
		target_file = JoinPath(target_dir, basename).append(".h");
	}

	// set SIGINT (Ctrl+C) handler
	signal(SIGINT, sigintHandler);

	int ret = convert(source_file, target_file, hname, args["stdvector"].as<bool>());

	if (ret == 0)
		cout << "Exported to: " << target_file << endl;
	else
		cout << "An error occured. Error code: " << ret << endl;

	return ret;
}
