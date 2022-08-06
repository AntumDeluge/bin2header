#!/usr/bin/env bash

cd "$(dirname $0)"

mkdir "new/"

compare() {
	diff -q "orig/flower.$1.h" "new/flower.$1.h"
	if test $? -gt 0; then
		exit 1
	fi
}

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
