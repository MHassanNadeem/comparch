#!/bin/bash

####README####
#Change file paths for pintool and spec
#run as ./run_benchmarks benchmark_name
#e.g. running benchmark 505.mcf_r
#./run_benchmark 505. mcf_r
###########################################

#Enter file path of Pintool installation here
pin_path=/home/feynmann/Downloads/pin-3.5-97503-gac534ca30-gcc-linux
#Enter file path of spec installation here
spec_path=/home/feynmann/spec/spec_cpu_2017
#Enter label name. Default value in .cfg file is mytest
label=test1
#Enter name of .cfg file. If you edit the standard file without changing the name it will be Example-gcc-linux-x86.cfg
config_file=copy_Example-gcc-linux-x86.cfg

clear
echo "Running spec benchmark with gcc .. "
bench="$1"
mark="$2"
bmark="$bench$mark"
cd $spec_path
source shrc
go config
runcpu --config=$config_file --action=build $bmark
go $mark exe
exe=$(ls | grep "$label")
echo $exe
locate_exe=$(pwd)
echo $locate_exe
exec_path="$locate_exe/$exe"
echo $exec_path
cd $pin_path/source/tools/ManualExamples
pwd
make obj-intel64/pinatrace.so
../../../pin -t obj-intel64/pinatrace.so -- $exec_path
pwd
# while true; do
#     read -p "Do you wish to view this output?" yn
#     case $yn in
#         [Yy]* ) vim pinatrace.out; break;;
#         [Nn]* ) exit;;
#         * ) echo "Please answer yes or no.";;
#     esac
# done
cp $pin_path/source/tools/ManualExamples/obj-intel64/pinatrace.so /home/feynmann/benchmarks/comparch/benchmarks/