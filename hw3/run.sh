#!/bin/bash

make

echo "5 3" | ./hw3boat.out & 
echo "5 3" | ./hw3person.out & 
wait
