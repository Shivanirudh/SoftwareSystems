#!/bin/bash

#Write a shell program called new_files that will accept a variable number of command line arguments. The shell program will create a new file associated with each command line argument and echo a message that notifies the user as each file is created.

path="new_files_created"
if [ -d $path ]
then
	echo "Directory exists. Starting file creation"
else
	mkdir $path
	echo "Created directory. Starting file creation"
fi
for file_name in $*
do
	file=$path
	file+="/"
	file+=$file_name
	touch $file
	echo "File $file_name created at $file"
done
