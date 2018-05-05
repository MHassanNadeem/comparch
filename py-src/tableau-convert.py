#!/usr/bin/python

import csv, json
import sys

json_data = []

def loadJSON(filename):
    with open(filename) as f:
        json_data.append( json.load(f) )
    
    return

def writeToCSV(filename):
    with open(filename, 'wb') as csvfile:
        fieldnames = ['Benchmark', 'Algorithm', 'Block/Page', 'Prefetch Degree', 'Misfetch Rate', 'Coverage']
        writer = csv.DictWriter(csvfile, fieldnames=fieldnames)

        writer.writeheader()

        for benchmark in json_data:
            for result in benchmark['results']:
                record = {
                    'Benchmark': benchmark['file'],
                    'Algorithm':'Const Stride' if result['pa'] == 0 else 'Linked List',
                    'Block/Page':'Page' if result['isPage'] == 1 else 'Block',
                    'Prefetch Degree':result['pd'],
                    'Coverage':result['coverage'],
                    'Misfetch Rate':result['misfetchrate'],
                }
                writer.writerow(record)
    return

if __name__ == "__main__":
    if len(sys.argv) < 2:
        sys.exit("ERRPR: Provide a list of filenames as input")
        
    fileList = sys.argv[1:]

    for filename in fileList:
        loadJSON(filename)
    
    writeToCSV('tableau.csv')

    print "That's All Folks!"