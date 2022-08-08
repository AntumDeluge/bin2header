#!/usr/bin/env bash

# Script for creating a distributable archive

cd "$(dirname "$0")/../"
dir_root="$(pwd)"

# import settings
. "${dir_root}/info.cfg"

STAGE="${NAME}-${VERSION}"

# Ensure creation of fresh staging directory & dist package
if [ -d "${STAGE}" ]; then
	rm -rf "${STAGE}"
fi
if [ -f "${STAGE}.tar.xz" ]; then
	rm -f "${STAGE}.tar.xz"
fi

echo "Staging files ..."
mkdir -p "${STAGE}"
cp -R ${DISTDIRS} "${STAGE}"
cp ${DISTFILES} "${STAGE}"
echo "Creating distribution archive: ${STAGE}.tar.xz ..."
tar -cJf "${STAGE}.tar.xz" "${STAGE}"
echo "Cleaning staging directory ..."
rm -rf "${STAGE}"
echo "Done!"
