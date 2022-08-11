#!/usr/bin/env bash

cd "$(dirname $0)"

dir_out="out/"
mkdir "{dir_out}"

check_result() {
	if test $1 -gt 0; then
		exit 1
	fi
}

compare() {
	diff -q "orig/flower.$1.h" "${dir_out}/flower.$1.h"
	check_result $?
}

./bin2header -h
check_result $?

./bin2header -v
check_result $?

./bin2header -s 1024 -o "${dir_out}/out.h" "flower.png"
check_result $?

./bin2header "flower.png"
diff -q "flower.png.h" "orig/flower.default.h"
check_result $?

./bin2header -o "${dir_out}/flower.default.h" "flower.png"
compare "default"

./bin2header --eol "cr" -o "${dir_out}/flower.cr.h" "flower.png"
compare "cr"

./bin2header --eol "crlf" -o "${dir_out}/flower.crlf.h" "flower.png"
compare "crlf"

./bin2header -f 3 -l 5 -o "${dir_out}/flower.truncate.h" "flower.png"
compare "truncate"

./bin2header -p 16 -o "${dir_out}/flower.16.h" "flower.png"
compare "16"

./bin2header -p 32 -o "${dir_out}/flower.32.h" "flower.png"
compare "32"

./bin2header -p 32 -e -o "${dir_out}/flower.32be.h" "flower.png"
compare "32be"

./bin2header -n "rock" -o "${dir_out}/flower.rock.h" "flower.png"
compare "rock"

./bin2header -c -o "${dir_out}/flower.comment.h" "flower.png"
compare "comment"

./bin2header -d 4 -o "${dir_out}/flower.nbdata.h" "flower.png"
compare "nbdata"

./bin2header --stdvector -o "${dir_out}/flower.vector.h" "flower.png"
compare "vector"


echo -e "\nChecking header data to image copy export ..."
g++ export_flower.cpp -o export_flower
./export_flower
diff -q flower.png flower_copy.png
check_result $?
