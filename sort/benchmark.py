#!/usr/bin/env python3

import subprocess
import datetime
from matplotlib import pyplot

SEC_TO_MS = 1000

PROGRAM_PATH = "./mysort"
DATA_PATH = "./data.txt"

ALGORITHMS = (('bq', "bubblesort"),
              ('iq', "insertionsort"),
              ('sq', "selectionsort"),
              ('hq', "heapsort"),
              ('mq', "mergesort"),
              ('qq', "quicksort"),
              )

ALG_FLAG_IDX = 0
ALG_NAME_IDX = 1

# optimal starting value and ranges below might vary between machines
X_START = 2000

# rows are adjusted for respective algorithms above to be comparable on the graph
SAMPLES = [list(range(X_START, 18001, 2000)), # bubble
           list(range(X_START, 26501, 2500)), # insertion
           list(range(X_START, 31001, 4000)), # selection
           list(range(X_START, 1000001, 100000)), # merge
           list(range(X_START, 1000001, 100000)), # quick
           list(range(X_START, 1000001, 100000)), # heap
          ]

# print start time
print("Start: " + str(datetime.datetime.now()))

# the benchmark loop
for i, alg in enumerate(ALGORITHMS):
	tmp_algorithm_results = []
	for j, n in enumerate(SAMPLES[i], start = 1):
		cmd = f"head -n {n} {DATA_PATH} | /usr/bin/time -f %e {PROGRAM_PATH} {alg[ALG_FLAG_IDX]}"
		print(f"* Running {alg[ALG_NAME_IDX]} for n = {n}... [{j}/{len(SAMPLES[i])}]")
		if j == 1: # don't spam too much
			print(f"  {cmd}")
		ps = subprocess.Popen(cmd, shell=True, stdout=subprocess.PIPE, stderr=subprocess.PIPE)
		_, stderr = ps.communicate() # usr/bin/time prints results on stderr
		tmp_algorithm_results.append(float(stderr) * SEC_TO_MS)
	pyplot.plot(SAMPLES[i], tmp_algorithm_results)

# print end time
print("End: " + str(datetime.datetime.now()))

# decorate plot
pyplot.title("Comparison of sorting algorithms")
pyplot.ylabel("Time [ms]")
pyplot.legend([i[ALG_NAME_IDX] for i in ALGORITHMS], loc="upper right")
pyplot.xlabel("Items to sort [lines of text]")
pyplot.xscale("log")
pyplot.grid(True)

# done
pyplot.show()
