#usage:./sub_jobs.sh <ID> <TYPE> <TIME> <# THREADS> <MEMORY> <extra>
EXTRA=${6}
THREADS=${4}
TIME=${3}
MEM=${5}


#sqsub -q $EXTRA threaded -n $THREADS -r $TIME --mpp=${MEM} -o logs/${2}/musket_${1}.out ./script_run.sh $1 $2 1 1
#WAIT=$(sqjobs | tail -n 1 | cut -d" " -f1)
#sqsub -q $EXTRA threaded -n $THREADS -r $TIME -w $WAIT --mpp=${MEM} -o logs/${2}/musket_${1}.out ./script_run.sh $1 $2 1 2

#sqsub -q $EXTRA threaded -n $THREADS -r $TIME --mpp=${MEM} -o logs/${2}/ace_${1}.out ./script_run.sh $1 $2 2 1
#WAIT=$(sqjobs | tail -n 1 | cut -d" " -f1)
#sqsub -q $EXTRA threaded -n $THREADS -r $TIME -w $WAIT --mpp=${MEM} -o logs/${2}/ace_${1}.out ./script_run.sh $1 $2 2 2

#sqsub -q $EXTRA threaded -n $THREADS -r $TIME --mpp=${MEM} -o logs/${2}/ace_${1}.out ./script_run.sh $1 $2 3 1
#WAIT=$(sqjobs | tail -n 1 | cut -d" " -f1)
#sqsub -q $EXTRA threaded -n $THREADS -r $TIME -w $WAIT --mpp=${MEM} -o logs/${2}/ace_${1}.out ./script_run.sh $1 $2 3 2

#sqsub -q $EXTRA threaded -n $THREADS -r $TIME --mpp=${MEM} -o logs/${2}/racer_${1}.out ./script_run.sh $1 $2 4 1
#WAIT=$(sqjobs | tail -n 1 | cut -d" " -f1)
#sqsub -q $EXTRA threaded -n $THREADS -r $TIME -w $WAIT --mpp=${MEM} -o logs/${2}/racer_${1}.out ./script_run.sh $1 $2 4 2

sqsub -q $EXTRA threaded -n $THREADS -r $TIME  --mpp=${MEM} -o logs/${2}/quess_${1}.out ./script_run.sh $1 $2 5 1
WAIT=$(sqjobs | tail -n 1 | cut -d" " -f1)
sqsub -q $EXTRA threaded -n $THREADS -r $TIME -w $WAIT --mpp=${MEM} -o logs/${2}/quess_${1}.out ./script_run.sh $1 $2 5 2 

#)sqsub -q $EXTRA threaded -n $THREADS -r $TIME --mpp=${MEM} -o logs/${2}/bless_${1}.out ./script_run.sh $1 $2 6 1 
#WAIT=$(sqjobs | tail -n 1 | cut -d" " -f1)
#sqsub -q $EXTRA threaded -n $THREADS -r $TIME -w $WAIT --mpp=${MEM} -o logs/${2}/bless_${1}.out ./script_run.sh $1 $2 6 2

#sqsub -q $EXTRA threaded -n $THREADS -r $TIME --mpp=${MEM} -o logs/${2}/blue_${1}.out ./script_run.sh $1 $2 7 1
#WAIT=$(sqjobs | tail -n 1 | cut -d" " -f1)
#sqsub -q $EXTRA threaded -n $THREADS -r $TIME  --mpp=${MEM} -w $WAIT -o logs/${2}/blue_${1}.out ./script_run.sh $1 $2 7 2

#sqsub -q $EXTRA threaded -n $THREADS -r $TIME --mpp=${MEM} -o logs/${2}/lighter_${1}.out ./script_run.sh $1 $2 8 1
#WAIT=$(sqjobs | tail -n 1 | cut -d" " -f1)
#sqsub -q $EXTRA threaded -n $THREADS -r $TIME -w $WAIT --mpp=${MEM} -o logs/${2}/lighter_${1}.out ./script_run.sh $1 $2 8 2

#sqsub -q $EXTRA threaded -n $THREADS -r $TIME --mpp=${MEM} -o logs/${2}/bfc_${1}.out ./script_run.sh $1 $2 9 1
#WAIT=$(sqjobs | tail -n 1 | cut -d" " -f1)
#sqsub -q $EXTRA threaded -n $THREADS -r $TIME -w $WAIT --mpp=${MEM} -o logs/${2}/bfc_${1}.out ./script_run.sh $1 $2 9 2

#sqsub -q $EXTRA threaded -n $THREADS -r $TIME --mpp=${MEM} -o logs/${2}/sga_${1}.out ./script_run.sh $1 $2 10 1
#WAIT=$(sqjobs | tail -n 1 | cut -d" " -f1)
#sqsub -q $EXTRA threaded -n $THREADS -r $TIME -w $WAIT --mpp=${MEM} -o logs/${2}/sga_${1}.out ./script_run.sh $1 $2 10 2

#sqsub -q $EXTRA threaded -n $THREADS -r $TIME --mpp=${MEM} -o logs/${2}/karect_${1}.out ./script_run.sh $1 $2 11 1 
#WAIT=$(sqjobs | tail -n 1 | cut -d" " -f1)
#sqsub -q $EXTRA threaded -n $THREADS -r $TIME -w $WAIT --mpp=${MEM} -o logs/${2}/karect_${1}.out ./script_run.sh $1 $2 11 2



#usage:./sub_jobs.sh <ID> <TYPE>

#./script_run.sh $1 $2 1 1 >> logs/${2}/musket_${1}.out
#./script_run.sh $1 $2 1 2 >> logs/${2}/musket_${1}.out

#./script_run.sh $1 $2 2 1 >> logs/${2}/ace_${1}.out
#./script_run.sh $1 $2 2 2 >> logs/${2}/ace_${1}.out

#./script_run.sh $1 $2 3 1 >> logs/${2}/ace_${1}.out
#./script_run.sh $1 $2 3 2 >> logs/${2}/ace_${1}.out

#./script_run.sh $1 $2 4 1 >> logs/${2}/racer_${1}.out
#./script_run.sh $1 $2 4 2 >> logs/${2}/racer_${1}.out

#./script_run.sh $1 $2 5 1 >> logs/${2}/quess_${1}.out
#./script_run.sh $1 $2 5 2 >> logs/${2}/quess_${1}.out

#./script_run.sh $1 $2 6 1 >> logs/${2}/bless2_${1}.out
#./script_run.sh $1 $2 6 2 >> logs/${2}/bless2_${1}.out

#./script_run.sh $1 $2 7 1 >> logs/${2}/blue_${1}.out
#./script_run.sh $1 $2 7 2 >> logs/${2}/blue_${1}.out

#./script_run.sh $1 $2 8 1 >> logs/${2}/lighter_${1}.out
#./script_run.sh $1 $2 8 2 >> logs/${2}/lighter_${1}.out

#./script_run.sh $1 $2 9 1 >> logs/${2}/bfc_${1}.out
#./script_run.sh $1 $2 9 2 >> logs/${2}/bfc_${1}.out

#./script_run.sh $1 $2 10 1 >> logs/${2}/sga_${1}.out
#./script_run.sh $1 $2 10 2 >> logs/${2}/sga_${1}.out

#./script_run.sh $1 $2 11 1 >> logs/${2}/karect_${1}.out
#./script_run.sh $1 $2 11 2 >> logs/${2}/karect_${1}.out
