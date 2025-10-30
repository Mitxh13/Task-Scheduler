#ifndef TASK_SCHEDULER_H
#define TASK_SCHEDULER_H

#define MAX_TASKS 1000
#define DETAILS_SIZE 256
#define MAP_SIZE 10007   /* prime for hash map */

typedef struct {
    int taskId;
    int submissionTime;
    int completed; /* 0 or 1 */
    char details[DETAILS_SIZE];
} Task;

typedef struct {
    Task **heap;    /* dynamic array of Task* */
    int size;
    int capacity;
    int *map;       /* hashmap: stores index into heap or -1 */
} Scheduler;

/* create / free */
Scheduler* createScheduler(int capacity);
void freeScheduler(Scheduler *s);

/* core ops */
int addTask(Scheduler *s, int taskId, int submissionTime, const char *details);
int serveTask(Scheduler *s, int taskId);
int cancelTask(Scheduler *s, int taskId);
int updateSubmissionTime(Scheduler *s, int taskId, int newTime);

/* access */
Task* getEarliest(Scheduler *s);

/* persistence */
int loadSchedulerFromFile(Scheduler *s, const char *path);
int saveSchedulerToFile(Scheduler *s, const char *path);

#endif
