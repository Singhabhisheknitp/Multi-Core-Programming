g++ TASlocks.cpp -o tas
g++ queuelock.cpp -o queue

./tas
./queue
echo "Data Generated"
python3 mergecsv.py
python3 plot.py

