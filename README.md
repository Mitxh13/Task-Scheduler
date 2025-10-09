# System-Pilot

A **Task Scheduler** implemented in **C** using a **Min-Heap** and **Hash Map** for efficient task management. Tasks are prioritized by **earliest submission time** and can be added, served, cancelled, or updated.

---

##  Features

| Feature                | Description                                                              |
| ---------------------- | ------------------------------------------------------------------------ |
| Add Task               | Insert a new task with unique `taskId`, `submissionTime`, and `details`. |
| Serve Task             | Mark a task as completed.                                                |
| Cancel Task            | Remove a task from the scheduler.                                        |
| Update Submission Time | Modify a task's submission time and reorder it in the heap.              |
| Retrieve Task          | Access the task with the earliest submission time.                       |

---

##  Functionalities

```c
addTask(taskId, submissionTime, details);        // Add a new task
serveTask(taskId);                               // Mark task as completed
cancelTask(taskId);                              // Remove task
updateSubmissionTime(taskId, newSubmissionTime);// Update submission time & reorder
```

---

##  Data Structures Used

* **Min-Heap:** Keeps tasks sorted by `submissionTime` for quick access to the earliest task.
* **Hash Map:** Maps `taskId` → heap index for **O(1)** access when updating, serving, or cancelling tasks.

---

##  How It Works

1. **Add Task:**
   Task is added to the heap. Hash map stores the task’s heap index.

2. **Serve Task:**
   Removes the task from the heap and maintains the min-heap property.

3. **Cancel Task:**
   Removes the task without marking it completed.

4. **Update Submission Time:**
   Modifies submission time and repositions task in the heap.

---


##  Installation / Compilation

```bash
git clone https://github.com/Mitxh13/System-Pilot.git
cd System-Pilot
gcc main.c -o main.exe  #Use gcc main.c -o main.out for MacOS/Linux
./scheduler
```

---

##  License

This project is licensed under the **MIT License** – see [LICENSE](LICENSE).

--- 




