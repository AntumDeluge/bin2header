#!/usr/bin/env bash

cd "$(dirname $0)"

mkdir "new/"

check_result() {
	if test $1 -gt 0; then
		exit 1
	fi
}

compare() {
	diff -q "orig/flower.$1.h" "new/flower.$1.h"
	check_result $?
}

./bin2header -h
check_result $?

./bin2header -v
check_result $?

./bin2header -s 1024 -o "new/out.h" "flower.png"
check_result $?

./bin2header "flower.png"
diff -q "flower.png.h" "orig/flower.default.h"
check_result $?

rm "flower.png.h"

./bin2header -o "new/flower.default.h" "flower.png"
compare "default"

./bin2header -f 3 -l 5 -o "new/flower.truncate.h" "flower.png"
compare "truncate"

./bin2header -p 16 -o "new/flower.16.h" "flower.png"
compare "16"

./bin2header -p 32 -o "new/flower.32.h" "flower.png"
compare "32"

./bin2header -p 32 -e -o "new/flower.32be.h" "flower.png"
compare "32be"

./bin2header -n "rock" -o "new/flower.rock.h" "flower.png"
compare "rock"

./bin2header -c -o "new/flower.comment.h" "flower.png"
compare "comment"

./bin2header -d 4 -o "new/flower.nbdata.h" "flower.png"
compare "nbdata"

./bin2header --stdvector -o "new/flower.vector.h" "flower.png"
compare "vector"
