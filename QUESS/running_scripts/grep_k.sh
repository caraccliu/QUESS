#!/bin/sh

grep -Eo '[-]?[0-9.,]*[e]?[-]?[0-9.,%]+' $1 > temp.txt
sed '12q;d' temp.txt >> extracted/$1_mod
sed -n '14,15 p' temp.txt >> extracted/$1_mod
sed -n '24,35 p' temp.txt >> extracted/$1_mod
rm temp.txt
