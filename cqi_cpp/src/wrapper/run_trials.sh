#!/bin/bash

# for i in {1..5}
# do
#         { time python random_policy.py --split_thresh_max $split_thresh_max ; } 2>> results.txt 
# done

# for i in {1..2}
# do
# python cqi_test.py --mql 1 | grep "Average reward" >> results.txt
python cqi_test.py --mql 1 --env "llc"
# done

# sed -i '/real/d' results.txt
# sed -i '/sys/d' results.txt

# cat results.txt
