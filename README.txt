
In this program, pthread is used to simulate the phone calls for ordering medicines. There are 3 semaphore declared in total, connect_lock to ensure only 8 calls at once, operators to ensure only 5 orders are being operated at the same time, and binary to prevent race condition for integer increments. The amount of phone calls are determined by argument that's passed in from the command line. In the thread function, call ID is updated as soon as the phone call is made, then it will check if the line is busy and print out message accordingly, then use semphore to block out extra calls. The ording process is simulated in a way that the program wait for 1 second. Integer connected is updated accordingly during the process, as it increases when more phone calls is connected, and decrease when calls are hung up. At the end, the main thread waits for all the new threads to finish running by calling pthread_join, then all semaphore are destroyed to clean up the memory before main thread is finished.


video link： 
https://drive.google.com/file/d/1BXA70_wrH4JUfM1OnjGxUZ_kHPvdLEeI/view?usp=sharing
