#! /usr/bin/env python
# -*- coding: utf-8 -*-

import sys, os, array


if sys.version_info.major < 3:
	print('\nERROR: Python ' + str(sys.version_info.major) + ' not supported. Please upgrade to Python 3.\n')
	sys.exit(2)


__WIN32__ = 'windows' in os.getenv('OS').lower();


version = '0.1.2'

## Normalizes the path for they current system
def NormalizePath(path):
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


def GetBaseName(f):
	base_name = os.path.basename(f)

	# MSYS versions of Python appear to not understand Windows paths
	if __WIN32__ and '\\' in base_name:
		base_name = base_name.split('\\')[-1]

	return base_name


def GetDirName(f):
	dir_name = os.path.dirname(f)

	# MSYS versions of Python appear to not understand Windows paths
	if not dir_name and __WIN32__:
		dir_name = '\\'.join(f.split('\\')[:-1])

	return dir_name


def PrintUsage():
	executable = os.path.basename(__file__)
	print('\nbin2header version {} (Python)\nCopyright © 2019 Jordan Irwin <antumdeluge@gmail.com>\n\n\tUsage:\t{} <file>\n'.format(version, year, executable))


def main(argv):
	source_file = NormalizePath(argv[1])

	# Check if file exists
	if not os.path.isfile(source_file):
		print('\nFile "{}" does not exist'.format(source_file))
		PrintUsage()
		sys.exit(1)

	### Get filenames and target directory ###
	filename = list(GetBaseName(source_file))
	hname = list(filename)
	target_dir = GetDirName(source_file)

	### Remove Unwanted Characters ###
	badchars = ('\\', '+', '-', '*', ' ')
	for x in range(len(hname)):
		if hname[x] in badchars or hname[x] == '.':
			hname[x] = '_'
		if filename[x] in badchars:
			filename[x] = '_'

	filename = ''.join(filename)
	hname = ''.join(hname)
	target_file = os.path.join(target_dir, filename) + '.h'

	### Uppercase Name for Header ###
	hname_upper = hname.upper()
	hname_upper += '_H'

	### Read Data In ###
	data = array.array('B', open(source_file, 'rb').read())

	### START Read Data Out to Header ###

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
	### END Read Data Out to Header ###

	print('Exported to: {}'.format(target_file))

	return 0


if __name__ == '__main__':
	if len(sys.argv) < 2:
		print('\nERROR: Missing <file> argument')
		PrintUsage()
		sys.exit(1)

	main(sys.argv)
