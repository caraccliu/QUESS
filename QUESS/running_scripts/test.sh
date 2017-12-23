
#usage: ./test.sh <1-Miseq 2-small Hiseq 3-large Hiseq 4-Nextseq>

if [ $1 -eq 1 ]
then
./sub_jobs.sh SRR5230111 Miseq 14d 12 250G  
./sub_jobs.sh SRR5075712 Miseq 14d 12 250G 
./sub_jobs.sh SRR4417428 Miseq 14d 12 250G 
./sub_jobs.sh SRR5181392 Miseq 14d 12 250G 
./sub_jobs.sh SRR5164004 Miseq 14d 12 250G 
./sub_jobs.sh SRR5110368 Miseq 14d 12 250G 
./sub_jobs.sh SRR4423181 Miseq 14d 12 250G 
./sub_jobs.sh ERR1485307 Miseq 14d 12 250G 
./sub_jobs.sh SRR2317595 Miseq 14d 12 250G 
./sub_jobs.sh SRR5146462 Miseq 14d 12 250G  

fi

if [ $1 -eq 2 ]
then
./sub_jobs.sh SRR5226582 Hiseq 14d 12 250G  
./sub_jobs.sh SRR5029830 Hiseq 14d 12 250G 
./sub_jobs.sh ERR1764059 Hiseq 14d 12 250G 
./sub_jobs.sh SRR5177719 Hiseq 14d 12 250G 
./sub_jobs.sh SRR4451711 Hiseq 14d 12 250G 
fi 

if [ $1 -eq 3 ]
then
./sub_jobs.sh ERR1904924 Hiseq 14d 12 250G 
./sub_jobs.sh SRR3939099 Hiseq 14d 12 250G 
./sub_jobs.sh ERR294487 Hiseq 14d 12 250G  
./sub_jobs.sh SRR5408851 Hiseq 14d 32 1000G  
./sub_jobs.sh DRR082909 Hiseq  14d 32 1000G  
fi
if [ $1 -eq 4 ]
then
./sub_jobs.sh ERR1845153 Nextseq 14d 12 250G 
./sub_jobs.sh SRR5336197 Nextseq 14d 12 250G 
./sub_jobs.sh SRR5088142 Nextseq 14d 12 250G 
./sub_jobs.sh SRR5251561 Nextseq 14d 12 250G 
./sub_jobs.sh SRR5381280 Nextseq 14d 12 250G 
./sub_jobs.sh SRR5093762 Nextseq 14d 12 250G 
./sub_jobs.sh SRR3285619 Nextseq 14d 12 250G 

fi

