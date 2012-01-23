make
clear
echo ================================
echo Processes
echo ================================
./bin/process -f process_default.txt
echo --------------------------------
./bin/process -w 100 -t 1500 -f t1_process.txt
echo --------------------------------
./bin/process -w 150 -t 2000 -f t2_process.txt
echo --------------------------------
./bin/process -w 200 -t 2500 -f t3_process.txt
echo ================================
echo Threads
echo ================================
./bin/thread -f thread_default.txt
echo --------------------------------
./bin/thread -w 100 -t 1500 -f t1_thread.txt
echo --------------------------------
./bin/thread -w 150 -t 2000 -f t2_thread.txt
echo --------------------------------
./bin/thread -w 200 -t 2500 -f t3_thread.txt
