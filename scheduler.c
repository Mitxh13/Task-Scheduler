#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include "scheduler.h"

#define MAX_TASK_ID 100 // max. task id range, 999 is the max no which u can give as id

void swap(Task **a, Task **b) {
    Task *temp = *a;
    *a = *b;
    *b = temp;
}

void updateIndexMap(MinHeap *heap, int i, int j) {
    if (i < heap->size && heap->arr[i])
        heap->indexMap[heap->arr[i]->taskId] = i;
    if (j < heap->size && heap->arr[j])
        heap->indexMap[heap->arr[j]->taskId] = j;
}

void heapifyUp(MinHeap *heap, int index) {
    while (index > 0 && heap->arr[(index - 1) / 2]->submissionTime > heap->arr[index]->submissionTime) {
        swap(&heap->arr[index], &heap->arr[(index - 1) / 2]);
        updateIndexMap(heap, index, (index - 1) / 2);
        index = (index - 1) / 2;
    }
}

void heapifyDown(MinHeap *heap, int index) {
    int smallest = index;
    int left = 2 * index + 1;
    int right = 2 * index + 2;

    if (left < heap->size && heap->arr[left]->submissionTime < heap->arr[smallest]->submissionTime)
        smallest = left;
    if (right < heap->size && heap->arr[right]->submissionTime < heap->arr[smallest]->submissionTime)
        smallest = right;

    if (smallest != index) {
        swap(&heap->arr[index], &heap->arr[smallest]);
        updateIndexMap(heap, index, smallest);
        heapifyDown(heap, smallest);
    }
}

MinHeap* createHeap(int capacity) {
    MinHeap *heap = (MinHeap*)malloc(sizeof(MinHeap));
    heap->arr = (Task**)malloc(capacity * sizeof(Task*));
    heap->indexMap = (int*)malloc(MAX_TASK_ID * sizeof(int));

    for (int i = 0; i < MAX_TASK_ID; i++)
        heap->indexMap[i] = -1; // initialize map to -1 

    heap->size = 0;
    heap->capacity = capacity;
    return heap;
}

void addTask(MinHeap *heap, int taskId, int submissionTime, const char *details) {
    if (heap->size == heap->capacity) {
        printf("Heap full! Cannot add more tasks.\n");
        return;
    }
    if (heap->indexMap[taskId] != -1) {
        printf("Task ID %d already exists!\n", taskId);
        return;
    }

    Task *newTask = (Task*)malloc(sizeof(Task));
    newTask->taskId = taskId;
    newTask->submissionTime = submissionTime;
    strcpy(newTask->details, details);
    newTask->completed = 0;
    newTask->startTime = time(NULL);

    heap->arr[heap->size] = newTask;
    heap->indexMap[taskId] = heap->size;

    heapifyUp(heap, heap->size);
    heap->size++;

    printf(" Task %d added! (Will auto-complete in %d minute(s))\n", taskId, submissionTime);
}

//--------------------------------------------------------------

void checkAndUpdateTasks(MinHeap *heap) {
    time_t now = time(NULL);
    for (int i = 0; i < heap->size; i++) {
        if (!heap->arr[i]->completed) {
            double elapsed = difftime(now, heap->arr[i]->startTime);
            if (elapsed >= heap->arr[i]->submissionTime * 60) {
                heap->arr[i]->completed = 1;
                printf("\nTask %d completed automatically! (%s)\n",
                       heap->arr[i]->taskId, heap->arr[i]->details);
            }
        }
    }
}

void printTasks(MinHeap *heap) {
    if (heap->size == 0) {
        printf("No tasks available.\n");
        return;
    }
    printf("\n Current Tasks:\n");
    printf("ID\tTime(min)\tStatus\t\tDetails\n");
    printf("--------------------------------------------------\n");
    for (int i = 0; i < heap->size; i++) {
        printf("%d\t%d\t\t%s\t%s\n",
               heap->arr[i]->taskId,
               heap->arr[i]->submissionTime,
               heap->arr[i]->completed ? "Completed" : "Pending",
               heap->arr[i]->details);
    }
    printf("--------------------------------------------------\n");
}

void serveTask(MinHeap *heap, int taskId) {
    int index = heap->indexMap[taskId];
    if (index == -1) {
        printf(" Task %d not found.\n", taskId);
        return;
    }
    if (heap->arr[index]->completed) {
        printf(" Task %d is already completed.\n", taskId);
        return;
    }
    heap->arr[index]->completed = 1;
    printf(" Task %d marked as completed manually.\n", taskId);
}

void cancelTask(MinHeap *heap, int taskId) {
    int index = heap->indexMap[taskId];
    if (index == -1) {
        printf(" Task %d not found.\n", taskId);
        return;
    }

    free(heap->arr[index]);
    heap->arr[index] = heap->arr[heap->size - 1];
    heap->indexMap[heap->arr[index]->taskId] = index;

    heap->size--;
    heap->indexMap[taskId] = -1;

    if (index < heap->size) {
        heapifyUp(heap, index);
        heapifyDown(heap, index);
    }

    printf(" Task %d canceled and removed from scheduler.\n", taskId);
}

void updateSubmissionTime(MinHeap *heap, int taskId, int newTime) {
    int index = heap->indexMap[taskId];
    if (index == -1) {
        printf(" Task %d not found.\n", taskId);
        return;
    }

    heap->arr[index]->submissionTime = newTime;
    heap->arr[index]->startTime = time(NULL);
    heapifyUp(heap, index);
    heapifyDown(heap, index);

    printf(" Task %d submission time updated to %d minute(s).\n", taskId, newTime);
}

void freeHeap(MinHeap *heap) {
    for (int i = 0; i < heap->size; i++)
        free(heap->arr[i]);
    free(heap->arr);
    free(heap->indexMap);
    free(heap);
}
