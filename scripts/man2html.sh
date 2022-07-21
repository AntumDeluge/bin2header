#!/usr/bin/env bash

# converts manpage documentation to HTML with pandoc

dir_scripts="$(dirname $0)"
cd "${dir_scripts}/../"
dir_root="$(pwd)"
infile="${dir_root}/man/bin2header.1"
outfile="${dir_root}/docs/reference/bin2header.1.html"

pandoc -s --from man --to html5 --eol lf < "${infile}" > "${outfile}"
