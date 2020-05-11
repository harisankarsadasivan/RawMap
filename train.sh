#!/bin/bash
rm -rf ilog train_output;
#find /y/hariss/FAST5/FAST5/zymo/c -type f | shuf >z;
#find /y/hariss/FAST5/FAST5/hela/c -type f | shuf >h;
#
#tail -n8000 z > 3.txt
#tail -n8000 h > 4.txt


#for ((i=5500;i<=6500;i=i+100)); do
i=6000;
	cat zymo_c| head -n $i > 1.txt;
	cat hela_c| head -n $i > 2.txt;
	./RawMap train 1.txt 2.txt;
	var=$(./RawMap test 3.txt 4.txt model.data | grep "Correct classification" | awk -F':' '{print $2}' | awk -F'%' '{print $1}'| awk -F'(' '{print $2}' );
	echo $var >>train_output;
	echo $i >>ilog;
#done
