#usage: ./script_run.sh <ID> <TYPE> <1-11> <1/2> 
#                        $1    $2     $3    $4     
# <1-11> 1 - musket    2 - ACE for Miseq   3 - ACE for Hiseq     4 - RACER     5 - QUESS 
#        6 - BLESS 2   7 - BLUE            8 - Lighter           9 - BFC      10 - SGA
#       11 - Karect
#
# <1-2> 1 - run program 2 - run evaluations
#
# Output files will be found under the output_tests/$TYPE/ folder of the main directory.
# By default, the main directory is the current one.
#
# To ensure proper evaluation, there must be runlist config files to run the programs.
# These are made by default if datasets are downloaded using the dl_fastq.sh script.
#
# It is assumed that .fna evaluation files are in the format 'REF_GENOME'.fna, where 
# 'REF_GENOME' is the filename of the reference genome.

#main directories
HOME=$(pwd)
GEN_DIR=$HOME/data/genome
TYPE=$2
IN=$HOME/data/datasets/$TYPE
OUT=$HOME/output_tests/$TYPE
ID=$(sed '1!d' $HOME/runlist/$1.config)
SIZE=$(sed '2!d' $HOME/runlist/$1.config)
GENOME=$(sed '3!d' $HOME/runlist/$1.config) 

#directories of programs
MUSKET_DIR=$HOME/musket-1.1
ACE_M_DIR=$HOME/ACE-miseq
ACE_DIR=$HOME/ACE-master
RACER_DIR=$HOME/RACER
QUESS_DIR=$HOME/QUESS
BLESS_DIR=$HOME/v1p02   
BLUE_DIR=$HOME/Linux
BFC_DIR=$HOME/bfc-master
LIGHTER_DIR=$HOME/Lighter-master
SGA_DIR=$HOME/sga-master/src/SGA
KARECT_DIR=$HOME/karect-master
READ_DIR=$HOME/readSearch
KMER_DIR=$HOME/kmerSearch

if [[ $3 -eq 1 ]]
then
	if [[ $4 -eq 1 ]]
	then
	    sed '1!d' $GEN_DIR/${GENOME}.fna > $OUT/musket_${1}_rOutput.txt
		$MUSKET_DIR/musket -p 12 -k 21 536870912 $IN/${1}.fastq -o $IN/musket_${1}.fastq

	fi

	if [[ $4 -eq 2 ]]               
        then               
		$READ_DIR/readSearch $GEN_DIR/${GENOME}.fna  $IN/${1}.fastq $IN/musket_${1}.fastq >> $OUT/musket_${1}_rOutput.txt
		$KMER_DIR/kmerSearch 20 $GEN_DIR/${GENOME}.fna  $IN/${1}.fastq $IN/musket_${1}.fastq > $OUT/musket_${1}_kOutput.txt           
        rm $IN/musket_${1}.fastq
	fi
fi

if [[ $3 -eq 2 ]]
then
    if [[ $4 -eq 1 ]]
        then
            sed '1!d' $GEN_DIR/${GENOME}.fna > $OUT/ace_${1}_rOutput.txt
            $ACE_M_DIR/ace $SIZE $IN/${1}.fastq $IN/ace_${1}.fastq
         	
	fi

    if [[ $4 -eq 2 ]]               
        then             
            $READ_DIR/readSearch $GEN_DIR/${GENOME}.fna  $IN/${1}.fastq $IN/ace_${1}.fastq >> $OUT/ace_${1}_rOutput.txt
            $KMER_DIR/kmerSearch 20 $GEN_DIR/${GENOME}.fna  $IN/${1}.fastq $IN/ace_${1}.fastq > $OUT/ace_${1}_kOutput.txt
           	rm $IN/ace_${1}.fastq
	fi
fi

if [[ $3 -eq 3 ]]
then
    if [[ $4 -eq 1 ]]
        then
            sed '1!d' $GEN_DIR/${GENOME}.fna > $OUT/ace_${1}_rOutput.txt
            $ACE_DIR/ace $SIZE $IN/${1}.fastq $IN/ace_${1}.fastq
       	 fi

    if [[ $4 -eq 2 ]]               
        then             
            $READ_DIR/readSearch $GEN_DIR/${GENOME}.fna  $IN/${1}.fastq $IN/ace_${1}.fastq >> $OUT/ace_${1}_rOutput.txt
            $KMER_DIR/kmerSearch 20 $GEN_DIR/${GENOME}.fna  $IN/${1}.fastq $IN/ace_${1}.fastq > $OUT/ace_${1}_kOutput.txt
        	rm $IN/ace_${1}.fastq 
	fi
fi

if [[ $3 -eq 4 ]]
then
    if [[ $4 -eq 1 ]]
        then
                sed '1!d' $GEN_DIR/${GENOME}.fna > $OUT/racer_${1}_rOutput.txt
                $RACER_DIR/RACER $IN/${1}.fastq $IN/racer_${1}.fastq $SIZE
        	
	fi

    if [[ $4 -eq 2 ]]               
        then             
                $READ_DIR/readSearch $GEN_DIR/${GENOME}.fna  $IN/${1}.fastq $IN/racer_${1}.fastq >> $OUT/racer_${1}_rOutput.txt
                $KMER_DIR/kmerSearch 20 $GEN_DIR/${GENOME}.fna  $IN/${1}.fastq $IN/racer_${1}.fastq > $OUT/racer_${1}_kOutput.txt
        		rm $IN/racer_${1}.fastq         
	fi
fi

if [[ $3 -eq 5 ]]
then
    if [[ $4 -eq 1 ]]
        then
            sed '1!d' $GEN_DIR/${GENOME}.fna > $OUT/QUESS_${1}_rOutput.txt
            $QUESS_DIR/QUESS -i $IN/${1}.fastq -g $SIZE
 fi

    if [[ $4 -eq 2 ]]               
        then             
            $READ_DIR/readSearch $GEN_DIR/${GENOME}.fna  $IN/${1}.fastq $IN/${1}_QUESS_corrected.fastq >> $OUT/QUESS_${1}_rOutput.txt
	    $KMER_DIR/kmerSearch 20 $GEN_DIR/${GENOME}.fna  $IN/${1}.fastq $IN/${1}_QUESS_corrected.fastq > $OUT/QUESS_${1}_kOutput.txt
        	rm  $IN/${1}_QUESS_corrected.fastq 
	fi
fi

if [[ $3 -eq 6 ]]
then
    if [[ $4 -eq 1 ]]
        then
            sed '1!d' $GEN_DIR/${GENOME}.fna > $OUT/bless_${1}_rOutput.txt
            $BLESS_DIR/bless -read1 $IN/${1}_1.fastq -read2 $IN/${1}_2.fastq -kmerlength 25 -prefix $IN/bless_${1}
 fi
        if [[ $4 -eq 2 ]]               
        then             
            cat  $IN/bless_${1}.2.corrected.fastq >> $IN/bless_${1}.1.corrected.fastq 
		    $READ_DIR/readSearch $GEN_DIR/${GENOME}.fna  $IN/${1}.fastq $IN/bless_${1}.1.corrected.fastq >> $OUT/bless_${1}_rOutput.txt
            $KMER_DIR/kmerSearch 20 $GEN_DIR/${GENOME}.fna  $IN/${1}.fastq $IN/bless_${1}.1.corrected.fastq > $OUT/bless_${1}_kOutput.txt
            rm  $IN/bless_${1}*.corrected.fastq
	fi
fi

if [[ $3 -eq 7 ]]
then
    if [[ $4 -eq 1 ]]
        then
            sed '1!d' $GEN_DIR/${GENOME}.fna > $OUT/blue_${1}_rOutput.txt
            mono $BLUE_DIR/Tessel.exe -k 25 -g $SIZE blue_${1} $IN/${1}.fastq 
        	mono $BLUE_DIR/GenerateMerPairs.exe blue_${1}_25.cbt $IN/${1}.fastq
		    mono $BLUE_DIR/Blue.exe -m 50 blue_${1}_25.cbt $IN/${1}.fastq
 	fi

        if [[ $4 -eq 2 ]]               
        then             
            $READ_DIR/readSearch $GEN_DIR/${GENOME}.fna  $IN/${1}.fastq $IN/${1}_corrected_50.fastq >> $OUT/blue_${1}_rOutput.txt
            $KMER_DIR/kmerSearch 20 $GEN_DIR/${GENOME}.fna  $IN/${1}.fastq $IN/${1}_corrected_50.fastq > $OUT/blue_${1}_kOutput.txt
        	rm blue_${1}_*.*
		rm ${1}*.txt
	fi
fi

if [[ $3 -eq 8 ]]
then
    if [[ $4 -eq 1 ]]
        then
            sed '1!d' $GEN_DIR/${GENOME}.fna > $OUT/lighter_${1}_rOutput.txt
            $LIGHTER_DIR/lighter -r $IN/${1}_1.fastq -r $IN/${1}_2.fastq  -K 20 $SIZE -od $IN 
    fi
    if [[ $4 -eq 2 ]]
        then
            cat  $IN/${1}_2.cor.fq >> $IN/${1}_1.cor.fq
		    $READ_DIR/readSearch $GEN_DIR/${GENOME}.fna  $IN/${1}.fastq $IN/${1}_1.cor.fq >> $OUT/lighter_${1}_rOutput.txt
            $KMER_DIR/kmerSearch 20 $GEN_DIR/${GENOME}.fna  $IN/${1}.fastq $IN/${1}_1.cor.fq > $OUT/lighter_${1}_kOutput.txt
            rm  $IN/${1}_1.cor.fq
    fi
fi

if [[ $3 -eq 9 ]]
then
        if [[ $4 -eq 1 ]]
        then
            sed '1!d' $GEN_DIR/${GENOME}.fna > $OUT/bfc_${1}_rOutput.txt
            $BFC_DIR/bfc -s $SIZE -t16 $IN/${1}.fastq >  $IN/bfc_${1}.fastq
 fi
        if [[ $4 -eq 2 ]]
        then
            $READ_DIR/readSearch $GEN_DIR/${GENOME}.fna  $IN/${1}.fastq $IN/bfc_${1}.fastq >> $OUT/bfc_${1}_rOutput.txt
            $KMER_DIR/kmerSearch 20 $GEN_DIR/${GENOME}.fna  $IN/${1}.fastq $IN/bfc_${1}.fastq > $OUT/bfc_${1}_kOutput.txt
            rm $IN/bfc_${1}.fastq
        fi
fi

if [[ $3 -eq 10 ]]
then
    if [[ $4 -eq 1 ]]
        then
            sed '1!d' $GEN_DIR/${GENOME}.fna > $OUT/sga_${1}_rOutput.txt
		    $SGA_DIR/sga preprocess --pe-mode 2 --permute-ambiguous --no-primer-check -m 1 -o $IN/sga_${1}.fastq $IN/${1}.fastq 
		    $SGA_DIR/sga index -a ropebwt -t 12 $IN/sga_${1}.fastq  
		    $SGA_DIR/sga correct -k 41 -t 12 $IN/sga_${1}.fastq 
 fi
        if [[ $4 -eq 2 ]]
        then
            $READ_DIR/readSearch $GEN_DIR/${GENOME}.fna  $IN/${1}.fastq $HOME/sga_${1}.ec.fa >> $OUT/sga_${1}_rOutput.txt
            $KMER_DIR/kmerSearch 20 $GEN_DIR/${GENOME}.fna  $IN/${1}.fastq $HOME/sga_${1}.ec.fa > $OUT/sga_${1}_kOutput.txt
            rm $IN/sga_${1}*
		rm $HOME/temp_res_${1}*
		rm $HOME/sga_${1}*    
		rm $HOME/sga_${1}.ec.fa
        fi
fi

if [[ $3 -eq 11 ]]
then
    if [[ $4 -eq 1 ]]
        then
            sed '1!d' $GEN_DIR/${GENOME}.fna > $OUT/karect_${1}_rOutput.txt
		    $KARECT_DIR/karect -correct  -threads=12 -matchtype=hamming -celltype=diploid -inputfile=$IN/${1}_1.fastq -inputfile=$IN/${1}_2.fastq        
    fi
    if [[ $4 -eq 2 ]]
        then
		    cat $HOME/karect_${1}_2.fastq >> $HOME/karect_${1}_1.fastq
		    mv  $HOME/karect_${1}_1.fastq  $HOME/karect_${1}.fastq
            $READ_DIR/readSearch $GEN_DIR/${GENOME}.fna  $IN/${1}.fastq $HOME/karect_${1}.fastq >> $OUT/karect_${1}_rOutput.txt
            $KMER_DIR/kmerSearch 20 $GEN_DIR/${GENOME}.fna  $IN/${1}.fastq $HOME/karect_${1}.fastq > $OUT/karect_${1}_kOutput.txt
            rm $HOME/karect_${1}*.fastq
        fi
fi
	
