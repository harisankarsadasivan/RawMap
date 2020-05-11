#!/bin/bash
rm -rf ilog train_output;
#find /y/hariss/FAST5/FAST5/zymo/c -type f | shuf >z;
#find /y/hariss/FAST5/FAST5/hela/c -type f | shuf >h;
#
#tail -n8000 z > 3.txt
#tail -n8000 h > 4.txt


#for ((i=4500;i<=6500;i=i+200)); do
	i=6500;
	cat z| head -n $i > 1.txt;
	cat h| head -n $i > 2.txt;
	./RawMap train 1.txt 2.txt;
	var=$(./RawMap test 3.txt 4.txt  | grep "Correct classification" | awk -F':' '{print $2}' | awk -F'%' '{print $1}'| awk -F'(' '{print $2}' );
	echo $var >>train_output;
	echo $i >>ilog;
#done
