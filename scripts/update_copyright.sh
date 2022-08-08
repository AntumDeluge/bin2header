#!/usr/bin/env bash

test -x "${dir_root}"
res=$?

# only change to root dir if this file was not imported
if test ${res} -ne 0; then
	cd "$(dirname $0)/../"
	dir_root="$(pwd)"
fi

year="$(date +"%Y")"

update_copyright_files=()
for f in $(grep -rl --exclude="cxxopts.hpp" "Copyright \(©\|(C)\|{}\)" src/); do
	update_copyright_files+=("${f}")
done

for F in ${update_copyright_files[@]}; do
	sed -i \
		-e "s|Copyright © [^ ]*|Copyright © 2017-${year}|g" \
		-e "s|Copyright (C) [^ ]*|Copyright (C) 2017-${year}|g" \
		-e "s|Copyright {} [^ ]*|Copyright {} 2017-${year}|g" \
		"${F}"
	echo "Updated copyright year in ${F}"
done
