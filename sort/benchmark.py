#!/usr/bin/env python3

import subprocess
import datetime
from matplotlib import pyplot

SEC_TO_MS = 1000

PROGRAM_PATH = "./mysort"
DATA_PATH = "./data.txt"

ALGORITHMS = ((0, 'bq', "bubblesort"),
              (1, 'iq', "insertionsort"),
              (2, 'sq', "selectionsort"),
              (3, 'hq', "heapsort"),
              (4, 'mq', "mergesort"),
              (5, 'qq', "quicksort"),
              )

ALG_SAMP_IDX = 0
ALG_FLAG_IDX = 1
ALG_NAME_IDX = 2

# each row is adjusted for the respective algorithm above
SAMPLES = [list(range(0, 20001, 2000)), # bubble
           list(range(0, 27501, 2500)), #insertion
           list(range(0, 36001, 4000)), # selection
           list(range(0, 1000001, 100000)), # merge
           list(range(0, 1000001, 100000)), # quick
           list(range(0, 1000001, 100000)), # heap
          ]

# Print start time
print("Start: " + str(datetime.datetime.now()))

for alg in ALGORITHMS:
	tmp_algorithm_results = []
	for n in SAMPLES[alg[ALG_SAMP_IDX]]:
		cmd = "head -n {} {} | /usr/bin/time -f %e {} {}".format(n, DATA_PATH, PROGRAM_PATH, alg[ALG_FLAG_IDX])
		print("Running {} for N = {}...".format(alg[ALG_NAME_IDX], n))
		ps = subprocess.Popen(cmd, shell=True, stdout=subprocess.PIPE, stderr=subprocess.PIPE)
		_, stderr = ps.communicate() # usr/bin/time prints results on stderr
		tmp_algorithm_results.append(float(stderr) * SEC_TO_MS)
	pyplot.plot(SAMPLES[alg[ALG_SAMP_IDX]], tmp_algorithm_results)

# Print end time
print("End: " + str(datetime.datetime.now()))

pyplot.title("Time complexity of sorting algorithms")
pyplot.ylabel("Time [ms]")
pyplot.xlabel("Items to sort [lines of text]")
pyplot.xscale("log")
pyplot.legend([i[2] for i in ALGORITHMS], loc="upper right")
pyplot.grid(True)
pyplot.show()
