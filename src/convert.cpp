
/* Copyright © 2019 Jordan Irwin (AntumDeluge) <antumdeluge@gmail.com>
 *
 * This file is part of the bin2header project & is distributed under the
 * terms of the MIT/X11 license. See: LICENSE.txt
 */

#include "convert.h"

#include <cmath> // ceil
#include <fstream>
#include <sstream>
#include <iomanip>
#include <iostream>

using namespace std;


unsigned int chunk_size = 1024 * 1024; // default 1MB

bool cancelled = false;


void setChunkSize(const unsigned int sz) { chunk_size = sz; }

// Cancels current write iteration.
void sigintHandler(int sig_num) {
	cancelled = true;
	// reset handler to catch SIGINT next time
	signal(SIGINT, sigintHandler);
}

int convert(const string fin, const string fout, string hname, const bool store_vector) {
	// file streams
	ifstream ifs;
	ofstream ofs;

	/* Add '_' when first char is a number */
	if (isdigit(hname[0]))
	{
		hname.insert(0, 1, '_');
	}

	/* START Uppercase Name for Header */

	char hname_upper[hname.length() + 2];
	for (int current = 0; current < len(hname_upper); current++) {
		hname_upper[current] = hname[current];
		hname_upper[current] = toupper(hname_upper[current]);
	}

	string name_upper_h = hname_upper;
	name_upper_h.append("_H");

	try {
		/* START Read Data In */

		ifs.open(fin.c_str(), ifstream::binary);

		unsigned long long data_length;
		ifs.seekg(0, ifstream::end);
		data_length = ifs.tellg();
		ifs.seekg(0, ifstream::beg);

		unsigned long long chunk_count = ceil((double) data_length / chunk_size);

		cout << "File size: " << to_string(data_length) << " bytes" << endl;
		cout << "Chunk size: " << to_string(chunk_size) << " bytes" << endl;

		/* START Read Data Out to Header */

		ofs.open(fout.c_str(), ofstream::binary); // currently only support LF line endings output
		ofs << "#ifndef " << name_upper_h.c_str() << "\n#define " << name_upper_h.c_str() << "\n";
		if (store_vector) {
			ofs << "\n#ifdef __cplusplus\n#include <vector>\n#endif\n";
		}
		ofs << "\nstatic const unsigned char " << hname << "[] = {\n";

		// empty line
		cout << endl;

		// to check if we are at the end of file
		// FIXME: better method?
		bool eof = false;

		// write array data
		unsigned long long bytes_written = 0;
		unsigned long long chunk_idx;
		for (chunk_idx = 0; chunk_idx < chunk_count; chunk_idx++) {
			if (cancelled) {
				cout << "\nCancelled" << endl;
				break;
			} else if (eof) {
				break;
			}

			cout << "\rWriting chunk " << to_string(chunk_idx + 1) << " out of " << to_string(chunk_count) << " (Ctrl+C to cancel)";

			char chunk[chunk_size];
			ifs.seekg(chunk_idx * chunk_size);
			ifs.read(chunk, chunk_size);

			unsigned int byte_idx;
			for (byte_idx = 0; byte_idx < chunk_size; byte_idx++) {
				if (cancelled) break;

				if ((bytes_written % 12) == 0) {
					ofs << "\t";
				}

				stringstream ss;
				ss << "0x" << hex << setw(2) << setfill('0') << (int) (unsigned char) chunk[byte_idx];

				ofs << ss.str();
				bytes_written++;

				if (bytes_written >= data_length) {
					eof = true;
					break;
				} else {
                    ofs << ",";
                    if ((bytes_written % 12) == 0) {
                        ofs << "\n";
                    }
                    else {
                        ofs << " ";
                    }
				}
			}	/* for (byte_idx...) */
		}

		// flush stdout
		cout << endl << endl;

		// release input file after read
		ifs.close();

		ofs << "\n};\n";
		if (store_vector) {
			ofs << "\n#ifdef __cplusplus\nstatic const std::vector<char> "
					<< hname << "_v(" << hname << ", " << hname << " + sizeof("
					<< hname << "));\n#endif\n";
		}
		ofs << "\n#endif /* " << name_upper_h << " */\n";

		ofs.close();

		if (bytes_written != data_length) {
			cout << "WARNING: input file size (" << to_string(data_length) << ") and bytes written (" << to_string(bytes_written) << ") do not match" << endl;
		} else {
			cout << "Wrote " << to_string(bytes_written) << " bytes" << endl;
		}
	} catch (int e) {
		return e;
	}

	return 0;
}
