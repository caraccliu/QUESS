#!/bin/sh



printf %b "RUN ID:" '\t' `date` '\t\t\t\t\t\t\t\t\t\t' "Read Accuracy" '\t\t\t\t\t\t\t' "Unique Reads" '\t\t\t\t\t' "20-mer" '\t\t\t' "Kmer Accuracy" '\t\t\t\t\t\t\t' "Kmer unique Reads"'\n' > $1
printf %b "Genome Ref" '\t' "Algorithm ID" '\t' "Ascension Number" '\t' "Genome Size" '\t' "Read Length" '\t' "Number of Reads" '\t' "Total Base Pairs" '\t' "Coverage" '\t' "Correct Bases" '\t' "% Correct" '\t' "Per Base Error" '\t' "TP" '\t' "FP" '\t' "FN" '\t' "TN" '\t' "Gain" '\t' "Sensitivity"  '\t' "Specificity" '\t' "TP" '\t' "FP" '\t' "FN" '\t' "TN" '\t' "Gain" '\t' "K-mers Found" '\t' "K-mers Max" '\t' "% Found" '\t' "TP" '\t' "FP" '\t' "FN" '\t' "TN" '\t' "Gain" '\t' "Sensitivity" '\t' "Specificity" '\t' "TP" '\t' "FP" '\t' "FN" '\t' "TN" '\t' "GAIN" '\n' >> $1  
for i in *rOutput.txt
do
	if test -f "$i"
	then 
		head -n 1 $i | tr -d '\n' >> $1
		asc=${i#*_}
		printf %b '\t'${i%_*_rOutput.txt}'\t' ${asc%_rOutput.txt} '\t'>> $1
		./rr.sh ./extracted/${i%rOutput.txt}_merged.txt $1
		printf %b "\n" >>$1
	fi
done
