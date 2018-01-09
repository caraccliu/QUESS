mkdir ./data
mkdir ./data/genome

mkdir ./data/datasets
mkdir ./data/datasets/Miseq
mkdir ./data/datasets/Hiseq
mkdir ./data/datasets/Nextseq

mkdir ./output_tests
mkdir ./output_tests/Miseq
mkdir ./output_tests/Hiseq
mkdir ./output_tests/Nextseq
mkdir ./output_tests/Miseq/extracted
mkdir ./output_tests/Hiseq/extracted
mkdir ./output_tests/Nextseq/extracted

mkdir ./runlist
mkdir ./logs
mkdir ./logs/Miseq
mkdir ./logs/Hiseq
mkdir ./logs/Nextseq

cp grep_r.sh ./output_tests/Miseq
cp grep_k.sh ./output_tests/Miseq
cp rr.sh ./output_tests/Miseq
cp extract_all.sh ./output_tests/Miseq
cp tail.sh ./output_tests/Miseq
cp export.sh ./output_tests/Miseq
cp merge_sim.sh ./output_tests/Miseq/extracted

cp grep_r.sh ./output_tests/Hiseq
cp grep_k.sh ./output_tests/Hiseq
cp rr.sh ./output_tests/Hiseq
cp extract_all.sh ./output_tests/Hiseq
cp tail.sh ./output_tests/Hiseq
cp export.sh ./output_tests/Hiseq
cp merge_sim.sh ./output_tests/Hiseq/extracted

cp grep_r.sh ./output_tests/Nextseq
cp grep_k.sh ./output_tests/Nextseq
cp rr.sh ./output_tests/Nextseq
cp extract_all.sh ./output_tests/Nextseq
cp tail.sh ./output_tests/Nextseq
cp export.sh ./output_tests/Nextseq
cp merge_sim.sh ./output_tests/Nextseq/extracted

rm grep_r.sh
rm grep_k.sh
rm rr.sh
rm extract_all.sh
rm tail.sh
rm export.sh
rm merge_sim.sh


