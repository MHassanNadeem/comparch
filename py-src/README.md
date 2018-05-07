# run.py
Creates multiple threads and runs the C++ cache model with varying prefetching degress and prefetching algorithms.

## How to run
`python run.py ../benchmarks/results`

## Results
Results are outputted to the console and also saved in new file with the name `<input_file_name>-<time>.json`

# tableau-convert.py
Converts the json file to csv that can be imported by tableau for visualization.

## How to run
Takes space separated list of json files and returns one csv that contains data from all the supplied json files.
`python tableau-convert.py <file1.json> <file2.json>`

# plotter.ipynb
Jupyter notebook for plotting the results using `matplotlib` graphing library.