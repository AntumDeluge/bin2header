#!/usr/bin/env bash

DIR_ROOT="$(dirname "$0")"
cd "${DIR_ROOT}"
DIR_ROOT="$(pwd)"

# import settings
. "${DIR_ROOT}/info.cfg"

sed -i -e "s|	VERSION .*$|	VERSION ${VERSION}|" -e "s|set(PROJECT_PY_VERSION.*)|set(PROJECT_PY_VERSION ${PYVERSION})|" "${DIR_ROOT}/CMakeLists.txt"
sed -i -e "s|^version =.*$|version = '${PYVERSION}'|" "${DIR_ROOT}/src/bin2header.py"
sed -i -e "s|version =.*;|version = \"${VERSION}\";|" "${DIR_ROOT}/src/bin2header.cpp"
sed -i -e "s|Latest release: \[v.*\]\(.*\)/tag/v.*)|Latest release: \[v${VERSION}\]\1/tag/v${VERSION})|" "${DIR_ROOT}/README.md"
sed -i -e "s|Latest Python release: \[v.*\]\(.*\)/tag/v.*)|Latest Python release: \[v${PYVERSION}\]\1/tag/v${PYVERSION})|" "${DIR_ROOT}/README.md"
sed -i -e "s|\"bin2header-.*\"|\"bin2header-${VERSION}\"|" "${DIR_ROOT}/man/bin2header.1"
