#!/bin/bash
# Write a shell script to count the number of block device files in /dev directory.
# Command to solve via CLI: ls /dev -l | grep b | wc -l 


count=0
for file in `ls /dev`
do
	path="/dev/"
	path+=$file
	if [ -b $path ]
       	then
		count=$((count+1))
	fi
done
echo $count
