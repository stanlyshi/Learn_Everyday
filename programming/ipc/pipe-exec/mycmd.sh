#!/bin/bash

echo "pipe-exec>"
while read -r CMD; do
	$CMD	
	echo "pipe-exec>"
done
