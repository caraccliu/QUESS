#/bin/sh

rm -f ./extracted/*txt*

for i in *rOutput.txt 
do
    if test -f "$i" 
    then
	tail -n 53 $i > ${i}.tail
       ./grep_r.sh ${i}.tail
	rm ${i}.tail
    fi
done

for i in *kOutput.txt 
do
    if test -f "$i" 
    then
       ./grep_k.sh $i
    fi
done
cd extracted
./merge_sim.sh
cd ..
./export.sh $1
