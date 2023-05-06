#!/bin/bash

rm -f *.out


for file in `ls test_*.c`
do 
	echo "Compile $file -> ${file%.*}.out"
	gcc $file -I./ cache.c -pthread -o ${file%.*}.out -w $* -DNDEBUG
done

