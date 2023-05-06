#!/bin/bash

rm -f *.out


for file in `ls test_*.c`
do 
	echo "Compile $file -> ${file%.*}.out"
	gcc $file -I./ extstore.c -pthread -o ${file%.*}.out -w $* -std=c99
done

