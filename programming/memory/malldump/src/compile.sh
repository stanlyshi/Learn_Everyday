#!/bin/bash

gcc *.c -std=c99 -I . -DMP__OFFSET=0 -DMALLINFO_OFFSET=0 -DNARENAS_OFFSET=0 -o malldump.out

