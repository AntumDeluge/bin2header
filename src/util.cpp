
/* Copyright © 2017-2022 Jordan Irwin (AntumDeluge) <antumdeluge@gmail.com>
 *
 * This file is part of the bin2header project & is distributed under the
 * terms of the MIT/X11 license. See: LICENSE.txt
 */

#include "util.h"

#include <chrono>
#include <cmath>
#include <sstream>

using namespace std;


bool checkEmptyString(const string st) {
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


long long currentTimeMillis() {
	return chrono::duration_cast<chrono::milliseconds>(
			chrono::system_clock::now().time_since_epoch()).count();
}


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
