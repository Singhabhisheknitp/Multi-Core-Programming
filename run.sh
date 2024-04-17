
rm -rf benchmarking/*.csv
rm -rf benchmarking/*.pdf
g++ main.cpp -o ./object/run
./object/run
python3 plot.py