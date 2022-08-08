#!/usr/bin/env python

import codecs, errno, os, re, sys

file_exe = os.path.basename(__file__)
dir_root = os.path.normpath(os.path.join(os.path.dirname(__file__), "../"))
dir_src = os.path.join(dir_root, "src")

ver = ""
ver_py = ""

def printInfo(lvl, msg):
	lvl = lvl.lower()

	if lvl == "i":
		lvl = "INFO"
	elif lvl == "w":
		lvl = "WARNING"
	elif lvl == "e":
		lvl = "ERROR"
	else:
		print("ERROR: ({} printInfo) Unknown level \"{}\"".format(file_exe, lvl))
		sys.exit(1)

	print("{}: {}".format(lvl, msg))

def cleanNewlines(text):
	# make sure we are working with LF line endings
	return text.replace("\r\n", "\n").replace("\r", "\n")

def readText(filename):
	if not os.path.isfile(filename):
		printInfo("e", "({} readText) File does not exist: {}".format(file_exe, filename))
		return errno.ENOENT

	buffer = codecs.open(filename, "r", "utf-8")
	text = cleanNewlines(buffer.read())
	buffer.close()

	return text

def readLines(filename):
	return readText(filename).split("\n")

def writeText(filename, text):
	buffer = codecs.open(filename, "w", "utf-8")
	buffer.write(text)
	buffer.close()

def writeLines(filename, lines):
	writeText(filename, "\n".join(lines))

def parseVersion():
	global ver, ver_py
	cfg = os.path.join(dir_root, "info.cfg")

	if not os.path.isfile(cfg):
		printInfo("e", "\"info.cfg\" not found in root directory")
		sys.exit(errno.ENOENT)

	lines = readLines(cfg)
	for l in lines:
		if l.startswith("VERSION="):
			ver = l.lstrip("VERSION=")
		if l.startswith("PYVERSION="):
			ver_py = l.lstrip("PYVERSION=")

	if not ver:
		printInfo("e", "Could not get version information from \"info.cfg\"")
		sys.exit(1)
	if not ver_py:
		printInfo("w", "Could not get Python version information from \"info.cfg\"")

	print("\nVersion info:\n  Native: {}\n  Python: {}".format(ver, ver_py))

def updateSource():
	print()
	printInfo("i", "Updating source ...")

	src_cpp = os.path.join(dir_src, "bin2header.cpp")
	if not os.path.isfile(src_cpp):
		printInfo("e", "src/bin2header.cpp not found")
		return errno.ENOENT

	old_text = readText(src_cpp)
	new_text = re.sub("^const string version = \".*\";$", "const string version = \"{}\";".format(ver),
			old_text, flags=re.M)

	if new_text == old_text:
		printInfo("i", "C++ source already up to date")
	else:
		writeText(src_cpp, new_text)
		printInfo("i", "Updated C++ source")

	src_py = os.path.join(dir_src, "bin2header.py")
	if not os.path.isfile(src_py):
		printInfo("e", "src/bin2header.py not found")
		return errno.ENOENT

	old_text = readText(src_py)
	new_text = re.sub("^version = '.*'$", "version = '{}'".format(ver_py),
			old_text, flags=re.M)

	if old_text == new_text:
		printInfo("i", "Python source already up to date")
	else:
		writeText(src_py, new_text)
		printInfo("i", "Updated Python source")

	return 0

def updateCMake():
	print()
	printInfo("i", "Updating CMake files ...")

	file_cmake = os.path.join(dir_root, "CMakeLists.txt")
	if not os.path.isfile(file_cmake):
		printInfo("e", "CMakeLists.txt not found")
		return errno.ENOENT

	old_text = readText(file_cmake)
	new_text = old_text
	new_text = re.sub("^\tVERSION .*$", "\tVERSION {}".format(ver),
			new_text, flags=re.M)
	new_text = re.sub("^set\(PROJECT_PY_VERSION .*\)$", "set(PROJECT_PY_VERSION {})".format(ver_py),
			new_text, flags=re.M)

	if new_text == old_text:
		printInfo("i", "CMake files already up to date")
		return 0

	writeText(file_cmake, new_text)
	printInfo("i", "Updated CMake files")
	return 0

def updateChangelog():
	print()
	printInfo("i", "Updating changelog ...")

	if not ver:
		parseVersion()

	changelog = os.path.join(dir_root, "CHANGES.txt")
	if not os.path.isfile(changelog):
		printInfo("w", "CHANGES.txt not found")
		return errno.ENOENT

	text = readText(changelog)

	if not text.strip():
		printInfo("w", "Empty changelog")
		return 0

	old_lines = text.split("\n")
	new_lines = list(old_lines)
	if new_lines[0] == "next":
		new_lines[0] = ver

	if new_lines == old_lines:
		printInfo("i", "Changelog contains version, no changes were made")
		return 0

	writeLines(changelog, new_lines)
	printInfo("i", "Updated changelog")
	return 0

def updateMan():
	print()
	printInfo("i", "Updating manpage ...")

	file_man = os.path.normpath(os.path.join(dir_root, "man/bin2header.1"))
	if not os.path.isfile(file_man):
		printInfo("e", "man/bin2header.1 not found")
		return errno.ENOENT

	old_text = readText(file_man)
	new_text = re.sub("\"bin2header-.*\"$", "\"bin2header-{}\"".format(ver),
			old_text,
			flags=re.M)

	if new_text == old_text:
		printInfo("i", "Manpage already up to date")
		return 0

	writeText(file_man, new_text)
	printInfo("i", "Updated manpage")
	return 0

def updateReadme():
	print()
	printInfo("i", "Updating README ...")

	file_readme = os.path.join(dir_root, "README.md")
	if not os.path.isfile(file_readme):
		printInfo("e", "README.md not found")
		return errno.ENOENT

	old_text = readText(file_readme)
	new_text = old_text
	new_text = re.sub("(Latest release: )\[v.*\]\((.*)/tag/v.*\)$", "\\1[v{0}](\\2/tag/v{0})".format(ver),
			new_text,
			flags=re.M)
	new_text = re.sub("(Latest Python release: )\[v.*\]\((.*)/tag/v.*\)$", "\\1[v{0}](\\2/tag/v{0})".format(ver_py),
			new_text,
			flags=re.M)

	if new_text == old_text:
		printInfo("i", "README already up to date")
		return 0

	writeText(file_readme, new_text)
	printInfo("i", "Updated README")
	return 0

def main():
	parseVersion()

	ret = updateSource()
	if ret > 0:
		printInfo("e", "Could not update source files")
		sys.exit(ret)

	ret = updateCMake()
	if ret > 0:
		printInfo("e", "Could not update CMake files")
		sys.exit(ret)

	ret = updateChangelog()
	if ret > 0:
		printInfo("e", "Could not update changelog")
		sys.exit(ret)

	ret = updateMan()
	if ret > 0:
		printInfo("e", "Could not update manpage")
		sys.exit(ret)

	ret = updateReadme()
	if ret > 0:
		printInfo("e", "Could not update README")
		sys.exit(ret)

if __name__ == "__main__":
	main()
