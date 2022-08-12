#!/usr/bin/env bash

# script for creating a distributable source package

cd "$(dirname "$0")/../"
dir_root="$(pwd)"

# import settings
. "${dir_root}/info.cfg"

stage="${NAME}-${VERSION}"
pkgname="${stage}"
if [ ! -x ${PKGREV} ] && [ ${PKGREV} -gt 0 ]; then
  pkgname="${pkgname}-${PKGREV}"
fi
pkgname="${pkgname}.tar.xz"

# Ensure creation of fresh staging directory & dist package
if [ -d "${stage}" ]; then
	rm -rf "${stage}"
fi
if [ -f "${pkgname}" ]; then
	rm -f "${pkgname}"
fi

echo "Staging files ..."
mkdir -p "${stage}"
cp -R ${DISTDIRS} "${stage}"
cp ${DISTFILES} "${stage}"
echo "Creating distribution archive: ${pkgname} ..."
tar -cJf "${pkgname}" "${stage}"
echo "Cleaning staging directory ..."
rm -rf "${stage}"
echo "Done!"
