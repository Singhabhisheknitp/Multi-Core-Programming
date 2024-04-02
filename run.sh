
rm -rf benchmarking/*.csv
rm -rf benchmarking/*.png
g++ -o run main.cpp src/clhlock.cpp
./run
# python3 merge.py
# python3 plot.py
