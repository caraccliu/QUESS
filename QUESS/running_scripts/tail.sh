#/bin/sh

for i in *kOutput.txt
do
	tail -n 58 $i > temp2.txt
	cat temp2.txt > $i
	
done
