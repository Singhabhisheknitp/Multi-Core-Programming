
rm -rf benchmarking/*.csv
rm -rf benchmarking/*.png
g++ -o run main.cpp  src/taslock.cpp src/ttaslock.cpp src/alock.cpp src/clhlock.cpp  src/mcslock.cpp
./run

