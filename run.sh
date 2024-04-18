
rm -rf benchmarking/*.csv
rm -rf benchmarking/*.pdf
g++  -o object/run main.cpp
./object/run
python3 plot.py