#usage:./dl_progs.sh <1-13> 
#       			   $1   
# <1-11> 1 - musket    2 - ACE for Miseq   4 - RACER     5 - QUESS     6 - BLESS 2   
#        7 - BLUE      8 - Lighter         9 - BFC      10 - Karect   11 - SGA   
#       12 - readSearch & kmerSearch      13 - SRATOOLKIT 


if [[ $1 -eq 1 ]]
then
	wget https://downloads.sourceforge.net/project/musket/musket-1.1.tar.gz
	tar -vxf musket-1.1.tar.gz
	rm musket-1.1.tar.gz

	cd musket-1.1
	module purge
	make
	module load user-environment/2.0.1	
	cd ..
fi

if [[ $1 -eq 2 ]]
then
	wget https://github.com/Sheikhizadeh/ACE/archive/master.zip
	unzip master.zip
	rm master.zip
	cp -r ACE-master ACE-miseq

	cd ACE-master
	sed -i '4s/251/151/' Makefile
	make
	cd ../

	cd ACE-miseq
	sed -i '4s/251/301/' Makefile
	make
	cd ../
fi

if [[ $1 -eq 4 ]]
then
	wget http://www.csd.uwo.ca/~ilie/RACER/racer.zip
	mkdir RACER
	mv racer.zip RACER
	cd RACER
	unzip racer.zip 
	mv Makefile_parallel Makefile
	make
	rm racer.zip
	cd ..
fi

if [[ $1 -eq 5 ]]
then
	echo ""
fi

if [[ $1 -eq 6 ]]
then
	wget https://downloads.sourceforge.net/project/bless-ec/bless.v1p02.tgz
	tar -vxf bless.v1p02.tgz
	rm bless.v1p02.tgz
	cd v1p02
	module purge
	module load gcc/4.9.2
	module load openmpi/gcc492-std/1.8.7	
	cd zlib
	./compile
	cd ..
	sed -i '9s/$(ZLIB)//g' Makefile
	make
	module purge
	module load user-environment/2.0.1
	cd ..
fi

if [[ $1 -eq 7 ]]
then
	wget http://bioinformatics.csiro.au/public/files/Linux-1.1.3.tar.gz
	tar -vxf Linux-1.1.3.tar.gz
	rm Linux-1.1.3.tar.gz
	cd Linux

	cd ..
fi

if [[ $1 -eq 8 ]]
then
	wget https://github.com/mourisl/Lighter/archive/master.zip
	unzip master.zip
	rm master.zip

	cd Lighter-master
	module purge
	make
	module load user-environment/2.0.1
	cd ..
fi

if [[ $1 -eq 9 ]]
then
	wget https://github.com/lh3/bfc/archive/master.zip
	unzip master.zip
	rm master.zip

	cd bfc-master
	make
	cd ..
fi

if [[ $1 -eq 10 ]]
then
	wget https://github.com/jts/sga/archive/master.zip
	unzip master.zip
	rm master.zip
	wget https://github.com/pezmaster31/bamtools/archive/master.zip
	unzip master.zip
	rm master.zip
	WORKING=$(pwd)
	module add cmake/3.4.3

	cd bamtools-master
	mkdir build
	cd build
	cmake -DCMAKE_INSTALL_PREFIX=${WORKING}/bamtools ..
	make install
	cd ../../ 
	cd sga-master/src
	./autogen.sh
	./configure --prefix=${WORKING}/sga-master/src/sga --with-bamtools=${WORKING}/bamtools
	make install
	cd ../../..

	############################################################################################################################################################################
fi
	
if [[ $1 -eq 11 ]]
then
        wget https://github.com/aminallam/karect/archive/master.zip
        unzip master.zip
        rm master.zip

        cd karect-master
        make
        cd ..
fi

if [[ $1 -eq 12 ]]
then
	wget http://www.csd.uwo.ca/~ilie/CorrectingIlluminaData/readSearch.zip
	unzip readSearch.zip
	rm -r __MACOSX/
	rm readSearch.zip
	cd readSearch
	make
	cd ..

	wget http://www.csd.uwo.ca/~ilie/CorrectingIlluminaData/kmerSearch.zip
	unzip kmerSearch.zip
	rm -r __MACOSX/
	rm kmerSearch.zip
	cd kmerSearch
	make
	cd ..
fi
	
if [[ $1 -eq 13 ]]
then
	wget https://ftp-trace.ncbi.nlm.nih.gov/sra/sdk/2.8.2-1/sratoolkit.2.8.2-1-ubuntu64.tar.gz
	tar -xvf sratoolkit.2.8.2-1-ubuntu64.tar.gz
	rm sratoolkit.2.8.2-1-ubuntu64.tar.gz
fi
	










	
	





