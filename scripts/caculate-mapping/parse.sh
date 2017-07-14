#!/bin/bash
FILE_NAME=mapping.txt 
OUT_FILE_NAME=12-core-32k-page-mapping.txt
cat $FILE_NAME | grep -P 0x[0-9a-f]{5}[0,8]0{3} | awk '{printf "%s%s\t%s%s\t%s%s\tcolor:%s\n", $1, $2, $7, $8%4, $9, $10, $8%4*4+$10}' > $OUT_FILE_NAME
