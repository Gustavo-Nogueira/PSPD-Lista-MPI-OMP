import os
import re
import csv
import time
import numpy as np

THREADS = [1, 2, 4, 6, 8]
VSIZES = [1000, 5000, 10000, 50000, 100000, 200000]
SOLUTIONS = [{'regex': r'C Quicksort ...........: \d+\.\d+ segundos', 'label': "QuickSort"},
             {'regex': r'Selection Sort Serial .: \d+\.\d+ segundos', 'label': "SelectionSort_S"},
             {'regex': r'Selection Sort OMP ....: \d+\.\d+ segundos', 'label': "SelectionSort_OMP"}]
RUN_COMMAND = "make run"
TESTS_PER_PROC = 3

result = {}


def read_file(path):
    file = open(path, mode='r')
    all_of_it = file.read()
    file.close()
    return all_of_it


def extract_exec_times(path):
    content = read_file(path)
    res = {}
    try:
        for soltype in SOLUTIONS:
            line = re.findall(soltype['regex'], content)
            time = re.findall(r'\d+\.\d+', line[0])
            res[soltype['label']] = time[0]
        return res
    except:
        print('Error...skipping')
        return None


def run():
        for thr in THREADS:
            for vsize in VSIZES:
                result[f'thr-{thr}_sz-{vsize}'] = []
                for test in range(TESTS_PER_PROC):
                    os.system(f'{RUN_COMMAND} {thr} {vsize} > outfile.txt')
                    res = extract_exec_times('outfile.txt')
                    result[f'thr-{thr}_sz-{vsize}'].append(res)
                    time.sleep(1)


run()

print(result)
