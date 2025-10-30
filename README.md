# 📊 Task Scheduler

A **Task Scheduler** implemented in **C** using a **Min-Heap** and **Hash Map** for efficient and prioritized task management.
Includes a **modern web-based frontend (HTML, CSS, JS)** for visualization and interaction — all connected via a **CGI-based backend** or **Docker container**.

---

## 🚀 Overview

This scheduler allows users to manage and prioritize tasks efficiently.
Each task is defined by:

* **taskId** (unique identifier)
* **submissionTime** (priority key)
* **details** (task description)
* **status** (Pending / Completed)

Tasks are stored in a **Min-Heap**, ensuring that the earliest submission time always has the highest priority.
A **Hash Map** ensures **O(1)** lookup, update, and deletion by `taskId`.

---

##  Functionalities

| Function                                   | Description                                     |
| ------------------------------------------ | ----------------------------------------------- |
| `addTask(taskId, submissionTime, details)` | Adds a new task to the scheduler                |
| `serveTask(taskId)`                        | Marks the specified task as **completed**       |
| `cancelTask(taskId)`                       | Removes a task from the scheduler               |
| `updateSubmissionTime(taskId, newTime)`    | Updates a task’s time and **reorders** the heap |
| `viewTasks()`                              | Retrieves all tasks, ordered by submission time |

---

## Data Structures Used

| Data Structure | Purpose                                                  |
| -------------- | -------------------------------------------------------- |
| **Min-Heap**   | Maintains tasks in ascending order of submission time    |
| **Hash Map**   | Maps `taskId → heap index` for O(1) updates and removals |

---

## 📁 Project Folder Structure

```
Task-Scheduler/
├─ web/
│  ├─ index.html
│  ├─ style.css
│  └─ script.js
├─ cgi-bin/
│  ├─ task_scheduler.h
│  ├─ task_scheduler.c
│  └─ scheduler.c
├── Dockerfile
├── main.c                   
└── README.md              

```

---

##  Code Structure

* **Header Files (`.h`)** — contain function prototypes, constants, and structures.
* **Source Files (`.c`)** — contain actual logic and implementations.
* **Main File (`main.c`)** — drives execution (for CLI testing).
* **Frontend** — provides a simple UI connected to the C backend using CGI.
---

## 🛡️ Compilation & Execution
### Clone the repo
```bash 
git clone https://github.com/Mitxh13/Task-Scheduler.git cd Task-Scheduler
```
## 🐳 Run via Docker

A ready-to-use Dockerfile can host both the backend (C CGI) and frontend.

```bash
docker build -t task-scheduler .
docker run -p 8080:80 task-scheduler
```
Then visit **[http://localhost:8080](http://localhost:8080)**

---
## 🎨 Frontend Features
* Intuitive and clean UI built with **HTML + CSS (Glassmorphic design)**
* Smooth interactions using **vanilla JavaScript**
* Real-time updates after each action (via Fetch API)
* Status color indicators:
  * 🟢 **Completed**
  * 🟠 **Pending**
