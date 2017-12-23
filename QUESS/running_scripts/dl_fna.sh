
cd data/genome

#Miseq Datasets

#M1 /H5
wget ftp://ftp.ncbi.nlm.nih.gov/genomes/all/GCF/000/025/425/GCF_000025425.1_ASM2542v1/GCF_000025425.1_ASM2542v1_genomic.fna.gz
gunzip GCF_000025425.1_ASM2542v1_genomic.fna.gz
ID=$(grep "complete genome" GCF_000025425.1_ASM2542v1_genomic.fna | cut -d">" -f2 | cut -d" " -f1)
mv GCF_000009085.1_ASM908v1_genomic.fna  ${ID}.fna

#M2 / H4 / N2
wget ftp://ftp.ncbi.nlm.nih.gov/genomes/all/GCF/000/005/845/GCF_000005845.2_ASM584v2/GCF_000005845.2_ASM584v2_genomic.fna.gz
gunzip GCF_000005845.2_ASM584v2_genomic.fna.gz
ID=$(grep "complete genome" GCF_000005845.2_ASM584v2_genomic.fna | cut -d">" -f2 | cut -d" " -f1)
mv GCF_000005845.2_ASM584v2_genomic.fna  ${ID}.fna

#M3 / H3
wget ftp://ftp.ncbi.nlm.nih.gov/genomes/all/GCF/000/007/045/GCF_000007045.1_ASM704v1/GCF_000007045.1_ASM704v1_genomic.fna.gz
gunzip GCF_000007045.1_ASM704v1_genomic.fna.gz
ID=$(grep "complete genome" GCF_000007045.1_ASM704v1_genomic.fna | cut -d">" -f2 | cut -d" " -f1)
mv GCF_000007045.1_ASM704v1_genomic.fna  ${ID}.fna

#M4 / N3
wget ftp://ftp.ncbi.nlm.nih.gov/genomes/all/GCF/000/196/035/GCF_000196035.1_ASM19603v1/GCF_000196035.1_ASM19603v1_genomic.fna.gz
gunzip GCF_000196035.1_ASM19603v1_genomic.fna.gz
ID=$(grep "complete genome" GCF_000196035.1_ASM19603v1_genomic.fna | cut -d">" -f2 | cut -d" " -f1)
mv GCF_000196035.1_ASM19603v1_genomic.fna  ${ID}.fna

#M5 / N5 #AE006468.2 is the same
wget ftp://ftp.ncbi.nlm.nih.gov/genomes/all/GCF/000/006/945/GCF_000006945.2_ASM694v2/GCF_000006945.2_ASM694v2_genomic.fna.gz
gunzip GCF_000006945.2_ASM694v2_genomic.fna.gz
mv GCF_000006945.2_ASM694v2_genomic.fna  ${ID}.fna

#M6 / H1 / N1
wget ftp://ftp.ncbi.nlm.nih.gov/genomes/all/GCF/000/013/425/GCF_000013425.1_ASM1342v1/GCF_000013425.1_ASM1342v1_genomic.fna.gz
gunzip GCF_000013425.1_ASM1342v1_genomic.fna.gz
ID=$(grep "complete genome" GCF_000013425.1_ASM1342v1_genomic.fna | cut -d">" -f2 | cut -d" " -f1)
mv GCF_000013425.1_ASM1342v1_genomic.fna  ${ID}.fna

#M7
wget ftp://ftp.ncbi.nlm.nih.gov/genomes/all/GCF/000/195/955/GCF_000195955.2_ASM19595v2/GCF_000195955.2_ASM19595v2_genomic.fna.gz
gunzip GCF_000195955.2_ASM19595v2_genomic.fna.gz
ID=$(grep "complete genome" GCF_000195955.2_ASM19595v2_genomic.fna | cut -d">" -f2 | cut -d" " -f1)
mv GCF_000195955.2_ASM19595v2_genomic.fna  ${ID}.fna

#M8
wget ftp://ftp.ncbi.nlm.nih.gov/genomes/all/GCF/000/006/745/GCF_000006745.1_ASM674v1/GCF_000006745.1_ASM674v1_genomic.fna.gz
gunzip GCF_000006745.1_ASM674v1_genomic.fna.gz
ID=$(grep "complete" GCF_000006745.1_ASM674v1_genomic.fna | cut -d">" -f2 | cut -d" " -f1 | head -n 1)
mv GCF_000006745.1_ASM674v1_genomic.fna  ${ID}.fna

#M9 / N4
wget ftp://ftp.ncbi.nlm.nih.gov/genomes/all/GCF/000/146/045/GCF_000146045.2_R64/GCF_000146045.2_R64_genomic.fna.gz
gunzip GCF_000146045.2_R64_genomic.fna.gz
ID=$(grep "complete genome" GCF_000146045.2_R64_genomic.fna | cut -d">" -f2 | cut -d" " -f1)
mv GCF_000146045.2_R64_genomic.fna  ${ID}.fna

#M10 / N6
wget ftp://ftp.ncbi.nlm.nih.gov/genomes/all/GCF/000/240/185/GCF_000240185.1_ASM24018v2/GCF_000240185.1_ASM24018v2_genomic.fna.gz
gunzip GCF_000240185.1_ASM24018v2_genomic.fna.gz
ID=$(grep "complete genome" GCF_000240185.1_ASM24018v2_genomic.fna | cut -d">" -f2 | cut -d" " -f1)
mv GCF_000240185.1_ASM24018v2_genomic.fna  ${ID}.fna

#Hiseq Datasets (missing sets are found above)

#H2
wget ftp://ftp.ncbi.nlm.nih.gov/genomes/all/GCF/000/006/925/GCF_000006925.2_ASM692v2/GCF_000006925.2_ASM692v2_genomic.fna.gz
gunzip GCF_000006925.2_ASM692v2_genomic.fna.gz
ID=$(grep "complete genome" GCF_000006925.2_ASM692v2_genomic.fna | cut -d">" -f2 | cut -d" " -f1)
mv GCF_000006925.2_ASM692v2_genomic.fna  ${ID}.fna

#H6
wget ftp://ftp.ncbi.nlm.nih.gov/genomes/all/GCF/000/001/735/GCF_000001735.3_TAIR10/GCF_000001735.3_TAIR10_genomic.fna.gz
gunzip GCF_000001735.3_TAIR10_genomic.fna.gz
ID=$(grep "sequence" GCF_000001735.3_TAIR10_genomic.fna | cut -d">" -f2 | cut -d" " -f1 | head -n 1)
mv GCF_000001735.3_TAIR10_genomic.fna  ${ID}.fna

#H7
wget ftp://ftp.ncbi.nlm.nih.gov/genomes/all/GCF/000/002/985/GCF_000002985.6_WBcel235/GCF_000002985.6_WBcel235_genomic.fna.gz
gunzip GCF_000002985.6_WBcel235_genomic.fna.gz
ID=$(grep ">" GCF_000002985.6_WBcel235_genomic.fna | cut -d">" -f2 | cut -d" " -f1 | head -n 1)
mv GCF_000002985.6_WBcel235_genomic.fna  ${ID}.fna

#H8 / N7
wget ftp://ftp.ncbi.nlm.nih.gov/genomes/all/GCF/000/001/215/GCF_000001215.4_Release_6_plus_ISO1_MT/GCF_000001215.4_Release_6_plus_ISO1_MT_genomic.fna.gz
gunzip GCF_000001215.4_Release_6_plus_ISO1_MT_genomic.fna.gz
ID=$(grep ">" GCF_000001215.4_Release_6_plus_ISO1_MT_genomic.fna | cut -d">" -f2 | cut -d" " -f1 | head -n 1)
mv GCF_000001215.4_Release_6_plus_ISO1_MT_genomic.fna  ${ID}.fna

#H9
wget ftp://ftp.ncbi.nlm.nih.gov/genomes/all/GCF/000/001/405/GCF_000001405.37_GRCh38.p11/GCF_000001405.37_GRCh38.p11_genomic.fna.gz
gunzip GCF_000001405.37_GRCh38.p11_genomic.fna.gz
ID=$(grep ">" GCF_000001405.37_GRCh38.p11_genomic.fna | cut -d">" -f2 | cut -d" " -f1 | head -n 1)
mv GCF_000001405.37_GRCh38.p11_genomic.fna  ${ID}.fna

#H10
wget ftp://ftp.ncbi.nlm.nih.gov/genomes/all/GCF/000/001/635/GCF_000001635.26_GRCm38.p6/GCF_000001635.26_GRCm38.p6_genomic.fna.gz
gunzip GCF_000001635.26_GRCm38.p6_genomic.fna.gz
ID=$(grep ">" GCF_000001635.26_GRCm38.p6_genomic.fna | cut -d">" -f2 | cut -d" " -f1 | head -n 1)
mv GCF_000001635.26_GRCm38.p6_genomic.fna  ${ID}.fna


#Nextseq Datasets (all sets are found above)


cd ../..
