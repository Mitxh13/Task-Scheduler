#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include "scheduler.h"
#include "scheduler.c"

int main() {
    MinHeap *heap = createHeap(50);
    int choice, id, timeMins;
    char details[100];

    while (1) {
        checkAndUpdateTasks(heap); // auto-update

        printf("\n====== Task Scheduler ======\n");
        printf("1. Add Task\n");
        printf("2. View Tasks\n");
        printf("3. Exit\n");
        printf("Enter choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                printf("Enter Task ID: ");
                scanf("%d", &id);
                printf("Enter time (in minutes) to auto-complete: ");
                scanf("%d", &timeMins);
                printf("Enter Task Details: ");
                getchar();
                fgets(details, sizeof(details), stdin);
                details[strcspn(details, "\n")] = '\0';
                addTask(heap, id, timeMins, details);
                break;
            case 2:
                printTasks(heap);
                break;
            case 3:
                freeHeap(heap);
                printf(" Exiting Scheduler.\n");
                return 0;
            default:
                printf(" Invalid choice!\n");
        }

        printf("\n(Checking every for completed tasks...)\n");
        for (int i = 0; i < 1; i++) {
            sleep(1);
            checkAndUpdateTasks(heap);
        }
    }
}
