
rm -rf benchmarking/*.csv
rm -rf benchmarking/*.pdf
# g++ -g -o run main.cpp
# valgrind ./object/run
g++  -o run main.cpp
./object/run
python3 plot.py