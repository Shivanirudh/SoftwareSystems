#!/bin/bash

#Write a shell program that checks the number of command line arguments and echoes an error message if
#there are not exactly three arguments or echoes the arguments themselves if there are three.

if [ $# == 3 ]
then
	echo "Arguments: $1 $2 $3"
else
	echo "Number of arguments is not 3"
fi
