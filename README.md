# 📊 Task Scheduler

A **Task Scheduler** implemented in **C** using a **Min-Heap** for efficient and prioritized task management.
It is a **console-based project** that automatically completes tasks after a set time period.

---

## 🚀 Overview

This scheduler allows users to manage and prioritize tasks efficiently.
Each task includes:

* **taskId** (unique identifier)
* **submissionTime** (auto-complete time in minutes)
* **details** (task description)
* **status** (Pending / Completed)

Tasks are stored in a **Min-Heap**, ensuring that the earliest (lowest) submission time always has the highest priority.
The program also auto-checks for completed tasks periodically.

---

## ⚙️ Functionalities

| Function                                   | Description                                               |
| ------------------------------------------ | --------------------------------------------------------- |
| `addTask(taskId, submissionTime, details)` | Adds a new task to the scheduler                          |
| `serveTask(taskId)`                        | Marks the specified task as **completed** manually        |
| `cancelTask(taskId)`                       | Removes a task from the scheduler                         |
| `updateSubmissionTime(taskId, newTime)`    | Updates a task’s auto-complete time and reorders the heap |
| `printTasks()`                             | Displays all current tasks with their status              |
| `checkAndUpdateTasks()`                    | Auto-completes tasks when their timer expires             |

---

## 🧱 Data Structure Used
| Data Structure | Purpose                                                  |
| -------------- | -------------------------------------------------------- |
| **Min-Heap**   | Maintains tasks in ascending order of submission time    |
| **Hash Map**   | Maps `taskId → heap index` for O(1) updates and removals |

---

## 📁 Project Folder Structure

```
Task-Scheduler/
├── scheduler.h       
├── scheduler.c       
├── main.c    
├── main.exe     
└── README.md          
```

---

## Code Structure

* **Header Files (`.h`)** — define data structures and function prototypes.
* **Source Files (`.c`)** — contain the actual logic (heap operations, task handling).
* **Main File (`main.c`)** — provides a menu-driven console interface for the user.

---

## 🛡️ Compilation & Execution

### 🧠 Compile manually

```bash
gcc main.c -o main.exe
```
### ▶️ Run
```bash
./main.exe
```

---

##  How It Works

1. User adds tasks with an ID, time (in minutes), and description.
2. Each task is inserted into a **Min-Heap** based on submission time.
3. The system continuously checks elapsed time using `checkAndUpdateTasks()`.
4. When the time expires, the task is automatically marked **Completed**.
5. Users can also **serve**, **cancel**, or **update** tasks manually.

---

## 🖥️ Example Console Output

```
Task Scheduler
1. Add Task
2. View Tasks
3. Serve Task
4. Cancel Task
5. Update Task Time
6. Exit

Enter choice: 1
Enter Task ID: 101
Enter time (in minutes) to auto-complete: 1
Enter Task Details: Test Task
Task 101 added! (Will auto-complete in 1 minute(s))

Checking for completed tasks...
Task 101 completed automatically! (Test Task)
```
