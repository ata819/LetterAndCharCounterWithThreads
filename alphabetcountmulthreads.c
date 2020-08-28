/*
 * alphabetcountmulthreads.c - this file implements the alphabetcountmulthreads function.
 *
 * Info:
 * Alan Ta
 * 819357091
 * cssc2363
 * CS570 Summer
 * Program 2: Letter frequency counter with threads
 * Due: 07/26/2020
 */

#include <stdio.h>
#include "count.h"

pthread_mutex_t lock; // Creates the lock for the threads to protect the critical region
pthread_t tid[1000]; // IDs for the threads
// A list for the files to be processed
char filelist[1000][256];

// A struct to store variables for the threads to use
struct threadStruct{
    // Sets where each thread's files are index. Start to finish.
    int threadStart;
    int threadLast;
    // pointer to values from the alphabetcountmulthreads()
    // The path (../data) and alphaFreq[] respectively
    char *threadPath;
    long *alphaThread;
};

// Thread function that handles the threads job in processing their files in parallel
void* threadRead(void* arg){
    unsigned long i;
    int n = 0, errnum = 0;
    struct threadStruct *t = (struct threadStruct*) arg;
    i = pthread_self();
    FILE *fptr;
    chdir(t->threadPath);
    printf("Thread id = %ld is processing file with index from %d to %d !\n", i, t->threadStart, t->threadLast);
    for(int j = t->threadStart; j <= t->threadLast; j++ ){
        //printf("Thread id = %ld is processing file with index from %d to %d !\n", i, t->threadStart, t->threadLast);
        printf("Thread id = %ld is processing file %s\n", i, filelist[j]);
        fptr = fopen(filelist[j], "r");
        // Locks up the thread here as this is where the incrementation happens
        pthread_mutex_lock(&lock);
        if(fptr != NULL){
            while ((n = fgetc(fptr)) != EOF){
                if(n >= 'a' && n <= 'z'){
                    t->alphaThread[n-71]++;
                }
                else if(n >= 'A' && n <= 'Z'){
                    t->alphaThread[n-65]++;
                }
            }
            fclose(fptr);
        }
        else if (fptr == NULL) {
            errnum = errno;
            fprintf(stderr, "Value of errno is: %d \n", errnum);
            // An error checker. Prints the errno value for any issue
        }
        // Unlocks the lock in the critical region, as the thread should be done incrementing
        pthread_mutex_unlock(&lock);
        // At this point, if its the last file for the thread, its done so print the statement
        if(j == t->threadLast){
            printf("Thread id = %ld is done !\n", i);
        }

    }

    return NULL;
}

// A function to find all the .txt files in a given directory and stores them in an array and returns the number of valid txt files
int storeFileList(char *path) {
    int i = 0;
    // Opens up a directory stream and uses the "path" to access the data directory
    DIR *d;
    struct dirent *dir;
    d = opendir(path);
    if (d) {
        // Begins reading the contents of the directory, and if valid, will copy the txt file into a 2D array
        while ((dir = readdir(d)) != NULL) {
            size_t len = strlen(dir->d_name);
            if (len > 4 && strcmp(dir->d_name + len - 4, ".txt") == 0) {
                strcpy(filelist[i], dir->d_name);
                i++;
            }
        }
        closedir(d);
    }
    // Closes the directory and returns the number of correct txt files
    return i;
}

// Handles the creation of the threads and sets up all the parameters for the thread function
void alphabetcountmulthreads(char *path, char *filetowrite, long alphabetfreq[], int num_threads)
{
    int fileTotal = 0, fileDivided = 0, fileRmdr = 0, fileLastDiv = 0, fileStart = 0;

    fileTotal = storeFileList(path); // Gets the total number of correct .txt files
    fileDivided = fileTotal/num_threads; // Calculates the file distribution per thread
    fileRmdr = fileTotal % num_threads; // Gets the remainder if the distribution isnt even
    fileLastDiv = (fileTotal%num_threads) + fileDivided;
    int fileLast = fileStart + fileDivided; // Sets up where the last file per thread should end on the index
    struct threadStruct threadVar[num_threads]; // creates a struct connected to the thread so more parameters/variables can be passed on to

    for(int i = 0; i < num_threads; i++){
        pthread_attr_t attr;
        pthread_attr_init(&attr);
        threadVar[i].alphaThread = &alphabetfreq[0]; //Sets the struct member pointer to the address of alphabetfreq[]
        threadVar[i].threadPath = path; // Sets the struct member point to the path
        // Sets up the first and last files needed for each thread to process
        threadVar[i].threadStart = fileStart;
        threadVar[i].threadLast = fileLast;
        if(i == num_threads-1 && fileRmdr >= 0){ // If this is the last thread
            // Adds the remainder files to the last thread and adjust the index for the last thread
            fileLast = fileLast + (fileRmdr-1);
            threadVar[i].threadLast = fileLast;
            pthread_create(&tid[i], &attr, threadRead, &threadVar[i]);
        }
        else {
            pthread_create(&tid[i], &attr, threadRead, &threadVar[i]);
        }
        // Updates the new start and last file indexes for the next thread
        fileStart = fileLast+1;
        fileLast = fileLast + fileDivided;

    }
    // Removes the threads after they finish
    for(int j = 0; j < num_threads; j++){
        pthread_join(tid[j], NULL);
    }

    // Creates a pointer to a file with write privileges and begins writing each letter and its frequency
    FILE *ResultFile = fopen(filetowrite, "w");
    fprintf(ResultFile," \n letter -> frequency \n \n");

    for(int i = 0; i < 52/2; i++)   // print upper case letter frequency
    {
        fprintf(ResultFile,"%c -> %d\n", (char)(i+65), alphabetfreq[i]);
    }

    for(int i = 52/2; i < 52; i++)  // print lower case letter frequency
    {
        fprintf(ResultFile, "%c -> %d\n", (char)(i+65 + 6), alphabetfreq[i]);
    }
    fclose(ResultFile);

}


