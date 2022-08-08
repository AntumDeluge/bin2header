#!/usr/bin/env python

import codecs, errno, os, sys

dir_root = os.path.normpath(os.path.join(os.path.dirname(__file__), "../"))

def getVersion():
	ver = None
	cfg = os.path.join(dir_root, "info.cfg")

	if not os.path.isfile(cfg):
		print("ERROR: \"info.cfg\" not found in root directory")
		sys.exit(errno.ENOENT)

	buffer = codecs.open(cfg, "r", "utf-8")
	lines = buffer.readlines()
	buffer.close()

	for l in lines:
		l = l.strip()
		if l.startswith("VERSION="):
			ver = l.lstrip("VERSION=")

	return ver

def updateChangelog():
	ver = getVersion()
	if not ver:
		print("ERROR: could not get version information from \"info.cfg\"")
		sys.exit(1)

	changelog = os.path.join(dir_root, "CHANGES.txt")
	if not os.path.isfile(changelog):
		print("ERROR: CHANGES.txt not found in root directory")
		sys.exit(errno.ENOENT)

	buffer = codecs.open(changelog, "r", "utf-8")
	lines = buffer.readlines()
	buffer.close()

	old_lines = []
	for l in lines:
		# remove newline characters
		old_lines.append(l.strip("\r\n"))

	new_lines = list(old_lines)
	if new_lines[0] == "next":
		new_lines[0] = ver

	if new_lines == old_lines:
		print("Changelog contains version, no changes were made")
		sys.exit(0)

	buffer = codecs.open(changelog, "w", "utf-8")
	buffer.write("{}\n".format("\n".join(new_lines)))
	buffer.close()

	print("Updated changelog version")

if __name__ == "__main__":
	updateChangelog()
