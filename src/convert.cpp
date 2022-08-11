
/* Copyright © 2017-2022 Jordan Irwin (AntumDeluge) <antumdeluge@gmail.com>
 *
 * This file is part of the bin2header project & is distributed under the
 * terms of the MIT/X11 license. See: LICENSE.txt
 */

#include "convert.h"
#include "paths.h"

#include <cerrno>
#include <chrono>
#include <cmath> // ceil
#include <csignal>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <iostream>

using namespace std;


unsigned int chunk_size = 1024 * 1024; // buffer size (default: 1MB)
unsigned int nbData     = 12;          // number of bytes to write per line
unsigned long offset    = 0;           // position at which to start reading file
unsigned long length    = 0;           // number of bytes to process (0 = all)
unsigned int outlen     = 8;           // output data type bit length (8/16/32)
bool showDataContent    = false;       // enable to show data content in comments
bool swap_bytes         = false;       // enable to swap byte order for bigger types (changes endianess)
string eol              = "\n";        // end of line character

bool cancelled = false;

void setChunkSize(const unsigned int sz) { chunk_size = sz; }
void setNumberDataPerLine(const unsigned int nd) { nbData = nd; }
void setShowDataContent(const bool dc) { showDataContent = dc; }
void setOutputBitLength(const unsigned int bl) { if ((bl == 16) || (bl == 32)) outlen = bl; }
void setReadOffset(const unsigned long ofs) { offset = ofs; }
void setReadLength(const unsigned long lgt) { length = lgt; }
void setSwapEndianess() { swap_bytes = true; }

void setEol(const string newEol) {
	if (newEol == "cr") {
		eol = "\r";
	} else if (newEol == "crlf") {
		eol = "\r\n";
	} else if (newEol != "lf") {
		cout << "\nWARNING: Unknown EOL type \"" << newEol << "\", using default \"lf\"\n" << endl;
	}
}

/** Cancels current write iteration. */
void sigintHandler(int signum) {
	cout << "\nSignal interrupt caught, cancelling ..." << endl;
	cancelled = true;

	// reset handler to catch SIGINT next time
	signal(SIGINT, sigintHandler);
}


/** Converts non-printable characters to ".".
 *
 *  @tparam char c
 *      Character to evaluate.
 *  @return
 *      Same character or "." non-printable.
 */
char toPrintableChar(char c) {
	if ((c >= ' ') && (c <= '~') ) {
		return c;
	} else {
		return '.';
	}
}


/** Checks for an empty string.
 *
 *  Checks string length & if contains only whitespace characters.
 *
 *  @tparam string st
 *      String to be checked.
 *  @return
 *      `true` if empty or only whitepace characters found.
 */
bool checkEmptyString(string st) {
	if (st.empty()) {
		return true;
	}

	if (st.find_first_not_of(' ') == string::npos) {
		if (st.find_first_not_of('\t') == string::npos) {
			if (st.find_first_not_of('\n') == string::npos) {
				if (st.find_first_not_of('\r') == string::npos) {
					return true;
				}
			}
		}
	}

	return false;
}


/** Retrieves current timestamp in milliseconds. */
long long currentTimeMillis() {
	return chrono::duration_cast<chrono::milliseconds>(
			chrono::system_clock::now().time_since_epoch()).count();
}


/** Formats duration for printing.
 *
 *  @param ts
 *      Process start timestamp (milliseconds).
 *  @param te
 *      Process end timestamp (milliseconds).
 *  @return
 *      String formatted for readability.
 */
string formatDuration(const long long ts, const long long te) {
	const int duration = te - ts;
	stringstream dmsg;

	int dsec = floor(duration / 1000);
	if (dsec < 1) {
		dmsg << duration << " ms";
	} else {
			const int dmin = floor(dsec / 60);
			if (dmin > 0) {
				dmsg << dmin << " min";
				dsec = dsec % (dmin * 60);
				if (dsec > 0) {
					dmsg << " " << dsec << " sec";
				}
			} else {
				dmsg << dsec << " sec";
			}
	}

	return dmsg.str();
}


int convert(const string fin, string fout, string hname, const bool stdvector) {
	const string source_basename = getBaseName(fin);
	string target_basename;
	string target_dir;
	if (checkEmptyString(fout)) {
		target_basename = source_basename + ".h";
		target_dir = getDirName(fin);
	} else {
		target_basename = getBaseName(fout);
		target_dir = getDirName(fout);
	}

	if (checkEmptyString(hname)) {
		// use source filename as default
		hname = source_basename;
	}

	/* *** START: check characters *** */

	char badchars[6] = {'\\', '+', '-', '*', ' '};
	for (int current = 0; current < hname.length(); current++) {
		for (int x = 0; x < len(badchars); x++) {
			if ((hname[current] == badchars[x]) || (hname[current] == '.'))
				hname.replace(current, 1, "_");
			if (target_basename[current] == badchars[x])
				target_basename.replace(current, 1, "_");
		}
	}

	fout = joinPath(target_dir, target_basename);

	// add '_' when first char is a number
	if (isdigit(hname[0]))
	{
		hname.insert(0, 1, '_');
	}

	/* *** END: check characters *** */

	/* *** START: uppercase header name *** */

	char hname_upper[hname.length() + 2];
	for (int current = 0; current < len(hname_upper); current++) {
		hname_upper[current] = hname[current];
		hname_upper[current] = toupper(hname_upper[current]);
	}

	string name_upper_h = hname_upper;
	name_upper_h.append("_H");

	/* *** END: uppercase header name *** */

	// set signal interrupt (Ctrl+C) handler
	signal(SIGINT, sigintHandler);

	/* *** START: read/write *** */

	// file streams
	ifstream ifs;
	ofstream ofs;
	unsigned long long bytes_written = 0;

	const long long starttime = currentTimeMillis();

	try {
		ifs.open(fin.c_str(), ifstream::binary);

		unsigned long long data_length;
		unsigned char wordbytes = outlen / 8;
		ifs.seekg(0, ifstream::end);
		data_length = ifs.tellg();
		ifs.seekg(0, ifstream::beg);

		if (offset > data_length) {
			cout << "ERROR: offset bigger than file length" << endl;
			return -1;
		}

		unsigned long long chunk_count = ceil((double) (data_length - offset) / chunk_size);

		if (chunk_size % wordbytes) {
			cout << "Warning: Chunk size truncated to full words length" << endl;
			chunk_size -= chunk_size % wordbytes;
		}

		cout << "File size:  " << to_string(data_length) << " bytes" << endl;
		cout << "Chunk size: " << to_string(chunk_size) << " bytes" << endl;

		if (offset) cout << "Start from position: " << to_string(offset) << endl;
		if (length) cout << "Process maximum " << to_string(length) << " bytes" << endl;
		if (outlen != 8) cout << "Pack into " << to_string(outlen) << " bit ints" << endl;
		if (outlen > 8 && swap_bytes) cout << "Swap endianess" << endl;

		ofs.open(fout.c_str(), ofstream::binary);
		ofs << "#ifndef " << name_upper_h.c_str() << eol << "#define " << name_upper_h.c_str() << eol;
		if (stdvector) {
			ofs << eol << "#ifdef __cplusplus" << eol << "#include <vector>" << eol << "#endif" << eol;
		}

		if (outlen == 32) ofs << eol << "static const unsigned int " << hname << "[] = {" << eol;
		else if (outlen == 16) ofs << eol << "static const unsigned short " << hname << "[] = {" << eol;
		else ofs << eol << "static const unsigned char " << hname << "[] = {" << eol;

		// empty line
		cout << endl;

		// to check if we are at the end of file
		// FIXME: better method?
		bool eof = false;

		// how many bytes to write
		unsigned long long bytes_to_go = data_length - offset;
		if (length > 0 && length < bytes_to_go) bytes_to_go = length;

		// check if there are any bytes to omit during packing
		// FIXME: incomplete words not processed
		int omit = bytes_to_go % (outlen / 8);
		if(omit) {
			cout << "Warning: Last " << to_string(omit) << " byte(s) will be ignored as not forming full data word" << endl;
			bytes_to_go -= omit;
		}

		unsigned long long chunk_idx;
		std::string comment = "";
		for (chunk_idx = 0; chunk_idx < chunk_count; chunk_idx++) {
			if (eof || cancelled) {
				break;
			}

			cout << "\rWriting chunk " << to_string(chunk_idx + 1) << " out of " << to_string(chunk_count) << " (Ctrl+C to cancel)" << std::flush;

			char chunk[chunk_size];
			ifs.seekg(chunk_idx * chunk_size + offset);
			ifs.read(chunk, chunk_size);

			unsigned int byte_idx;
			for (byte_idx = 0; byte_idx < chunk_size; byte_idx++) {
				if (cancelled) break;

				if ((bytes_written % (nbData * wordbytes)) == 0) {
					ofs << "\t";
					comment = "";
				}

				stringstream ss;
				unsigned int word;
				if (wordbytes == 2) {
					// pack input bytes into 16 bit ints
					if (swap_bytes) {
						word = (unsigned char) chunk[byte_idx++];
						word += (unsigned char) chunk[byte_idx] << 8;
					} else {
						word = (unsigned char) chunk[byte_idx++] << 8;
						word += (unsigned char) chunk[byte_idx];
					}
					ss << "0x" << hex << setw(4) << setfill('0') << (short) word;
				} else if (wordbytes == 4) {
					// pack input bytes into 32 bit words
					if (swap_bytes) {
						word = (unsigned char) chunk[byte_idx++];
						word += (unsigned char) chunk[byte_idx++] << 8;
						word += (unsigned char) chunk[byte_idx++] << 16;
						word += (unsigned char) chunk[byte_idx] << 24;
					} else {
						word = (unsigned char) chunk[byte_idx++] << 24;
						word += (unsigned char) chunk[byte_idx++] << 16;
						word += (unsigned char) chunk[byte_idx++] << 8;
						word += (unsigned char) chunk[byte_idx];
					}
					ss << "0x" << hex << setw(8) << setfill('0') << (int) word;
				} else {
					// pack single bytes
					ss << "0x" << hex << setw(2) << setfill('0') << (int)(unsigned char) chunk[byte_idx];
				}
				ofs << ss.str();
				if (showDataContent) {
					comment += toPrintableChar(chunk[byte_idx]);
				}
				bytes_written += wordbytes;

				if (bytes_written >= bytes_to_go) {
					eof = true;
					if (showDataContent) {
						for (int i = (bytes_written % (nbData * wordbytes)); i < (nbData * wordbytes); i++) {
							ofs << "      ";
						}
						ofs << "  /* " << comment << " */";
					}
					ofs << eol;
					break;
				} else {
					if ((bytes_written % (nbData * wordbytes)) == 0) {
						ofs << ",";
						if (showDataContent) {
							ofs << " /* " << comment << " */";
						}
						ofs << eol;
					} else {
						ofs << ", ";
					}
				}
			}	/* for (byte_idx...) */
		}

		// release input file after read
		ifs.close();
		if (cancelled) {
			// close write stream & exit
			ofs.close();
			return ECANCELED;
		}

		// empty line
		cout << endl << endl;

		ofs << "};" << eol;
		if (stdvector) {
			ofs << eol << "#ifdef __cplusplus" << eol << "static const std::vector<char> "
					<< hname << "_v(" << hname << ", " << hname << " + sizeof("
					<< hname << "));" << eol << "#endif" << eol;
		}
		ofs << eol << "#endif /* " << name_upper_h << " */" << eol;

		ofs.close();

	} catch (const int e) {
		// close read/write streams
		if (ifs.is_open()) {
			ifs.close();
		}
		if (ofs.is_open()) {
			ofs.close();
		}

		cout << "An error occurred during read/write. Code: " << e << endl;
		return e;
	}

	const long long endtime = currentTimeMillis();

	/* *** END: read/write *** */

	cout << "Wrote " << bytes_written << " bytes" << endl;
	cout << "Elapsed:     " << formatDuration(starttime, endtime) << endl;
	cout << "Exported to: " << fout << endl;

	return 0;
}
