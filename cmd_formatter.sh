#!/bin/bash
exe=$1
flag=$2
input_file=$3
output_file=$4
# Clear file
: > $output_file
# Generate data
while read line; do
    formated_cmd="$exe $flag \""
    formated_cmd+="$line"
    formated_cmd+="\""
    formated_cmd+=" >> $output_file"
    eval $formated_cmd
done < $input_file