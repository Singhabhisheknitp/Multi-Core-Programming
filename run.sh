
rm -rf benchmarking/*.csv
rm -rf benchmarking/*.pdf
g++ -o run main.cpp  src/taslock.cpp src/alock.cpp  src/clhlock.cpp src/mcslock.cpp src/ttaslock.cpp
./run
python3 plot.py
