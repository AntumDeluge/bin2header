#!/usr/bin/env bash

VER=0.1.2

DIR_ROOT="$(dirname "$0")"
cd "${DIR_ROOT}"
DIR_ROOT="$(pwd)"

sed -i -e "s|VERSION=.*$|VERSION=${VER}|" "${DIR_ROOT}/dist.sh"
sed -i -e "s|	VERSION .*$|	VERSION ${VER}|" "${DIR_ROOT}/CMakeLists.txt"
sed -i -e "s|^version =.*$|version = '${VER}'|" "${DIR_ROOT}/src/bin2header.py"
sed -i -e "s|version =.*;|version = \"${VER}\";|" "${DIR_ROOT}/src/bin2header.cpp"
