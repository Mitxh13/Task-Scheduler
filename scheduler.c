#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include "scheduler.h"

void swap(Task **a, Task **b) {
    Task *temp = *a;
    *a = *b;
    *b = temp;
}

void heapifyUp(MinHeap *heap, int index) {
    while (index > 0 && heap->arr[(index - 1) / 2]->submissionTime > heap->arr[index]->submissionTime) {
        swap(&heap->arr[index], &heap->arr[(index - 1) / 2]);
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
        heapifyDown(heap, smallest);
    }
}

MinHeap* createHeap(int capacity) {
    MinHeap *heap = (MinHeap*)malloc(sizeof(MinHeap));
    heap->arr = (Task**)malloc(capacity * sizeof(Task*));
    heap->size = 0;
    heap->capacity = capacity;
    return heap;
}

void addTask(MinHeap *heap, int taskId, int submissionTime, const char *details) {
    if (heap->size == heap->capacity) {
        printf("Heap full! Cannot add more tasks.\n");
        return;
    }
    Task *newTask = (Task*)malloc(sizeof(Task));
    newTask->taskId = taskId;
    newTask->submissionTime = submissionTime;
    strcpy(newTask->details, details);
    newTask->completed = 0;
    newTask->startTime = time(NULL);  // record current time

    heap->arr[heap->size] = newTask;
    heapifyUp(heap, heap->size);
    heap->size++;
    printf(" Task %d added! (Will auto-complete in %d minute(s))\n", taskId, submissionTime);
}

void checkAndUpdateTasks(MinHeap *heap) {
    time_t now = time(NULL);
    for (int i = 0; i < heap->size; i++) {
        if (!heap->arr[i]->completed) {
            double elapsed = difftime(now, heap->arr[i]->startTime);
            if (elapsed >= heap->arr[i]->submissionTime * 60) {
                heap->arr[i]->completed = 1;
                printf("\n Task %d completed automatically! (%s)\n", heap->arr[i]->taskId, heap->arr[i]->details);
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

void freeHeap(MinHeap *heap) {
    for (int i = 0; i < heap->size; i++)
        free(heap->arr[i]);
    free(heap->arr);
    free(heap);
}
