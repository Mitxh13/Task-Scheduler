#ifndef SCHEDULER_H
#define SCHEDULER_H

#include <time.h>

typedef struct {
    int taskId;
    int submissionTime;  // in minutes
    char details[100];
    int completed;
    time_t startTime;
} Task;

typedef struct {
    Task **arr;
    int size;
    int capacity;
} MinHeap;

// Function declarations
MinHeap* createHeap(int capacity);
void addTask(MinHeap *heap, int taskId, int submissionTime, const char *details);
void checkAndUpdateTasks(MinHeap *heap);
void printTasks(MinHeap *heap);
void freeHeap(MinHeap *heap);

#endif
