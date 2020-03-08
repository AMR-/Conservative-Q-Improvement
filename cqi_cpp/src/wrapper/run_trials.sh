#!/bin/bash

split_thresh_max=0.1
num_splits=3

# for i in {1..5}
# do
#     python example.py --num_splits $num_splits --split_thresh_max $split_thresh_max | grep "Average reward" >> results.txt 
# done

for i in {1..5}
do
    python random_policy.py | grep "Average reward" >> results.txt 
done

# for i in {1..5}
# do
#         { time python random_policy.py --num_splits $num_splits --split_thresh_max $split_thresh_max ; } 2>> results.txt 
# done

# for steps in 10 100 1000 10000 100000
# do
#         { time python cqi_test.py --num_splits $num_splits --split_thresh_max $split_thresh_max --steps $steps ; } 2>> results.txt 
# done

# for steps in 10 100 1000 10000 100000
# do
#         python example.py --num_splits $num_splits --split_thresh_max $split_thresh_max --steps $steps | grep "Average reward" >> results.txt 
# done

sed -i '/real/d' results.txt
sed -i '/sys/d' results.txt

cat results.txt
rm results.txt
