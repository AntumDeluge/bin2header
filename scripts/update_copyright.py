#!/usr/bin/env python

import codecs, errno, os, re, sys
from datetime import datetime

dir_root = os.path.normpath(os.path.join(os.path.dirname(__file__), "../"))
dir_src = os.path.join(dir_root, "src")

year = datetime.now().year

excludes = ["include/cxxopts.hpp"]
for idx in range(len(excludes)):
	excludes[idx-1] = os.path.normpath(excludes[idx-1])

def updateCopyright(filename, text):
	new_text = text
	new_text = re.sub("Copyright © [^ ]*", "Copyright © 2017-{}".format(year), new_text)
	new_text = re.sub("Copyright \(C\) [^ ]*", "Copyright (C) 2017-{}".format(year), new_text)
	new_text = re.sub("Copyright \{\} [^ ]*", "Copyright {{}} 2017-{}".format(year), new_text)

	if new_text != text:
		buffer = codecs.open(filename, "w", "utf-8")
		buffer.write(new_text)
		buffer.close()

		print("Updated copyright year in {}".format(filename))

def parseFiles():
	dir_src = os.path.join(dir_root, "src")

	for ROOT, DIRS, FILES in os.walk(dir_src):
		for f in FILES:
			f = os.path.join(ROOT[len(dir_src):], f).lstrip("/\\")

			if f not in excludes:
				filename = os.path.join(dir_src, f)
				buffer = codecs.open(filename, "r", "utf-8")
				text = buffer.read()
				buffer.close()

				contains_copyright = "Copyright ©" in text
				if not contains_copyright:
					contains_copyright = "Copyright (C)" in text
				if not contains_copyright:
					contains_copyright = "Copyright {{}}" in text

				if contains_copyright:
					updateCopyright(filename, str(text))

def main():
	parseFiles()

if __name__ == "__main__":
	main()
