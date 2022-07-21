#!/usr/bin/env bash

DIR_ROOT="$(dirname "$0")/../"
cd "${DIR_ROOT}"
DIR_ROOT="$(pwd)"

year="$(date +"%Y")"

# import settings
. "${DIR_ROOT}/info.cfg"

sed -i -e "s|	VERSION .*$|	VERSION ${VERSION}|" -e "s|set(PROJECT_PY_VERSION.*)|set(PROJECT_PY_VERSION ${PYVERSION})|" "${DIR_ROOT}/CMakeLists.txt"
sed -i -e "s|^version =.*$|version = '${PYVERSION}'|" "${DIR_ROOT}/src/bin2header.py"
sed -i -e "s|version =.*;|version = \"${VERSION}\";|" "${DIR_ROOT}/src/bin2header.cpp"
sed -i -e "s|Latest release: \[v.*\]\(.*\)/tag/v.*)|Latest release: \[v${VERSION}\]\1/tag/v${VERSION})|" "${DIR_ROOT}/README.md"
sed -i -e "s|Latest Python release: \[v.*\]\(.*\)/tag/v.*)|Latest Python release: \[v${PYVERSION}\]\1/tag/v${PYVERSION})|" "${DIR_ROOT}/README.md"
sed -i -e "s|\"bin2header-.*\"|\"bin2header-${VERSION}\"|" "${DIR_ROOT}/man/bin2header.1"

files_src="$(find "${DIR_ROOT}/src" -type f -not -name "cxxopts.hpp") LICENSE.txt"

for F in ${files_src}; do
	sed -i -e "s|Copyright © [^ ]*|Copyright © 2017-${year}|g" -e "s|Copyright (C) [^ ]*|Copyright (C) 2017-${year}|g" "${F}"
done

# set changelog entry labelled "Next" to current version
sed -i -e "s|^Next$|${VERSION}|" "${DIR_ROOT}/CHANGES.txt"

# update HTML manpage for webpage
which pandoc > /dev/null 2>&1
if test $? -eq 0; then
	#pandoc --eol=lf -r man -w gfm -o docs/reference/bin2header.1.md man/bin2header.1
	pandoc -s --eol=lf -r man -w html5 -o docs/reference/bin2header.1.html man/bin2header.1
fi
