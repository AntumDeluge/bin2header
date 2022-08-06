#!/usr/bin/env bash

cd "$(dirname $0)"

mkdir "new/"

./bin2header -o "new/flower.default.h" "flower.png"
diff -q "orig/flower.default.h" "new/flower.default.h"
res=$?

if test ${res} -gt 0; then
	exit 1
fi
