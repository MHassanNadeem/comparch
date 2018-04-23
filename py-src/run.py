#!/usr/bin/python

from subprocess import call
import subprocess
import threading
import os, sys

CPP_MODEL_PATH = '../cpp-src/main.bin'

def runBenchmark(fileName, pdRange, prefetchingAlgo, isPagePrediction):
    results = []
    thread_list = []

    def run(pd):
        res = subprocess.check_output([CPP_MODEL_PATH, fileName, str(pd), str(prefetchingAlgo), str(isPagePrediction)])
        res = res.strip().split()
        resDict = {
            "hits":res[0],
            "misses":res[1],
            "hitrate":res[2],
            "misrate":res[3],
            "coverage":res[4],
            "misfetchrate":res[5]
            }
        results.append(resDict)
        print res

    # Create threads
    for pd in pdRange:
        t = threading.Thread(target=run, args=(pd, ))
        thread_list.append(t)

    # Starts threads
    for thread in thread_list:
        thread.start()

    # Wait until all threads finish execution
    for thread in thread_list:
        thread.join()

    return results

if __name__ == "__main__":
    if len(sys.argv) > 1:
        fileName = sys.argv[1]
        fileName = '526_blender_r_pinatrace.out'
        fileName = '../benchmarks/results'
        fileName = os.path.abspath( fileName )

        print fileName

        results = runBenchmark(fileName, [0,1], 0, 0)
        print results