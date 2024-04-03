
rm -rf benchmarking/*.csv
rm -rf benchmarking/*.png
g++ -o run main.cpp  src/taslock.cpp src/ttaslock.cpp src/alock.cpp src/clhlock.cpp  src/mcslock.cpp
./run
# python3 merge.py
# python3 plot.py
