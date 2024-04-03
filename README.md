Multiprocessors programming requires synchronization to shared memeory access. 
To achieve mutual exclusion around critical section, we build different version of locks.
This repo typically focusses on spin locks , which are often better when we have relatively small critical section so thread context switch is costly alternatively.
On contrary , it is better for waiting cores to spin untill they acuire the locks and execute the job in critical section. 
This project deals with with implemmenting 5 fundamental spin locks and comapring their contention performance on x86 architechture system.

How to check
1. clone the repo to your local folder
2. run the script file in the terminal ./run.sh (this should be done on linux based system which have gcc compiler)
3. After step 2 5 csv files each for different lock with generate in benchmarking folder
4. after step 3 run the command python3 merge.py 
5. This will generate benchmarking.csv file in benchmarking folder, which will basically concatenate all the indivisual csv files 
6. run the command python3 plot.py, this will generaete png file in the benchmarking folder


   
