--------------------------------------------------------------------------
QUESS Working Scripts

Developed by: Lucian Ilie and Stephen lu
--------------------------------------------------------------------------

These scripts are intended to aid in evaluation of QUESS, and to document
the running parameters to replicate the results as seen.


--------------------------------------------------------------------------
Dependencies
--------------------------------------------------------------------------

The scripts are run on SHARCNET under the module user-environment/2.0.1. 
This by default has the modules: intel/12.1.3 and openmpi/intel/1.6.2.
However, this does not mean that all/any of the programs require compilation
by use of these modules, and if a program suggests or requires compilation 
with other specifications, those have taken precedent.

--------------------------------------------------------------------------
Installation and Running Parameters
--------------------------------------------------------------------------

1. Create a directory as to test the programs.
2. run ./config.sh
3. run ./test.sh 1 to test for MiSeq
4. run ./test.sh 2 and ./test.sh 3 to test for HiSeq
5. run ./test.sh 4 to test for NextSeq


--------------------------------------------------------------------------
Details
--------------------------------------------------------------------------

It is recommended that to test the programs in question, extract all the 
files into a directory and run ./config.sh. All output should be contained 
within this directory with the exception of NCBI-related output associated 
with SRAToolkit, which will be located by default at /home/user. 

To test the programs using SHARCNET, ./test.sh 1 will run all of the 
programs upon all of the MiSeq datasets, assuming that ./config.sh was 
successful. In addition, ./test.sh 2 and ./test.sh 3 will run the programs 
on HiSeq datasets, and ./test.sh 4 will run the programs on NextSeq datasets.
Adjustments can be made within the ./test.sh and ./sub_jobs.sh files to run 
specific programs on specific datasets.

To check a log of results, check the 'log/<Type>' directory. This will
contain the output of the programs, in addition to time and memory measures
as according to SHARCNET standards.

To check for results as evaluated by readSearch and kmerSearch, check the
'output_tests/<Type>' directory. This evaluations are contained within the
'<program>_<dataset_name>_<r/k>Output.txt', in where r/k denotes read or 
kmer Search. 

To export the results of the aforementioned evaluations, go to the directory
'output_tests/<Type>' and run extract_all.sh <outfile>. This will put align
all of the outputs into an easily legible file. Note that the helper files
are put into the correct place with 'create_dir.sh'.

**Important**
Note that Blue requires the package 'mono' to run, which is available under
the module 'nix' on SHARCNET. It is important to put the following under 
your bash profile to run Blue as a job SHARCNET:

module load nix
nix-env -iA nixpkgs.mono

--------------------------------------------------------------------------
Script Descriptions
--------------------------------------------------------------------------

The following are script files in order to help with accessing the program 
QUESS:

Core executables:

config.sh 	- the core script file that sets up the directories and 
			  necessary downloads to proceed in testing.

test.sh <1-4>	- queues multiple jobs in succession by calling 
		  'sub_jobs.sh' for multiple datasets for multiple programs.

sub_jobs.sh 	- submits jobs to SHARCNET. 
		- If you want to run the jobs on your system, there is a 
		  commented section at the bottom that can be modified to 
		  your system parameters.

extract_all.sh <outfile> - extracts all the relevant information from the
			   tested datasets into outfile.

Helper functions:

create_dir.sh 	- creates necessary organizational directories, and 
		  important files to extract results

dl_fastq.sh 	- script to download accession files from NCBI
		- note that if you exceed the default maximum space
		  in your home directory, you can clear the cache for 
		  previous data located defaultly at:
		  /home/<user>/ncbi/public/sra

dl_miseq.sh 	- script to mass download MiSeq datasets

dl_hiseq.sh 	- script to mass download HiSeq datasets
		- Note that some datasets (H6-H10) may be exceedingly 
		  large, so it may be a good idea to download them one at 
		  a time and test them.

dl_nextseq.sh - script to mass download NextSeq datasets 

script_run.sh - script containing running parameters for each program


--------------------------------------------------------------------------
Bug Reports:
--------------------------------------------------------------------------
Contact slu93@uwo.ca
