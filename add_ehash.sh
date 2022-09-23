#!/bin/bash
filename=$1
while read line; do
    formated_cmd="./ehash.out add \""
    formated_cmd+="$line"
    formated_cmd+="\""
    eval $formated_cmd
done < $filename