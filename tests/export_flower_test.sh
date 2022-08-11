#!/usr/bin/bash


cd "$(dirname $0)"


echo -e "\nChecking header data to image copy export ..."
g++ export_flower.cpp -o export_flower
./export_flower
diff -q flower.png flower_copy.png
exit $?
