#! /usr/bin/env python

# Copyright © 2017-2022 Jordan Irwin (AntumDeluge) <antumdeluge@gmail.com>
#
# This file is part of the bin2header project & is distributed under the
# terms of the MIT/X11 license. See: LICENSE.txt

import array, errno, os, sys, traceback


if sys.version_info.major < 3:
	print("\nERROR: Python " + str(sys.version_info.major) + " not supported. Please upgrade to Python 3.\n")
	sys.exit(2)


__WIN32__ = "windows" in str(os.getenv("OS")).lower();

appname = "Binary to Header"
version = "0.1.2"
executable = os.path.basename(__file__)

symbolcp = "©"
if __WIN32__:
	# some characters don't display correctly in Windows console
	symbolcp = "(C)"


# options configured from command parameters
options = {}
options_defaults = {
	"help": {"short": "h", "value": False},
	"version": {"short": "v", "value": False},
}


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
#  @tparam bool newline
#      If `True` prepend message with newline character
#      (default: `False`).
def printInfo(lvl, msg=None, newline=False):
	if msg == None:
		msg = lvl
		lvl = "i"

	lvl = lvl.lower()
	if lvl in ("q", "quiet"):
		# quiet displays message without level
		lvl = "q"
	elif lvl in ("i", "info"):
		lvl = "INFO"
	elif lvl in ("w", "warn"):
		lvl = "WARNING"
	elif lvl in ("e", "error"):
		lvl = "ERROR"
	elif lvl in ("d", "debug"):
		lvl = "DEBUG"
	else:
		print("ERROR: ({}) Unknown message level: {}".format(printInfo.__name__, lvl))
		sys.exit(1)

	if lvl != "q":
		msg = "{}: {}".format(lvl, msg)

	if newline:
		msg = "\n{}".format(msg)

	cout = sys.stdout
	if lvl == "ERROR":
		cout = sys.stderr

	cout.write("{}\n".format(msg))


## Prints app name & version.
def printVersion():
	print("\n{} version {} (Python)".format(appname, version)
			+ "\nCopyright {} 2017-2022 Jordan Irwin <antumdeluge@gmail.com>".format(symbolcp))

## Prints usage information.
def printUsage():
	printVersion()
	print("\n  Usage:\n\t{} [options] <file>\n".format(executable)
			+ "\n  Options:"
			+ "\n\t-h, --help\t\tPrint help information & exit."
			+ "\n\t-v, --version\t\tPrint version information & exit.")

## Prints message to stderr & exits program.
#
#  @tparam int code
#      Program exit code.
#  @tparam str msg
#      Message to print.
#  @tparam bool show_usage
#      If `True` will print usage info.
def exitWithError(code, msg, show_usage=False):
	printInfo("e", msg, True)
	if show_usage:
		printUsage()
	sys.exit(code)


## Retrieves a configured option.
#
#  @tparam str key
#      Option identifier.
#  @tparam bool default
#      If `True` retrieves default value (default: `False`).
#  @treturn str,str
#      Returns option identifier & configured
#      value (or `None`,`None` if identifier not found).
def getOpt(key, default=False):
	if len(key) == 1:
		key_match = False
		for long_key in options_defaults:
			opt_value = options_defaults[long_key]

			# convert short id to long
			if "short" in opt_value and key == opt_value["short"]:
				key = long_key
				key_match = True
				break

		# invalid short id
		if not key_match:
			return None, None

	if not default and key in options:
		return key, options[key]

	# get default if not configured manually
	if key in options_defaults:
		return key, options_defaults[key]["value"]

	return None, None

## Configures an option from a command line parameter.
#
#  @tparam str key
#      Option identifier.
#  @tparam str value
#      New value to be set.
def setOpt(key, value):
	long_key, default_val = getOpt(key, True)

	# not a valid parameter id
	if long_key == None:
		printInfo("e", "({}) Unknown parameter: {}".format(setOpt.__name__, key))
		traceback.print_stack()
		sys.exit(errno.EINVAL)

	# only compatible types can be set
	opt_type, val_type = type(default_val), type(value)
	if opt_type != val_type:
		printInfo("e", "({}) Value for option \"{}\" must be \"{}\" but found \"{}\""
				.format(setOpt.__name__, long_key, opt_type.__name__, val_type.__name__))
		traceback.print_stack()
		sys.exit(errno.EINVAL)

	options[long_key] = value

## Checks if an argument uses a short ID.
#
#  Does not differenciate between '-a' & '-abcd'.
#
#  @tparam str a
#      String to be checked.
def checkShortArg(a):
	if a.startswith("-") and a.count("-") == 1:
		return a.lstrip("-")

	return None

## Checks if an argument uses a long ID.
#
#  @tparam str a
#      String to be checked.
def checkLongArg(a):
	if a.startswith("--"):
		d_count = 0
		for ch in a:
			if ch != "-":
				break

			d_count = d_count + 1

		if d_count == 2:
			return a.lstrip("-")

	return None

## Parses options from command line.
#
#  @tparam list args
#      Command line arguments.
#  @treturn list
#      Remaining arguments.
def parseCommandLine(args):
	# unparsed arguments
	rem = []

	while len(args) > 0:
		cur_arg = args[0]

		# skip values
		if not cur_arg.startswith("-"):
			if len(args) > 1:
				# FIXME: error code?
				exitWithError(1, "Unknown argument: {}".format(cur_arg))

			rem.append(args.pop(0))
			continue

		res = checkShortArg(cur_arg)
		if res == None:
			res = checkLongArg(cur_arg)

		if res == None and cur_arg.startswith("-"):
			# FIXME: exit code?
			exitWithError(1, "Malformed argument: {}".format(cur_arg))

		if res != None:
			# check if argument is recognized
			key, val_default = getOpt(res, True)
			val_type = type(val_default)

			if val_default == None:
				# FIXME: exit code?
				exitWithError(1, "Unknown argument: {}".format(cur_arg), True)

			if val_type == bool:
				value = True
			else:
				if len(args) < 2:
					# FIXME: exit code?
					exitWithError(1, "\"{}\" requires value".format(cur_arg), True)

				try:
					value = val_type(args[1])
				except ValueError:
					# FIXME: error code?
					exitWithError(1, "\"{}\" value must be of type \"{}\"".format(cur_arg, val_type.__name__))

				# remove value from argument list
				args.pop(1)

			# update config
			setOpt(key, value)
			# remove from argument list
			args.pop(0)

	# input file should be only remaining argument
	return rem


## Normalizes the path node separators for the current system.
#
#  @tparam str path
#      Path to be normalized.
#  @treturn str
#      Path formatted with native directory/node delimeters.
def normalizePath(path):
	new_path = path
	to_replace = "\\"
	replace_with = "/"

	if __WIN32__:
		to_replace = "/"
		replace_with = "\\"

	new_path = new_path.replace(to_replace, replace_with)

	if __WIN32__:
		# MSYS2/MinGW paths
		if new_path.lower().startswith("\\c\\"):
			new_path = "C:{}".format(new_path[2:])

	return new_path;


## Removes path to parent directory from path name.
#
#  @tparam str path
#      Path to be parsed.
#  @treturn str
#      Name of last node in path.
def getBaseName(path):
	base_name = os.path.basename(path)

	# MSYS versions of Python appear to not understand Windows paths
	if __WIN32__ and "\\" in base_name:
		base_name = base_name.split("\\")[-1]

	return base_name


## Removes last node from path name.
#
#  @tparam str path
#      Path to be parsed.
#  @treturn
#      Path to parent directory.
def getDirName(path):
	dir_name = os.path.dirname(path)

	# MSYS versions of Python appear to not understand Windows paths
	if not dir_name and __WIN32__:
		dir_name = "\\".join(f.split("\\")[:-1])

	return dir_name


## Main function called at program start.
#
#  @tparam list argv
#      Command line arguments.
def main(argv):
	argc = len(argv)
	argv = parseCommandLine(argv)

	if getOpt("help")[1]:
		printUsage()
		return 0

	if getOpt("version")[1]:
		printVersion()
		return 0

	if len(argv) == 0:
		exitWithError(1, "Missing <file> argument", True)

	# XXX: make this global?
	source_file = argv[0]
	argv.pop(0)

	if len(argv) > 0:
		printInfo("w", "Some command arguments were not parsed: {}".format(argv))

	# check if file exists
	if not os.path.isfile(source_file):
		exitWithError(errno.ENOENT, "\nFile \"{}\" does not exist".format(source_file))

	# get filenames and target directory
	filename = list(getBaseName(source_file))
	hname = list(filename)
	target_dir = getDirName(source_file)

	# remove unwanted characters
	badchars = ("\\", "+", "-", "*", " ")
	for x in range(len(hname)):
		if hname[x] in badchars or hname[x] == ".":
			hname[x] = "_"
		if filename[x] in badchars:
			filename[x] = "_"

	filename = "".join(filename)
	hname = "".join(hname)
	target_file = os.path.join(target_dir, filename) + ".h"

	# uppercase name for header
	hname_upper = hname.upper()
	hname_upper += "_H"

	# read data in
	data = array.array("B", open(source_file, "rb").read())

	# *** START: write data *** #

	# adds C++ std::vector support
	# TODO: make optional
	store_vector = True

	# currently only support LF line endings output
	outfile = open(target_file, "w", newline="\n")

	text = "#ifndef {0}\n#define {0}\n".format(hname_upper)
	if store_vector:
		text += "\n#ifdef __cplusplus\n#include <vector>\n#endif\n"
	text += "\nstatic const unsigned char {}[] = {{\n".format(hname)

	current = 0
	data_length = len(data)
	for byte in data:
		if (current % 12) == 0:
			text += "    "
		text += "0x%02x" % byte

		if (current + 1) < data_length:
			text += ", "
		if (current % 12) == 11:
			text += "\n"

		current += 1

	text += "\n};\n"
	if store_vector:
		text += "\n#ifdef __cplusplus\nstatic const std::vector<char> " \
		+ hname + "_v(" + hname + ", " + hname + " + sizeof(" + hname \
		+ "));\n#endif\n"
	text +="\n#endif /* {} */\n".format(hname_upper)

	outfile.write(text)
	outfile.close()

	# *** END: write data *** #

	print("Exported to: {}".format(target_file))

	return 0


# program entry point.
if __name__ == "__main__":
	# remove executable name from arguments passed to main function
	sys.exit(main(sys.argv[1:]))
