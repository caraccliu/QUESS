
DIR=$(pwd)
FLAG=0

echo "Checking program compilation completion"

if [ ! -f $DIR/musket-1.1/musket ]
then
       echo "Musket does not exist in its default directory!"
	FLAG=$((FLAG+1))
fi

if [ ! -f $DIR/ACE-master/ace ]
then
       echo "ACE does not exist in its default directory!"
        FLAG=$((FLAG+1))

fi

if [ ! -f $DIR/RACER/RACER ]
then
       echo "RACER does not exist in its default directory!"
        FLAG=$((FLAG+1))

fi

if [ ! -f $DIR/QUESS/QUESS ]
then
       echo "QUESS does not exist in its default directory!"
        FLAG=$((FLAG+1))

fi

if [ ! -f $DIR/v1p02/bless ]
then
       echo "BLESS 2 does not exist in its default directory!"
        FLAG=$((FLAG+1))

fi

if [ ! -f $DIR/Linux/Blue.exe ]
then
       echo "Blue does not exist in its default directory!"
        FLAG=$((FLAG+1))

fi

if [ ! -f $DIR/bfc-master/bfc ]
then
       echo "BFC does not exist in its default directory!"
        FLAG=$((FLAG+1))

fi

if [ ! -f $DIR/Lighter-master/lighter ]
then
       echo "Lighter does not exist in its default directory!"
        FLAG=$((FLAG+1))

fi

if [ ! -f $DIR/karect-master/karect ]
then
       echo "Karect does not exist in its default directory!"
        FLAG=$((FLAG+1))

fi

if [ ! -f $DIR/sga-master/src/SGA/sga ]
then
       echo "SGA does not exist in its default directory!"
        FLAG=$((FLAG+1))

fi

if [ ! -f $DIR/readSearch/readSearch ] &&  [ ! -f $DIR/kmerSearch/kmerSearch ] 
then
       echo "readSearch and/or kmerSearch does not exist in its default directory!"
        FLAG=$((FLAG+1))

fi

echo "There are $FLAG compilation errors of programs"
