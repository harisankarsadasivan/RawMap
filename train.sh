
rm -rf ilog train_output
for ((i=5000;i<=8000;i=i+100)); do
	cat z| head -n $i > 1.txt
	cat h| head -n $i > 2.txt
	./RawMap train 1.txt 2.txt
	./RawMap test 3.txt 4.txt  | grep "Correct classification" | awk -F':' '{print $2}' | awk -F'%' '{print $1}'| awk -F'(' '{print $2}' >> train_output
	echo $i >>ilog
done
