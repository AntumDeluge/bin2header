#!/bin/sh

# Script for creating a distributable archive

DIR_ROOT="$(dirname $0)"
cd "${DIR_ROOT}"
DIR_ROOT="$(pwd)"

# import settings
. "${DIR_ROOT}/info.cfg"

STAGE="${NAME}-${VERSION}"
DIRS="docs man src"
FILES="CMakeLists.txt configure dist.sh version.sh info.cfg LICENSE.txt README.md"

# Ensure creation of fresh staging directory & dist package
if [ -d "${STAGE}" ]; then
	rm -rf "${STAGE}"
fi
if [ -f "${STAGE}.tar.xz" ]; then
	rm -f "${STAGE}.tar.xz"
fi

echo "Staging files ..."
mkdir -p "${STAGE}"
cp -R ${DIRS} "${STAGE}"
cp ${FILES} "${STAGE}"
echo "Creating distribution archive: ${STAGE}.tar.xz ..."
tar -cJf "${STAGE}.tar.xz" "${STAGE}"
echo "Cleaning staging directory ..."
rm -rf "${STAGE}"
echo "Done!"
