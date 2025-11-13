#ifndef SCHEDULER_H
#define SCHEDULER_H

#include <time.h>

typedef struct {
    int taskId;
    int submissionTime;      //take this time in mins. 
    char details[100];
    int completed;
    int priority;            // give off higher values for more priority
    int paused;              // 0 = running, 1 = paused
    time_t startTime;
} Task;

typedef struct {
    Task **arr;
    int size;
    int capacity;
    int *indexMap;
} MinHeap;

//declarations
MinHeap* createHeap(int capacity);
void addTask(MinHeap *heap, int taskId, int submissionTime, const char *details);
void checkAndUpdateTasks(MinHeap *heap);
void printTasks(MinHeap *heap);
void serveTask(MinHeap *heap, int taskId);
void cancelTask(MinHeap *heap, int taskId);
void updateSubmissionTime(MinHeap *heap, int taskId, int newTime);
void setTaskPriority(MinHeap *heap, int taskId, int priority);
void pauseTask(MinHeap *heap, int taskId);
void resumeTask(MinHeap *heap, int taskId);
void freeHeap(MinHeap *heap);

#endif
