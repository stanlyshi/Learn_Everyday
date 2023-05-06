#!/bin/bash

rm -f *.out

for file in `ls test-*`
do 
	echo "Compile $file -> ${file%.*}.out"
	gcc $file -pthread -I./ -o ${file%.*}.out -w $*
done



