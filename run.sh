g++ main.cpp -o run
./run
echo "Data Generated"
python3 mergecsv.py
python3 plot.py

