#!/bin/bash
#print $2 times $1
input=$1
for (( i=1; i<=$2; i++ ))
do
    printf $1
done

