#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include "scheduler.h"

#define MAX_TASK_ID 100

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

//---- added during review
int compareTasks(Task *a, Task *b) {
    if (a->priority != b->priority)
        return b->priority - a->priority;  //give off any large number to set priority
    return a->submissionTime - b->submissionTime;
}

void heapifyUp(MinHeap *heap, int index) {
    while (index > 0 && compareTasks(heap->arr[index], heap->arr[(index - 1) / 2]) < 0) {
        swap(&heap->arr[index], &heap->arr[(index - 1) / 2]);
        updateIndexMap(heap, index, (index - 1) / 2);
        index = (index - 1) / 2;
    }
}

void heapifyDown(MinHeap *heap, int index) {
    int smallest = index;
    int left = 2 * index + 1;
    int right = 2 * index + 2;

    if (left < heap->size && compareTasks(heap->arr[left], heap->arr[smallest]) < 0)
        smallest = left;
    if (right < heap->size && compareTasks(heap->arr[right], heap->arr[smallest]) < 0)
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
        heap->indexMap[i] = -1;

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
    newTask->priority = 1;
    newTask->paused = 0;
    newTask->startTime = time(NULL);

    heap->arr[heap->size] = newTask;
    heap->indexMap[taskId] = heap->size;
    heapifyUp(heap, heap->size);
    heap->size++;

    printf("Task %d added! (Auto-complete in %d minute(s))\n", taskId, submissionTime);
}

void checkAndUpdateTasks(MinHeap *heap) {
    time_t now = time(NULL);
    for (int i = 0; i < heap->size; i++) {
        Task *t = heap->arr[i];
        if (!t->completed && !t->paused) {
            double elapsed = difftime(now, t->startTime);
            if (elapsed >= t->submissionTime * 60) {
                t->completed = 1;
                printf("\n Task %d completed automatically! (%s)\n", t->taskId, t->details);
            }
        }
    }
}

void printTasks(MinHeap *heap) {
    if (heap->size == 0) {
        printf("No tasks available.\n");
        return;
    }
    printf("\nCurrent Tasks:\n");
    printf("ID\tTime(min)\tPriority\tStatus\t\tDetails\n");
    printf("-------------------------------------------------------------\n");
    for (int i = 0; i < heap->size; i++) {
        Task *t = heap->arr[i];
        printf("%d\t%d\t\t%d\t\t%s\t%s\n",
               t->taskId,
               t->submissionTime,
               t->priority,
               t->completed ? "Completed" : (t->paused ? "Paused" : "Pending"),
               t->details);
    }
    printf("-------------------------------------------------------------\n");
}

void serveTask(MinHeap *heap, int taskId) {
    int index = heap->indexMap[taskId];
    if (index == -1) {
        printf("Task %d not found.\n", taskId);
        return;
    }
    if (heap->arr[index]->completed) {
        printf("Task %d already completed.\n", taskId);
        return;
    }
    heap->arr[index]->completed = 1;
    printf("Task %d marked as completed manually.\n", taskId);
}

void cancelTask(MinHeap *heap, int taskId) {
    int index = heap->indexMap[taskId];
    if (index == -1) {
        printf("Task %d not found.\n", taskId);
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

    printf("Task %d canceled and removed from scheduler.\n", taskId);
}

void updateSubmissionTime(MinHeap *heap, int taskId, int newTime) {
    int index = heap->indexMap[taskId];
    if (index == -1) {
        printf("Task %d not found.\n", taskId);
        return;
    }

    heap->arr[index]->submissionTime = newTime;
    heap->arr[index]->startTime = time(NULL);
    heapifyUp(heap, index);
    heapifyDown(heap, index);

    printf("Task %d submission time updated to %d minute(s).\n", taskId, newTime);
}

//----all below added during review 
// the priority u gave will be used here then set the heap acc to that
void setTaskPriority(MinHeap *heap, int taskId, int priority) {
    int index = heap->indexMap[taskId];
    if (index == -1) {
        printf("Task %d not found.\n", taskId);
        return;
    }
    heap->arr[index]->priority = priority;
    heapifyUp(heap, index);
    heapifyDown(heap, index);
    printf("Priority for Task %d set to %d.\n", taskId, priority);
}

// if we are giving the flag has 1 then it will pause the task for that id
void pauseTask(MinHeap *heap, int taskId) {
    int index = heap->indexMap[taskId];
    if (index == -1) {
        printf("Task %d not found.\n", taskId);
        return;
    }
    Task *t = heap->arr[index];
    if (t->paused) {
        printf("Task %d is already paused.\n", taskId);
        return;
    }
    if (t->completed) {
        printf("Task %d already completed.\n", taskId);
        return;
    }
    t->paused = 1;
    printf("Task %d paused successfully.\n", taskId);
}

void resumeTask(MinHeap *heap, int taskId) {
    int index = heap->indexMap[taskId];
    if (index == -1) {
        printf("Task %d not found.\n", taskId);
        return;
    }
    Task *t = heap->arr[index];
    if (!t->paused) {
        printf("Task %d is not paused.\n", taskId);
        return;
    }
    if (t->completed) {
        printf("Task %d already completed.\n", taskId);
        return;
    }
    t->paused = 0;
    printf("Task %d resumed successfully.\n", taskId);
}

void freeHeap(MinHeap *heap) {
    for (int i = 0; i < heap->size; i++)
        free(heap->arr[i]);
    free(heap->arr);
    free(heap->indexMap);
    free(heap);
}
