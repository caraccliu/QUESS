------------------------------------------------------------------------
QUESS: QUality Error correction using Spaced Seeds

Developed by: Lucian Ilie and Stephen lu
------------------------------------------------------------------------


------------------------------------------------------------------------
Change History
------------------------------------------------------------------------
V1.0.1: 10/13/2017
First release

------------------------------------------------------------------------
Dependencies
------------------------------------------------------------------------


------------------------------------------------------------------------
Installation
------------------------------------------------------------------------
1. Enter directory of QUESS.cpp.
2. Type 'make QUESS'.


------------------------------------------------------------------------
Running QUESS
------------------------------------------------------------------------
./QUESS -i <input_file> -g <genome_size>

<input_file> -- input file containing fastq or fasta
<genome_size> -- approximate size of reference genome of fastq/fasta file

The output file will be in the directory of <input_file> and will be named <input_prefix>_QUESS_corrected.<ext>

------------------------------------------------------------------------
Bug Reports:
------------------------------------------------------------------------
Stephen Lu <slu93@uwo.ca>
