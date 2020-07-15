#!/bin/bash

# for i in {1..5}
# do
#         { time python random_policy.py --split_thresh_max $split_thresh_max ; } 2>> results.txt 
# done

for i in {1..10}
do
    python example.py | grep "Average reward" >> results.txt
done

# sed -i '/real/d' results.txt
# sed -i '/sys/d' results.txt

cat results.txt
