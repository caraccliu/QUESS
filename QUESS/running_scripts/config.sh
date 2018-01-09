
# Creates directories and setting files
./create_dir.sh
# Downloading sratoolkit to download programs
# required for dl_fna.sh to function
./dl_progs.sh 13 #SRATOOLKIT
./dl_fna.sh
./dl_miseq.sh data/datasets/Miseq
./dl_hiseq.sh data/datasets/Hiseq
./dl_nextseq.sh data/datasets/NextSeq
./check_data.sh


#list of programs to download, user can comment out if they do not want to evaluate
./dl_progs.sh 1 #musket
./dl_progs.sh 2 #ACE
./dl_progs.sh 4 #RACER
#./dl_progs.sh 5 #QUESS
./dl_progs.sh 6 #BLESS 2
./dl_progs.sh 7 #Blue
./dl_progs.sh 8 #Lighter
./dl_progs.sh 9 #BFC
./dl_progs.sh 10 #SGA
./dl_progs.sh 11 #Karect
./dl_progs.sh 12 #readSearch/kmerSearch
./check_progs.sh
