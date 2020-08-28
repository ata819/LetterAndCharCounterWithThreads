/* charcount.h - This header file include type definitions (including function prototypes) and macros
   used for the programing assignment two.
 * Info:
 * Alan Ta
 * 819357091
 * cssc2363
 * CS570 Summer
 * Program 2: Letter frequency counter with threads
 * Due: 07/26/2020
*/

#include <stdio.h>
#include <dirent.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <pthread.h>
#define ALPHABETSIZE 52     //The total number of alphabetical letter from A - Z  a - z (case sensitive)

// A function for the threads to work with. Counts the alphabet frequency from the .txt files provided
void* threadRead(void* arg);

// A function that counts/collects the correct .txt files from "./data" path and returns the total as well as provide a 2D array with the file names
int storeFileList(char *path);

// Handles creating ans setting up the struct variables for the threadRead function
// Handles how many threads are made and when to remove them after finishing
// As well as printing the results to the result.txt
void alphabetcountmulthreads(char *path, char *filetowrite, long alphabetfreq[], int num_threads);
