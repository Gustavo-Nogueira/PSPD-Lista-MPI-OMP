import os
import time
import numpy as np

PROCESSES = [1, 2, 4, 6, 8, 10, 15, 20, 25, 30, 35, 40]
RUN_COMMAND = "make run"
FRACTAL_HEIGHT = 10000
TESTS_PER_PROC = 5

result = {}


def run():
    for nump in PROCESSES:
        result[nump] = {'times': [], 'avg': 0, 'stddev': 0}
        for t in range(TESTS_PER_PROC):
            stime = time.time()*1000
            os.system(f'{RUN_COMMAND} {nump} {FRACTAL_HEIGHT}')
            etime = time.time()*1000
            result[nump]['times'].append(etime - stime)
            time.sleep(1)
        result[nump]['avg'] = np.mean(result[nump]['times'])
        result[nump]['stddev'] = np.std(result[nump]['times'])


run()
