#!/bin/bash

# Grid search
for num_splits in 2 3 4 5
do
    for split_thresh_max in 0.1 1 10 100
    do
        python cqi_test.py --num_splits $num_splits --split_thresh_max $split_thresh_max --grid_search True
    done
done

# Find max reward and corresponding hparams line
awk 'BEGIN {max = max_line = -1000} {if ($5 > max) {max = $5; max_line = NR}} END {print max, max_line + 1}' grid_search_results.txt 
