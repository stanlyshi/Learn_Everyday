#!/bin/bash
# 荣涛 2021年1月27日

rm -f *.out

for file in `ls test-*`
do 
	echo "Compile $file -> ${file%.*}.out"
	gcc $file fastq.c -lcrypt -pthread -I./ -o ${file%.*}.out -w $*
done


