const API = "/cgi-bin/scheduler.cgi";

// Helper for POST requests
function postData(body) {
return fetch(API, {
method: "POST",
headers: { "Content-Type": "application/x-www-form-urlencoded" },
body
}).then(res => res.json());
}

// Add Task
function addTask() {
const id = document.getElementById("taskId").value;
const details = document.getElementById("taskDesc").value;
const time = document.getElementById("taskTime").value;

if (!id || !details || !time) return alert("Fill all fields!");
postData(`action=add&id=${id}&time=${time}&details=${encodeURIComponent(details)}`)
.then(() => loadTasks());
}

// Update submission time
function updateTask() {
const id = document.getElementById("taskId").value;
const time = document.getElementById("taskTime").value;
if (!id || !time) return alert("Enter Task ID and new time!");

postData(`action=update&id=${id}&newtime=${time}`)
.then(() => loadTasks());
}

// Cancel Task
function cancelTask() {
const id = document.getElementById("taskId").value;
if (!id) return alert("Enter Task ID to cancel!");

postData(`action=cancel&id=${id}`)
.then(() => loadTasks());
}

// Mark as complete
function serveTask() {
const id = document.getElementById("taskId").value;
if (!id) return alert("Enter Task ID to mark complete!");

postData(`action=serve&id=${id}`)
.then(() => loadTasks());
}

// Fetch and show tasks
function loadTasks() {
fetch(`${API}?action=view`)
.then(res => res.json())
.then(data => {
const tbody = document.querySelector("#taskTable tbody");
tbody.innerHTML = "";
  if (!data.tasks || data.tasks.length === 0) {
    tbody.innerHTML = "<tr><td colspan='4'>No tasks found</td></tr>";
    return;
  }

  data.tasks.forEach(t => {
    const row = document.createElement("tr");
    row.innerHTML = `
      <td>${t.id}</td>
      <td>${t.details}</td>
      <td>${t.time}</td>
      <td class="${t.done ? "done" : "pending"}">${t.done ? "✅ Completed" : "⏳ Pending"}</td>
    `;
    tbody.appendChild(row);
  });
})
.catch(err => console.error("Error loading tasks:", err));

}

// Auto-load on page start
window.onload = loadTasks;
