#/bin/sh

rSUFF=rOutput.txt_mod
kSUFF=kOutput.txt_mod

for i in ./*$rSUFF
do
    if test -f "$i" 
    then
	cat ${i%$rSUFF}$kSUFF >> $i
	mv -f $i ${i%$rSUFF}_merged.txt
	rm ${i%$rSUFF}$kSUFF
   fi
done


