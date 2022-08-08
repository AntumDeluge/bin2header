#! /usr/bin/env python

# Copyright © 2017-2022 Jordan Irwin (AntumDeluge) <antumdeluge@gmail.com>
#
# This file is part of the bin2header project & is distributed under the
# terms of the MIT/X11 license. See: LICENSE.txt

import array, os, sys


if sys.version_info.major < 3:
	print('\nERROR: Python ' + str(sys.version_info.major) + ' not supported. Please upgrade to Python 3.\n')
	sys.exit(2)


__WIN32__ = 'windows' in str(os.getenv('OS')).lower();

version = '0.1.2'
symbolcp = '©'
if __WIN32__:
	# some characters don't display correctly in Windows console
	symbolcp = '(C)'


## Prints message to console.
#
#  If the `msg` parameter is omitted, `lvl` is used for message
#  `lvl` is used for message text & level defaults to INFO.
#
#  Levels:
#  - q: QUIET (displays message with level info)
#  - i: INFO
#  - w: WARNING
#  - e: ERROR
#
#  @tparam str lvl
#      Message level (or message text if `msg` is omitted).
#  @tparam str msg
#      Message text to be printed.
def printInfo(lvl, msg=None):
	if msg == None:
		msg = lvl
		lvl = "i"

	lvl = lvl.lower()
	if lvl == "q":
		# quiet displays message without level
		pass
	elif lvl == "i":
		lvl = "INFO"
	elif lvl == "w":
		lvl = "WARNING"
	elif lvl == "e":
		lvl = "ERROR"
	else:
		print("ERROR: (printInfo) Unknown message level")
		sys.exit(1)

	if lvl == "q":
		print(msg)
	else:
		print("{}: {}".format(lvl, msg))


## Normalizes the path for the current system.
#
#  @tparam str path
#      Path to be normalized.
#  @treturn str
#      Path with directory/node delimeters for current system.
def normalizePath(path):
	new_path = path
	to_replace = '\\'
	replace_with = '/'

	if __WIN32__:
		to_replace = '/'
		replace_with = '\\'

	new_path = new_path.replace(to_replace, replace_with)

	if __WIN32__:
		# MSYS2/MinGW paths
		if new_path.lower().startswith('\\c\\'):
			new_path = 'C:{}'.format(new_path[2:])

	return new_path;


## Removes prefixes from a path.
#
#  @tparam str path
#      Path to be parsed.
#  @treturn str
#      Last node of path.
def getBaseName(path):
	base_name = os.path.basename(path)

	# MSYS versions of Python appear to not understand Windows paths
	if __WIN32__ and '\\' in base_name:
		base_name = base_name.split('\\')[-1]

	return base_name


## Removes basename from a path.
#
#  @tparam str path
#      Path to be parsed.
#  @treturn
#      Path with last node removed.
def getDirName(path):
	dir_name = os.path.dirname(path)

	# MSYS versions of Python appear to not understand Windows paths
	if not dir_name and __WIN32__:
		dir_name = '\\'.join(f.split('\\')[:-1])

	return dir_name


## Prints usage information to console.
def printUsage():
	executable = os.path.basename(__file__)
	print('\nbin2header version {} (Python)'.format(version)
			+ '\nCopyright {} 2017-2022 Jordan Irwin <antumdeluge@gmail.com>'.format(symbolcp)
			+ '\n\n\tUsage:\t{} <file>\n'.format(executable))


## Main function called at program start.
#
#  @tparam list argv
#      Command line arguments.
def main(argv):
	source_file = normalizePath(argv[1])

	# check if file exists
	if not os.path.isfile(source_file):
		print('\nFile "{}" does not exist'.format(source_file))
		printUsage()
		sys.exit(1)

	# get filenames and target directory
	filename = list(getBaseName(source_file))
	hname = list(filename)
	target_dir = getDirName(source_file)

	# remove unwanted characters
	badchars = ('\\', '+', '-', '*', ' ')
	for x in range(len(hname)):
		if hname[x] in badchars or hname[x] == '.':
			hname[x] = '_'
		if filename[x] in badchars:
			filename[x] = '_'

	filename = ''.join(filename)
	hname = ''.join(hname)
	target_file = os.path.join(target_dir, filename) + '.h'

	# uppercase name for header
	hname_upper = hname.upper()
	hname_upper += '_H'

	# read data in
	data = array.array('B', open(source_file, 'rb').read())

	### START: read data out to header ###

	# adds C++ std::vector support
	# TODO: make optional
	store_vector = True

	# currently only support LF line endings output
	outfile = open(target_file, 'w', newline='\n')

	text = '#ifndef {0}\n#define {0}\n'.format(hname_upper)
	if store_vector:
		text += '\n#ifdef __cplusplus\n#include <vector>\n#endif\n'
	text += '\nstatic const unsigned char {}[] = {{\n'.format(hname)

	current = 0
	data_length = len(data)
	for byte in data:
		if (current % 12) == 0:
			text += '    '
		text += '0x%02x' % byte

		if (current + 1) < data_length:
			text += ', '
		if (current % 12) == 11:
			text += '\n'

		current += 1

	text += '\n};\n'
	if store_vector:
		text += '\n#ifdef __cplusplus\nstatic const std::vector<char> ' \
		+ hname + '_v(' + hname + ', ' + hname + ' + sizeof(' + hname \
		+ '));\n#endif\n'
	text +='\n#endif /* {} */\n'.format(hname_upper)

	outfile.write(text)
	outfile.close()

	### END: read eata out to header ###

	print('Exported to: {}'.format(target_file))

	return 0


## Program entry point.
if __name__ == '__main__':
	if len(sys.argv) < 2:
		print('\nERROR: Missing <file> argument')
		printUsage()
		sys.exit(1)

	main(sys.argv)
