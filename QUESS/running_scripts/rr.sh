#!/bin/bash
while IFS='' read -r line || [[ -n "$line" ]]; do
	printf %b "$line\t" >> $2
done < "$1"
