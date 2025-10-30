const API = "/bin/scheduler.cgi";

async function postData(body) {
  const res = await fetch(API, {
    method: "POST",
    headers: { "Content-Type": "application/x-www-form-urlencoded" },
    body
  });
  const text = await res.text();
  try { return JSON.parse(text); } catch { return { text }; }
}

function getVal(id) {
  const el = document.getElementById(id);
  return el ? el.value.trim() : "";
}

function addTask() {
  const id = getVal("taskId");
  const details = getVal("taskDesc");
  const time = getVal("taskTime");
  if (!id || !details || !time) return alert("Fill all fields!");
  postData(`action=add&id=${encodeURIComponent(id)}&time=${encodeURIComponent(time)}&details=${encodeURIComponent(details)}`)
    .then(() => {
      loadTasks();
      // optional: clear fields
      // document.getElementById("taskDesc").value = "";
    })
    .catch(e => alert("Add failed: " + e));
}

function updateTask() {
  const id = getVal("taskId");
  const time = getVal("taskTime");
  if (!id || !time) return alert("Enter Task ID and new time!");
  postData(`action=update&id=${encodeURIComponent(id)}&newtime=${encodeURIComponent(time)}`)
    .then(() => loadTasks())
    .catch(e => alert("Update failed: " + e));
}

function cancelTask() {
  const id = getVal("taskId");
  if (!id) return alert("Enter Task ID!");
  postData(`action=cancel&id=${encodeURIComponent(id)}`)
    .then(() => loadTasks())
    .catch(e => alert("Cancel failed: " + e));
}

function serveTask() {
  const id = getVal("taskId");
  if (!id) return alert("Enter Task ID!");
  postData(`action=serve&id=${encodeURIComponent(id)}`)
    .then(() => loadTasks())
    .catch(e => alert("Serve failed: " + e));
}

function loadTasks() {
  fetch(`${API}?action=view`)
    .then(r => {
      if (!r.ok) throw new Error("Network response was not ok");
      return r.json();
    })
    .then(data => {
      const tbody = document.querySelector("#taskTable tbody");
      tbody.innerHTML = "";
      if (!data.tasks || data.tasks.length === 0) {
        tbody.innerHTML = "<tr><td colspan='4'>No tasks found</td></tr>";
        return;
      }
      data.tasks.forEach(t => {
        const safeDetails = (t.details ?? "").replace(/</g, "&lt;").replace(/>/g, "&gt;");
        const row = `<tr>
          <td>${t.id}</td>
          <td>${safeDetails}</td>
          <td>${t.time}</td>
          <td class="${t.done ? "done" : "pending"}">${t.done ? "✅ Completed" : "⏳ Pending"}</td>
        </tr>`;
        tbody.innerHTML += row;
      });
    })
    .catch(err => {
      console.error(err);
      const tbody = document.querySelector("#taskTable tbody");
      tbody.innerHTML = `<tr><td colspan='4'>Error loading tasks</td></tr>`;
    });
}

window.addEventListener("load", loadTasks);
