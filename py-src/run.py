#!/usr/bin/python

from subprocess import call
import subprocess
import threading
import os, sys
import datetime, time
import json

CPP_MODEL_PATH = os.path.abspath( '../cpp-src/main.bin' )


def runBenchmark(fileName, pdRange, prefetchingAlgoRange, isPagePredictionRange):
    results = []
    thread_list = []

    def run(pd, prefetchingAlgo, isPagePrediction):
        res = subprocess.check_output([CPP_MODEL_PATH, fileName, str(pd), str(prefetchingAlgo), str(isPagePrediction)])
        res = res.strip().split()
        resDict = {
            "pd":pd,
            "pa":prefetchingAlgo,
            "isPage":isPagePrediction,
            "hits":res[0],
            "misses":res[1],
            "hitrate":res[2],
            "misrate":res[3],
            "coverage":res[4],
            "misfetchrate":res[5]
            }
        results.append(resDict)
        # print res

    # Create threads
    for pd in pdRange:
        for prefetchingAlgo in prefetchingAlgoRange:
            for isPagePrediction in isPagePredictionRange:
                t = threading.Thread(target=run, args=(pd,prefetchingAlgo,isPagePrediction ))
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
        filePath = os.path.abspath( sys.argv[1] )
        fileName = os.path.split(filePath)[-1]

        if not os.path.isfile(filePath):
            sys.exit("ERROR: File {0} does not exist".format(filePath))

        if not os.path.isfile(CPP_MODEL_PATH):
            sys.exit("ERROR: CPP Model does not exist")


        start_time = time.time()
        results = runBenchmark(filePath, range(0,160,10), [0,1], [0,1])
        elapsed_time = time.time() - start_time

        # Arrange results
        results = sorted(results, key=lambda k: (k['isPage'],k['pa'],k['pd'])) 

        output = {
            'file':fileName,
            'filepath':filePath,
            'results':results,
            'time':elapsed_time
        }        

        # Print result on console
        print json.dumps(output, indent=4, sort_keys=True)

        # Save results to file
        time = datetime.datetime.fromtimestamp(time.time()).strftime('%Y-%m-%d %H:%M:%S')
        with open('{0}-{1}.json'.format(fileName, time), 'w') as outfile:
            json.dump(output, outfile, indent=4, sort_keys=True)
    else:
        sys.exit("ERROR: Benchmark file not specified")