#!/bin/bash
# Write a script that uses find to look for a file and echo a suitable message if the file is not found. You must not store the output of the find command to a file.

find $1
if [ $status -ne 0 ] 
then
	echo "File not found"
else
	echo "File found"
fi


