rm rf benchmarking/*.csv
rm rf benchmarking/*.png
g++ -o run main.cpp src/clhlock.cpp src/ttaslock.cpp 
./run