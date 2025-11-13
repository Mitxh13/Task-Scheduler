#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include "scheduler.h"
#include "scheduler.c"

int main() {
    MinHeap *heap = createHeap(50);
    int choice, id, timeMins, priority;
    char details[100];

    while (1) {
        checkAndUpdateTasks(heap);

        printf("\n Task Scheduler \n");
        printf("1. Add Task\n");
        printf("2. View Tasks\n");
        printf("3. Mark Complete\n");
        printf("4. Cancel Task\n");
        printf("5. Update Time\n");
        printf("6. Set Priority\n");
        printf("7. Pause Task\n");
        printf("8. Resume Task\n");
        printf("9. Exit\n");
        printf("Enter choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                printf("Enter Task ID: ");
                scanf("%d", &id);
                printf("Enter time (in minutes): ");
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
                printf("Enter Task ID to mark complete: ");
                scanf("%d", &id);
                serveTask(heap, id);
                break;

            case 4:
                printf("Enter Task ID to cancel: ");
                scanf("%d", &id);
                cancelTask(heap, id);
                break;

            case 5:
                printf("Enter Task ID to update: ");
                scanf("%d", &id);
                printf("Enter new time (in minutes): ");
                scanf("%d", &timeMins);
                updateSubmissionTime(heap, id, timeMins);
                break;

            case 6:
                printf("Enter Task ID to set priority: ");
                scanf("%d", &id);
                printf("Enter priority (1 = low, higher = more important): ");
                scanf("%d", &priority);
                setTaskPriority(heap, id, priority);
                break;

            case 7:
                printf("Enter Task ID to pause: ");
                scanf("%d", &id);
                pauseTask(heap, id);
                break;

            case 8:
                printf("Enter Task ID to resume: ");
                scanf("%d", &id);
                resumeTask(heap, id);
                break;

            case 9:
                freeHeap(heap);
                printf("Exiting Scheduler.\n");
                return 0;

            default:
                printf("Invalid choice!\n");
        }

        printf("\nChecking for completed tasks...\n");
        sleep(1);
        checkAndUpdateTasks(heap);
    }
}
