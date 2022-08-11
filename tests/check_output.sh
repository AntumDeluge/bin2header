#!/usr/bin/env bash

cd "$(dirname $0)"

dir_out="out"
rm -rf "${dir_out}"
mkdir -p "${dir_out}"

check_result() {
	if test $1 -gt 0; then
		exit 1
	fi
}

compare() {
	diff -q "orig/flower.$1.h" "${dir_out}/flower.$1.h"
	check_result $?
}

execute() {
	echo -e "\nExecuting with params: $@"
	./bin2header $@
	check_result $?
}

execute -h
execute -v
execute -s 1024 -o "${dir_out}/out.h" "flower.png"

test -f "${dir_out}/out.h"
res=$?
if test ${res} -ne 0; then
	echo "ERROR: cannot find output \"${dir_out}/out.h\""
	exit ${res}
fi

execute "flower.png"
diff -q "flower.png.h" "orig/flower.default.h"
check_result $?

execute -o "${dir_out}/flower.default.h" "flower.png"
compare "default"

execute --eol cr -o "${dir_out}/flower.cr.h" "flower.png"
compare "cr"

execute --eol crlf -o "${dir_out}/flower.crlf.h" "flower.png"
compare "crlf"

execute -f 3 -l 5 -o "${dir_out}/flower.truncate.h" "flower.png"
compare "truncate"

execute -p 16 -o "${dir_out}/flower.16.h" "flower.png"
compare "16"

execute -p 32 -o "${dir_out}/flower.32.h" "flower.png"
compare "32"

execute -p 32 -e -o "${dir_out}/flower.32be.h" "flower.png"
compare "32be"

execute -n rock -o "${dir_out}/flower.rock.h" "flower.png"
compare "rock"

execute -c -o "${dir_out}/flower.comment.h" "flower.png"
compare "comment"

execute -d 4 -o "${dir_out}/flower.nbdata.h" "flower.png"
compare "nbdata"

execute --stdvector -o "${dir_out}/flower.vector.h" "flower.png"
compare "vector"
