#!/bin/sh

grep -Eo '[-]?[0-9.,]*[e]?[-]?[0-9.,%]+' $1 > temp.txt
sed '3q;d' temp.txt > extracted/$1%.tail}_mod
sed -n '12,16 p' temp.txt >> extracted/${1%.tail}_mod
sed '18q;d' temp.txt >> extracted/${1%.tail}_mod
sed '20q;d' temp.txt >> extracted/${1%.tail}_mod
sed -n '32,38 p' temp.txt >> extracted/${1%.tail}_mod
sed -n '39,43 p' temp.txt >> extracted/${1%.tail}_mod
rm temp.txt
