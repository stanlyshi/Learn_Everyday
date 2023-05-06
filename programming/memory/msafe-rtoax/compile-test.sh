#!/bin/bash

rm -f *.out

for file in `ls test_*`
do 
	echo "Compile $file -> ${file%.*}.out"
	gcc $file msafe.c -pthread -lrt -I./ -o ${file%.*}.out -w 
done



