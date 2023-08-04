#!/bin/bash

#Write a menu driven program to create Employee Record.
#The Record should contains: Employee Name, Employee Number, and Telephone Number.
#The script should be able to delete a record, search and display a specific employee details, sort the record and list the complete record.

file=$1

function menu(){
	echo ""
	echo "Menu:"
	echo "1. Create Employee Record"
	echo "2. Delete Employee Record"
	echo "3. Search and Display Record"
	echo "4. Sort Records"
	echo "5. List Records"
	echo "6. Exit"
	echo ""
	echo "Enter choice (1-6): "
}

#read choice

#select

tmp="tmp.txt"

select option in "Create Employee Record" "Delete Employee Record" "Search and Display Record" "Sort Records" "List Records" "Exit"
do 	
	echo $option
	if [[ $option == "Exit" ]];then
		break
	elif [[ $option == "Create Employee Record" ]];then
		echo "Enter Employee name"
		read name
		echo "Enter Employee number"
		read num
		echo "Enter phone number"
		read phone
		
		if grep -P "\t$num\t" $file; then
			echo "Record exits already"
		else
			echo -e "$name\t$num\t$phone" >> $file
			echo "Record added successfully"
		fi
		
	elif [[ $option == "List Records" ]]; then
		cat $file
	elif [[ $option == "Search and Display Record" ]]; then
		echo "Enter Employee number to search"
		read num
		grep -P "\t$num\t" $file
	elif [[ $option == "Delete Employee Record" ]]; then
		echo "Enter Employee number to delete"
		read num
		touch $tmp
		grep -v -P "\t$num\t" $file > $tmp
		cp $tmp $file
		rm $tmp
		echo "Record deleted successfully"
	elif [[ $option=="Sort Records" ]]; then
		touch $tmp
		sort -k 2 $file > $tmp
		cp $tmp $file
		rm $tmp
		echo "Record sorted successfully"
	else 
		echo "Invalid input"
	fi
	menu
done
		
