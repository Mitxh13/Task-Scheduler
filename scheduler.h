#ifndef SCHEDULER_H
#define SCHEDULER_H

#include <time.h>

typedef struct {
    int taskId;
    int submissionTime;  // this is for auto time i will take deafult in mins if sec needed then give in point value
    char details[100];
    int completed;
    time_t startTime;
} Task;

typedef struct {
    Task **arr;
    int size;
    int capacity;
    int *indexMap; 
} MinHeap;

// all needed functions i am declaring here
MinHeap* createHeap(int capacity);
void addTask(MinHeap *heap, int taskId, int submissionTime, const char *details);
void checkAndUpdateTasks(MinHeap *heap);
void printTasks(MinHeap *heap);
void serveTask(MinHeap *heap, int taskId);
void cancelTask(MinHeap *heap, int taskId);
void updateSubmissionTime(MinHeap *heap, int taskId, int newTime);
void freeHeap(MinHeap *heap);

#endif
