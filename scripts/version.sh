#!/usr/bin/env bash

cd "$(dirname "$0")/../"
dir_root="$(pwd)"

# import settings
. "${dir_root}/info.cfg"

sed -i -e "s|	VERSION .*$|	VERSION ${VERSION}|" -e "s|set(PROJECT_PY_VERSION.*)|set(PROJECT_PY_VERSION ${PYVERSION})|" "${dir_root}/CMakeLists.txt"
sed -i -e "s|^version =.*$|version = '${PYVERSION}'|" "${dir_root}/src/bin2header.py"
sed -i -e "s|version =.*;|version = \"${VERSION}\";|" "${dir_root}/src/bin2header.cpp"
sed -i -e "s|Latest release: \[v.*\]\(.*\)/tag/v.*)|Latest release: \[v${VERSION}\]\1/tag/v${VERSION})|" "${dir_root}/README.md"
sed -i -e "s|Latest Python release: \[v.*\]\(.*\)/tag/v.*)|Latest Python release: \[v${PYVERSION}\]\1/tag/v${PYVERSION})|" "${dir_root}/README.md"
sed -i -e "s|\"bin2header-.*\"|\"bin2header-${VERSION}\"|" "${dir_root}/man/bin2header.1"

# check for Python executable
have_python=0
which python > /dev/null 2>&1
if test $? -eq 0; then
	have_python=1
fi

if test ${have_python} -gt 0; then
	# run script to update copyright year
	python "scripts/update_copyright.py"

	# run script to set changelog entry labeled "next" to current version
	python "scripts/update_version.py"
else
	echo "Python not found, not updating copyright year"
	echo "Python not found, not updating changelog version"
fi

# update HTML manpage for webpage
which pandoc > /dev/null 2>&1
if test $? -eq 0; then
	#pandoc --eol=lf -r man -w gfm -o docs/reference/bin2header.1.md man/bin2header.1
	pandoc -s --eol=lf -r man -w html5 -o docs/reference/bin2header.1.html man/bin2header.1
fi
