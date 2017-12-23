
DIR=$(pwd)/data
MISEQ=$DIR/datasets/Miseq
HISEQ=$DIR/datasets/Hiseq
NEXTSEQ=$DIR/datasets/Nextseq
FLAG=0

echo "Checking dataset downloads"

MISEQ_ID[1]=SRR5230111
MISEQ_ID[2]=SRR5075712
MISEQ_ID[3]=SRR4417428
MISEQ_ID[4]=SRR5181392
MISEQ_ID[5]=SRR5164004
MISEQ_ID[6]=SRR5110368
MISEQ_ID[7]=SRR4423181
MISEQ_ID[8]=ERR1485307
MISEQ_ID[9]=SRR2317595
MISEQ_ID[10]=SRR5146462

MISEQ_SIZE[1]=3797840
MISEQ_SIZE[2]=4956704
MISEQ_SIZE[3]=4151656
MISEQ_SIZE[4]=6357592
MISEQ_SIZE[5]=8238664
MISEQ_SIZE[6]=9036824
MISEQ_SIZE[7]=12133640
MISEQ_SIZE[8]=13364744
MISEQ_SIZE[9]=13545000
MISEQ_SIZE[10]=10314568

for i in {1..10}
do

if [ -f  $MISEQ/${MISEQ_ID[$i]}.fastq ] && [ -f  $MISEQ/${MISEQ_ID[$i]}_1.fastq ] && [ -f  $MISEQ/${MISEQ_ID[$i]}_2.fastq ]
then
	if [ $(wc -l $MISEQ/${MISEQ_ID[$i]}.fastq | cut -d" " -f1 ) -ne ${MISEQ_SIZE[$i]} ]
	then
		FLAG=$((FLAG+1))	
		echo "$MISEQ/${MISEQ_ID[$i]}.fastq was imported incorrectly, has different number of lines than expected"
	fi
	if [ $(wc -l $MISEQ/${MISEQ_ID[$i]}_1.fastq | cut -d" " -f1 ) -ne $(expr ${MISEQ_SIZE[$i]} / 2)  ]
	then
                FLAG=$((FLAG+1))
		echo "$MISEQ/${MISEQ_ID[$i]}_1.fastq was imported incorrectly, has different number of lines than expected"
	fi
	if [ $(wc -l $MISEQ/${MISEQ_ID[$i]}_2.fastq | cut -d" " -f1 ) -ne $(expr ${MISEQ_SIZE[$i]} / 2)  ]
	then
                FLAG=$((FLAG+1))
        	echo "$MISEQ/${MISEQ_ID[$i]}_2.fastq was imported incorrectly, has different number of lines than expected" 
	fi
else
        if [ ! -f $MISEQ/${MISEQ_ID[$i]}.fastq ]
        then
                FLAG=$((FLAG+1))
                echo "$MISEQ/${MISEQ_ID[$i]}.fastq does not exist!"
        fi
        if [ ! -f $MISEQ/${MISEQ_ID[$i]}_1.fastq ]
        then
                FLAG=$((FLAG+1))
                echo "$MISEQ/${MISEQ_ID[$i]}_1.fastq does not exist!"
        fi
        if [ ! -f $MISEQ/${MISEQ_ID[$i]}_2.fastq ]
        then
                FLAG=$((FLAG+1))
                echo "$MISEQ/${MISEQ_ID[$i]}_2.fastq does not exist!"
        fi
fi
done

echo "There are $FLAG download errors for MiSeq datasets"

HISEQ_ID[1]=SRR5226582
HISEQ_ID[2]=SRR5029830
HISEQ_ID[3]=ERR1764059
HISEQ_ID[4]=SRR5177719
HISEQ_ID[5]=SRR4451711
HISEQ_ID[6]=ERR1904924
HISEQ_ID[7]=ERR294487
HISEQ_ID[8]=SRR3939099
HISEQ_ID[9]=SRR5408851
HISEQ_ID[10]=DRR082909

HISEQ_SIZE[1]=33597776
HISEQ_SIZE[2]=313599376
HISEQ_SIZE[3]=311856776
HISEQ_SIZE[4]=322080000
HISEQ_SIZE[5]=322082352
HISEQ_SIZE[6]=3517365952
HISEQ_SIZE[7]=3522827880
HISEQ_SIZE[8]=31350990360
HISEQ_SIZE[9]=33809744408
HISEQ_SIZE[10]=33810596728
FLAG=0

for i in {1..10}
do

if [ -f  $HISEQ/${HISEQ_ID[$i]}.fastq ] && [ -f  $HISEQ/${HISEQ_ID[$i]}_1.fastq ] && [ -f  $HISEQ/${HISEQ_ID[$i]}_2.fastq ]
then
        if [ $(wc -l $HISEQ/${HISEQ_ID[$i]}.fastq | cut -d" " -f1 ) -ne ${HISEQ_SIZE[$i]} ]
        then
                FLAG=$((FLAG+1))
                echo "$HISEQ/${HISEQ_ID[$i]}.fastq was imported incorrectly, has different number of lines than expected"
        fi
        if [ $(wc -l $HISEQ/${HISEQ_ID[$i]}_1.fastq | cut -d" " -f1 ) -ne $(expr ${HISEQ_SIZE[$i]} / 2)  ]
        then
                FLAG=$((FLAG+1))
                echo "$HISEQ/${HISEQ_ID[$i]}_1.fastq was imported incorrectly, has different number of lines than expected"
        fi
        if [ $(wc -l $HISEQ/${HISEQ_ID[$i]}_2.fastq | cut -d" " -f1 ) -ne $(expr ${HISEQ_SIZE[$i]} / 2)  ]
        then
                FLAG=$((FLAG+1))
                echo "$HISEQ/${HISEQ_ID[$i]}_2.fastq was imported incorrectly, has different number of lines than expected"
        fi
else
        if [ ! -f $HISEQ/${HISEQ_ID[$i]}.fastq ]
        then
                FLAG=$((FLAG+1))
                echo "$HISEQ/${HISEQ_ID[$i]}.fastq does not exist!"
        fi
        if [ ! -f $HISEQ/${HISEQ_ID[$i]}_1.fastq ]
        then
                FLAG=$((FLAG+1))
                echo "$HISEQ/${HISEQ_ID[$i]}_1.fastq does not exist!"
        fi
        if [ ! -f $HISEQ/${HISEQ_ID[$i]}_2.fastq ]
        then
                FLAG=$((FLAG+1))
                echo "$HISEQ/${HISEQ_ID[$i]}_2.fastq does not exist!"
        fi
fi
done

echo "There are $FLAG download errors for HiSeq datasets"

NEXTSEQ_ID[1]=ERR1845153
NEXTSEQ_ID[2]=SRR5336197
NEXTSEQ_ID[3]=SRR5088142
NEXTSEQ_ID[4]=SRR5251561
NEXTSEQ_ID[5]=SRR5381280
NEXTSEQ_ID[6]=SRR5093762
NEXTSEQ_ID[7]=SRR3285619

NEXTSEQ_SIZE[1]=6700072
NEXTSEQ_SIZE[2]=11978208
NEXTSEQ_SIZE[3]=11005208
NEXTSEQ_SIZE[4]=53527416
NEXTSEQ_SIZE[5]=30983320
NEXTSEQ_SIZE[6]=40960512
NEXTSEQ_SIZE[7]=250188184
FLAG=0

for i in {1..7}
do

if [ -f  $NEXTSEQ/${NEXTSEQ_ID[$i]}.fastq ] && [ -f  $NEXTSEQ/${NEXTSEQ_ID[$i]}_1.fastq ] && [ -f  $NEXTSEQ/${NEXTSEQ_ID[$i]}_2.fastq ]
then
        if [ $(wc -l $NEXTSEQ/${NEXTSEQ_ID[$i]}.fastq | cut -d" " -f1 ) -ne ${NEXTSEQ_SIZE[$i]} ]
        then
                FLAG=$((FLAG+1))
                echo "$NEXTSEQ/${NEXTSEQ_ID[$i]}.fastq was imported incorrectly, has different number of lines than expected"
        fi
        if [ $(wc -l $NEXTSEQ/${NEXTSEQ_ID[$i]}_1.fastq | cut -d" " -f1 ) -ne $(expr ${NEXTSEQ_SIZE[$i]} / 2)  ]
        then
                FLAG=$((FLAG+1))
                echo "$NEXTSEQ/${NEXTSEQ_ID[$i]}_1.fastq was imported incorrectly, has different number of lines than expected"
        fi
        if [ $(wc -l $NEXTSEQ/${NEXTSEQ_ID[$i]}_2.fastq | cut -d" " -f1 ) -ne $(expr ${NEXTSEQ_SIZE[$i]} / 2)  ]
        then
                FLAG=$((FLAG+1))
                echo "$NEXTSEQ/${NEXTSEQ_ID[$i]}_2.fastq was imported incorrectly, has different number of lines than expected"
        fi
else
        if [ ! -f $NEXTSEQ/${NEXTSEQ_ID[$i]}.fastq ]
        then
                FLAG=$((FLAG+1))
                echo "$NEXTSEQ/${NEXTSEQ_ID[$i]}.fastq does not exist!"
        fi
        if [ ! -f $NEXTSEQ/${NEXTSEQ_ID[$i]}_1.fastq ]
        then
                FLAG=$((FLAG+1))
                echo "$NEXTSEQ/${NEXTSEQ_ID[$i]}_1.fastq does not exist!"
        fi
        if [ ! -f $NEXTSEQ/${NEXTSEQ_ID[$i]}_2.fastq ]
        then
                FLAG=$((FLAG+1))
                echo "$NEXTSEQ/${NEXTSEQ_ID[$i]}_2.fastq does not exist!"
        fi
fi
done

echo "There are $FLAG download errors for Nextseq datasets."
