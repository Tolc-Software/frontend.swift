#!/bin/sh
## Maintainer: Simon Rydell
## Date created: Feb 14, 2022
# Must be run from the same directory as the export_examples_sh file
# Must be run after all the tests have been run
# Outputs ./public/examples.md from ./examples/*.md

# Exit on any error
set -o errexit
# Print and exit on undefined variable
set -o nounset

# Exports some example code
# Parameter: language
# Returns: None
export_example() {
	TMPFILE="$(mktemp -t --suffix=.SUFFIX export_examples_sh.XXXXXX)"
	trap 'rm -f $TMPFILE' 0               # EXIT
	trap 'rm -f $TMPFILE; exit 1' 2       # INT
	trap 'rm -f $TMPFILE; exit 1' 1 15    # HUP TERM

	cp ./examples_$1.md.in "$TMPFILE"
	for e in examples/$1/*.md
	do
		cat "$e" >> "$TMPFILE"
	done

	mv "$TMPFILE" ./public/$1/examples.md
}

export_example objc
export_example swift
