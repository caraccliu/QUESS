#usage: ./dl_fastq_slow.sh <directory> <accession id> <ref_genome> <estimated size> <location of .sra>
# requires sratoolkit to exist // refer to config.sh
# #5 will be by default /home/<user>/ncbi/public/sra/<accession id>.sra
# If one has their own datasets to use, you can create a config file under the
# default runlist/ folder that contains the lines:
# <Accession_ID>
# <Estimated_Size>
# <Ref_Genome>
#

echo "Downloading Accession = ${2} to ${1}"
sratoolkit.2.8.2-1-ubuntu64/bin/prefetch.2 --max-size=60G $2
sratoolkit.2.8.2-1-ubuntu64/bin/fastq-dump.2 --split-files --outdir $1 $5
echo "Download complete!"
cp ${1}/${2}_1.fastq ${1}/${2}.fastq
cat ${1}/${2}_2.fastq >> ${1}/${2}.fastq
echo ${2} > runlist/${2}.config
echo ${4} >> runlist/${2}.config
echo ${3} >> runlist/${2}.config
chmod 771 runlist/${2}.config
echo "Created config file!"
