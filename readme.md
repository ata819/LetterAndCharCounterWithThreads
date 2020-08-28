Info:
Alan Ta
819357091
cssc2363
CS570 Summer
Program 2: Letter frequency counter with threads
Due: 07/19/2020
Program Description: The purpose of this program was to understand the use of threads in a program and how they can drastically increase processing. It was also to understand how threads work and the requirements they need, such as a mutex to lock the critical region. The program was designed to could the frequency of a list of text files and it would use threads to provide parallel processing.

Files included:
count.h
alphabetcountmulthreads.c
makefile
readme.md

Libraries:
#include <stdio.h> - print()
#include <dirent.h> - Used for directory paths. i.e. opeddir(), readdir()
#include <string.h> - strcpy, strcmp, strlen
#include <unistd.h> -  chdir()
#include <errno.h> - errno
#include <pthread.h> - pthread_t, pthread_mutex, pthread_create, ...

How to run:

Enter the appropriate directory where all the files are held.

Warning: Because of the way the program was design, the location of neccesary directories are need. There should be a result and data directory as they are paths the program will use to 1. Gather the data set from (txt files) and 2. place the result.txt. in

:make

:./testmulthreads # // the execution is followed by a number, which is the number of threads you would like

The successful execution should present the results in the result directory labed either
result.txt

It will also print out the results to stdout on screen:
Thread id = 274237184 starts processing files with index from 0 to 10!
Thread id = 265844480 starts processing files with index from 11 to 22!
Thread id = 257451776 starts processing files with index from 23 to 31!
Thread id = 265844480 is processing file input_11.txt
Thread id = 257451776 is processing file input_22.txt
Thread id = 274237184 is processing file input_00.txt
… …
Thread id = 274237184 is done !
Thread id = 265844480 is done !
Thread id = 257451776 is done !
The results are counted as follows:
a -> 2861232
b -> 494472
c -> 747252
… …

// It will label the indexes, file names, and thread id of all threads and what file they are processing

:make clean // To remove execuables and .o files

Design & Code explaination:
This program assignment was probably one of the hardest and most confusing to deal with. I had a tough time because I had never used threads before. upon reading, I figure you could pass in 1 parameter when you create the thread. But i knew that wasn't enough. In order for the thread function to perform correctly, I had to figure out a way to pass in more variables to use. This stumped me for the longest time until I found this video: https://www.youtube.com/watch?v=1ks-oMotUjc . Where it introduced the idea of using structs which was better than global variables. This was the key to everything. I knew using a struct, I could have more variables for my thread to use.  
So the basic idea of threads is that you need a function and youd use thread_create and pass on the function name. Knowing that I could use a struct, I knew I needed at least 3 variables. 1 variable for the start of the index and 1 for the last. The last variable had to somehow connect to the alphaFreq[] because the alphaFreq[] had to be kept up to date. Before this idea, I thought I could just create a separate array and copy it over. Of course that doesn't work cuz it doesnt keep alphaFreq[] up to date.  
So I create a long pointer to point to the address of the alphaFreq[]. I had this idea because I figured the array of alphaFreq[] is fixed in memory, as in its memory address is created and stays fixed. Thus if I could point to that address, I could access and increment the values in alphaFreq[]. This was probably the hardest idea I had to come up with.

struct threadStruct{
    int threadStart;
    int threadLast;
    char *threadPath;
    long *alphaThread;
};

The rest of the code was inspired by the video. The rest of the code (alphacountmulthreads()) was to set up for the thread.

    fileTotal = storeFileList(path);
    fileDivided = fileTotal/num_threads; 
    fileRmdr = fileTotal % num_threads; 
    int fileLast = fileStart + fileDivided; 
    struct threadStruct threadVar[num_threads]; 
I need to find the fileTotal and with that, I could get how the threads should be distributed. I could also calculate the remainder for an uneven distribution.
I needed to set up where the index should start and thus end. So the start needs to be [0] and the last should be the start + the distribution:
30 files with 3 threads = 10 per thread
Start: Index[0]
End: Index[0] + 10

But in order to keep this going, I had to update the start and end after the pthread_create() so when the next thread starts from the top, itll have the new start and end index
Start = End
End = End + 10 // Is the basic idea
This is all in a for loop and the for loops would increment per threads.
Before the for loop, I set up the struct and pass the values I got (fileDivided, fileStart, etc.) to the struct variables. This adds variables for the thread function to use and perform correctly. 
        
Using the struct, I can assign values that I can access/use/update in my thread
        threadVar[i].alphaThread = &alphabetfreq[0]; //Sets the struct member pointer to the address of alphabetfreq[]
        threadVar[i].threadPath = path; // Sets the struct member point to the path
        // Sets up the first and last files needed for each thread to process
        threadVar[i].threadStart = fileStart;
        threadVar[i].threadLast = fileLast;
        
In terms of the thread function:
Have the start and end index, I just print it and had a for loop for the files per thread to read. They would read and getc of each character, where it would increment using the struct pointer that is pointing to the alpbaFreq[] address. This set of code is extremely similar to my old program 0 code. Using the ascii values to compare and increment correctly.
I had a catch if loops near the end of my for loop where if it was the last file for that thread, it will print that the thread was finished processing. 
Back on my alphabetcountmulthread, I had a for loops to remove the threads

Possible Improvements/Alternate Designs:
So I noticed that in my program, the indexes for the first thread is 1 more than it should be. So say there 31 files and there are 3 threads. The first index could be 0 - 9, but mine is 0 - 10. This is because I set the start to 0, which is the first of an index and I added the distribution making it 0 + 10 which is 10. Hence, 0 - 10. Because how the indexes are and how numbers are, 0 isnt really 0 for indexes. Its 1, but labeled as 0. Its mismatch is what causes the potential issue. Technically it all works fine in the end as instead of the last thread having the extra file, its the first. I tried to figure a correct to this, but I am really satisfied with this program and I noticed someone's post had the same issue and was approved by the professor. So it could be an issue or not. 
