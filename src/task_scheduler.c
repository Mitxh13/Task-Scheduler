#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "src\task_scheduler.h"
/* Hash map: open addressing with linear probing */
static int map_hash(int taskId) {
    int h = taskId % MAP_SIZE;
    if (h < 0) h += MAP_SIZE;
    return h;
}

/* Create scheduler */
Scheduler* createScheduler(int capacity) {
    Scheduler *s = malloc(sizeof(Scheduler));
    if (!s) return NULL;
    s->heap = malloc(sizeof(Task*) * capacity);
    s->map = malloc(sizeof(int) * MAP_SIZE);
    if (!s->heap || !s->map) { free(s->heap); free(s->map); free(s); return NULL; }
    for (int i = 0; i < MAP_SIZE; ++i) s->map[i] = -1;
    s->size = 0;
    s->capacity = capacity;
    return s;
}

void freeScheduler(Scheduler *s) {
    if (!s) return;
    for (int i = 0; i < s->size; ++i) free(s->heap[i]);
    free(s->heap);
    free(s->map);
    free(s);
}

/* map operations */
static void map_put(Scheduler *s, int taskId, int index) {
    int h = map_hash(taskId);
    while (s->map[h] != -1 && s->map[h] != -2) {
        Task *t = s->heap[s->map[h]];
        if (t && t->taskId == taskId) break;
        h = (h + 1) % MAP_SIZE;
    }
    s->map[h] = index;
}

static int map_get(Scheduler *s, int taskId) {
    int h = map_hash(taskId);
    int start = h;
    while (s->map[h] != -1) {
        if (s->map[h] != -2) {
            int idx = s->map[h];
            if (idx >= 0 && idx < s->size) {
                Task *t = s->heap[idx];
                if (t && t->taskId == taskId) return idx;
            }
        }
        h = (h + 1) % MAP_SIZE;
        if (h == start) break;
    }
    return -1;
}

static void map_remove(Scheduler *s, int taskId) {
    int h = map_hash(taskId);
    int start = h;
    while (s->map[h] != -1) {
        if (s->map[h] != -2) {
            int idx = s->map[h];
            if (idx >= 0 && idx < s->size) {
                Task *t = s->heap[idx];
                if (t && t->taskId == taskId) {
                    s->map[h] = -2; /* tombstone */
                    return;
                }
            }
        }
        h = (h + 1) % MAP_SIZE;
        if (h == start) break;
    }
}

/* heap helpers */
static void swap_heap(Scheduler *s, int i, int j) {
    Task *tmp = s->heap[i];
    s->heap[i] = s->heap[j];
    s->heap[j] = tmp;
    /* update map entries: find where each task id is stored and update to new index */
    map_put(s, s->heap[i]->taskId, i);
    map_put(s, s->heap[j]->taskId, j);
}

/* heapify up */
static void heapify_up(Scheduler *s, int i) {
    while (i > 0) {
        int p = (i - 1) / 2;
        if (s->heap[p]->submissionTime <= s->heap[i]->submissionTime) break;
        swap_heap(s, p, i);
        i = p;
    }
}

/* heapify down */
static void heapify_down(Scheduler *s, int i) {
    while (1) {
        int l = 2*i + 1, r = 2*i + 2, smallest = i;
        if (l < s->size && s->heap[l]->submissionTime < s->heap[smallest]->submissionTime) smallest = l;
        if (r < s->size && s->heap[r]->submissionTime < s->heap[smallest]->submissionTime) smallest = r;
        if (smallest == i) break;
        swap_heap(s, i, smallest);
        i = smallest;
    }
}

/* public functions */
int addTask(Scheduler *s, int taskId, int submissionTime, const char *details) {
    if (!s) return -1;
    if (s->size >= s->capacity) return -2;
    /* if exists, treat as update */
    int existing = map_get(s, taskId);
    if (existing != -1) return -3; /* duplicate */

    Task *t = malloc(sizeof(Task));
    if (!t) return -4;
    t->taskId = taskId;
    t->submissionTime = submissionTime;
    t->completed = 0;
    strncpy(t->details, details ? details : "", DETAILS_SIZE-1);
    t->details[DETAILS_SIZE-1] = '\0';

    s->heap[s->size] = t;
    map_put(s, taskId, s->size);
    heapify_up(s, s->size);
    s->size++;
    return 0;
}

int serveTask(Scheduler *s, int taskId) {
    int idx = map_get(s, taskId);
    if (idx == -1) return -1;
    s->heap[idx]->completed = 1;
    return 0;
}

int cancelTask(Scheduler *s, int taskId) {
    int idx = map_get(s, taskId);
    if (idx == -1) return -1;
    free(s->heap[idx]);
    s->heap[idx] = s->heap[--s->size];
    /* if moved element exists, update its map entry (we'll re-put by id) */
    if (idx < s->size) {
        map_put(s, s->heap[idx]->taskId, idx);
        heapify_up(s, idx);
        heapify_down(s, idx);
    }
    map_remove(s, taskId);
    return 0;
}

int updateSubmissionTime(Scheduler *s, int taskId, int newTime) {
    int idx = map_get(s, taskId);
    if (idx == -1) return -1;
    s->heap[idx]->submissionTime = newTime;
    heapify_up(s, idx);
    heapify_down(s, idx);
    return 0;
}

Task* getEarliest(Scheduler *s) {
    if (!s || s->size == 0) return NULL;
    return s->heap[0];
}

/* persistence: binary file dump of fixed-size structs */
int loadSchedulerFromFile(Scheduler *s, const char *path) {
    if (!s) return -1;
    FILE *f = fopen(path, "rb");
    if (!f) return 0; /* treat as empty - not an error */
    int count = 0;
    if (fread(&count, sizeof(int), 1, f) != 1) { fclose(f); return -2; }
    for (int i = 0; i < count && i < s->capacity; ++i) {
        Task tmp;
        if (fread(&tmp, sizeof(Task), 1, f) != 1) break;
        Task *t = malloc(sizeof(Task));
        if (!t) break;
        *t = tmp;
        s->heap[s->size] = t;
        map_put(s, t->taskId, s->size);
        s->size++;
    }
    fclose(f);
    /* rebuild heap because file may not be heap-ordered (safe) */
    for (int i = (s->size/2) - 1; i >= 0; --i)
        heapify_down(s, i);
    return 0;
}

int saveSchedulerToFile(Scheduler *s, const char *path) {
    if (!s) return -1;
    FILE *f = fopen(path, "wb");
    if (!f) return -2;
    if (fwrite(&s->size, sizeof(int), 1, f) != 1) { fclose(f); return -3; }
    for (int i = 0; i < s->size; ++i) {
        Task tmp = *s->heap[i];
        if (fwrite(&tmp, sizeof(Task), 1, f) != 1) { fclose(f); return -4; }
    }
    fclose(f);
    return 0;
}
