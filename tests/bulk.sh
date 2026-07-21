#!/bin/bash
for dir in tests/nlsandler/chapter_1/*; do
	echo Directory :: $dir
		for file in $dir/*; do
			echo File :: "$(basename $file)"
			cat $file
			echo ; echo ;
			./bin/micro_cc $file
			echo ; echo ; echo
		done
done
