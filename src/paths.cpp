
/* Copyright © 2019 Jordan Irwin (AntumDeluge) <antumdeluge@gmail.com>
 *
 * This file is part of the bin2header project & is distributed under the
 * terms of the MIT/X11 license. See: LICENSE.txt
 */

#include "paths.h"

#include <algorithm> // transform

using namespace std;


/** Normalizes the path for they current system */
string NormalizePath(string path) {
	string new_path = path;
	string to_replace;
	string replace_with;

#ifdef __WIN32__
	to_replace = '/';
	replace_with = '\\';
#else
	to_replace = '\\';
	replace_with = '/';
#endif

	string cur_char;
	for (int idx = 0; idx < path.length(); idx++) {
		cur_char = path[idx];
		if (cur_char == to_replace) {
			new_path.replace(idx, 1, replace_with);
		}
	}

#ifdef __WIN32__
	// MSYS2/MinGW paths
	string first_chars = new_path.substr(0, 3);
	// FIXME: use tolower from cstio?
	transform(first_chars.begin(), first_chars.end(), first_chars.begin(), ::tolower);

	// TODO: support all drive letters
	if (first_chars == "\\c\\") {
		new_path.replace(0, 2, "C:");
	}
#endif

	return new_path;
}


string JoinPath(string a, string b) {
#ifdef __WIN32__
	return a.append("\\").append(b);
#else
	return a.append("/").append(b);
#endif
}


string GetBaseName(string f) {
	int split;

#ifdef __WIN32__
	split = f.find_last_of('\\');
#else
	split = f.find_last_of('/');
#endif
	f.erase(0, split + 1);

	return f;
}


string GetDirName(string f) {
	int split;

#ifdef __WIN32__
	split = f.find_last_of('\\');
#else
	split = f.find_last_of('/');
#endif
	f.erase(split, f.length() - split);

	return f;
}
