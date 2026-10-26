// Expense Tracker - web version
// Works like the C++ program, but saves data in the browser (localStorage)
// instead of CSV files.

const CATEGORIES = ["Food", "Rent", "Transport", "Other"];

// Each entry looks like: { date: "2026-10-26", type: "Expense", category: "Food", amount: 12.5 }
let entries = [];

// Budgets look like: { Food: 150, Rent: 900, Transport: 0, Other: 0 }  (0 means no budget)
let budgets = { Food: 0, Rent: 0, Transport: 0, Other: 0 };

// When fixing an entry, this holds its position in the list. -1 means "adding a new one".
let editingIndex = -1;

// ---------- Saving and loading ----------

function loadData() {
  const savedEntries = localStorage.getItem("entries");
  const savedBudgets = localStorage.getItem("budgets");
  if (savedEntries) {
    entries = JSON.parse(savedEntries);
  }
  if (savedBudgets) {
    budgets = JSON.parse(savedBudgets);
  }
}

function saveData() {
  localStorage.setItem("entries", JSON.stringify(entries));
  localStorage.setItem("budgets", JSON.stringify(budgets));
}

// ---------- Small helpers ----------

// Turns 1234.5 into "$1234.50"
function money(amount) {
  return "$" + amount.toFixed(2);
}

// Today's date as "YYYY-MM-DD" in your own time zone
function todayDate() {
  const now = new Date();
  const month = String(now.getMonth() + 1).padStart(2, "0");
  const day = String(now.getDate()).padStart(2, "0");
  return now.getFullYear() + "-" + month + "-" + day;
}

// Total spent in one category during one month ("YYYY-MM")
function spentInMonth(category, month) {
  let total = 0;
  for (const entry of entries) {
    if (entry.type === "Expense" && entry.category === category && entry.date.startsWith(month)) {
      total += entry.amount;
    }
  }
  return total;
}

// ---------- Budget warning ----------

// Shows a warning for every category that is over budget this month
function checkBudgets() {
  const month = todayDate().substring(0, 7);
  const messages = [];

  for (const category of CATEGORIES) {
    const spent = spentInMonth(category, month);
    if (budgets[category] > 0 && spent > budgets[category]) {
      messages.push("You are over your " + category + " budget by " +
        money(spent - budgets[category]) + " this month!");
    }
  }

  const warning = document.getElementById("warning");
  warning.textContent = "WARNING: " + messages.join(" ");
  warning.hidden = messages.length === 0;
}

// ---------- Report ----------

function showReport() {
  const allTime = document.getElementById("allTime").checked;
  const month = document.getElementById("reportMonth").value; // "YYYY-MM"
  document.getElementById("reportMonth").disabled = allTime;

  let totalIncome = 0;
  let totalExpenses = 0;
  const amounts = { Food: 0, Rent: 0, Transport: 0, Other: 0 };

  for (const entry of entries) {
    // Skip entries from other months (unless "All time" is ticked)
    if (!allTime && !entry.date.startsWith(month)) {
      continue;
    }
    if (entry.type === "Income") {
      totalIncome += entry.amount;
    } else {
      totalExpenses += entry.amount;
      amounts[entry.category] += entry.amount;
    }
  }

  document.getElementById("totalIncome").textContent = money(totalIncome);
  document.getElementById("totalExpenses").textContent = money(totalExpenses);
  document.getElementById("moneyLeft").textContent = money(totalIncome - totalExpenses);

  // Find the category with the most spending
  let biggest = "";
  for (const category of CATEGORIES) {
    if (amounts[category] > 0 && (biggest === "" || amounts[category] > amounts[biggest])) {
      biggest = category;
    }
  }

  const body = document.getElementById("reportBody");
  body.innerHTML = "";

  for (const category of CATEGORIES) {
    let percent = 0;
    if (totalExpenses > 0) {
      percent = amounts[category] / totalExpenses * 100;
    }

    // Budgets are monthly, so only compare them in a monthly report
    let budgetText = "-";
    let note = "";
    if (!allTime && budgets[category] > 0) {
      budgetText = money(budgets[category]);
      if (amounts[category] > budgets[category]) {
        note = "OVER";
      }
    }
    if (category === biggest) {
      note += (note ? " / " : "") + "biggest";
    }

    const row = document.createElement("tr");
    row.innerHTML =
      "<td>" + category + "</td>" +
      "<td>" + money(amounts[category]) + "</td>" +
      "<td>" + percent.toFixed(2) + "%</td>" +
      "<td>" + budgetText + "</td>" +
      "<td class='" + (note.includes("OVER") ? "over" : "") + "'>" + note + "</td>";
    body.appendChild(row);
  }
}

// ---------- List of entries ----------

function showEntries() {
  const list = document.getElementById("entryList");
  list.innerHTML = "";

  for (let i = 0; i < entries.length; i++) {
    const entry = entries[i];
    const row = document.createElement("tr");
    row.innerHTML =
      "<td>" + entry.date + "</td>" +
      "<td>" + entry.type + "</td>" +
      "<td>" + (entry.type === "Expense" ? entry.category : "-") + "</td>" +
      "<td>" + money(entry.amount) + "</td>" +
      "<td>" +
      "<button class='small' onclick='startFix(" + i + ")'>Fix</button> " +
      "<button class='small delete' onclick='deleteEntry(" + i + ")'>Delete</button>" +
      "</td>";
    list.appendChild(row);
  }

  document.getElementById("noEntries").hidden = entries.length > 0;
}

// Redraws everything on the page
function refresh() {
  showReport();
  showEntries();
  checkBudgets();
}

// ---------- Add, fix and delete ----------

// Hides the category box for income, since income has no category
function updateCategoryVisibility() {
  const type = document.getElementById("entryType").value;
  document.getElementById("categoryLabel").hidden = type === "Income";
}

function resetForm() {
  editingIndex = -1;
  document.getElementById("entryForm").reset();
  document.getElementById("entryDate").value = todayDate();
  document.getElementById("saveButton").textContent = "Add";
  document.getElementById("cancelButton").hidden = true;
  updateCategoryVisibility();
}

function saveEntry(event) {
  event.preventDefault(); // stop the page from reloading

  const type = document.getElementById("entryType").value;
  const entry = {
    date: document.getElementById("entryDate").value,
    type: type,
    category: type === "Expense" ? document.getElementById("entryCategory").value : "",
    amount: Number(document.getElementById("entryAmount").value)
  };

  if (editingIndex === -1) {
    entries.push(entry);            // new entry
  } else {
    entries[editingIndex] = entry;  // fixed entry
  }

  // Keep entries in date order
  entries.sort((a, b) => a.date.localeCompare(b.date));

  saveData();
  resetForm();
  refresh();
}

// Copies an entry into the form so you can change it
function startFix(index) {
  const entry = entries[index];
  editingIndex = index;
  document.getElementById("entryType").value = entry.type;
  document.getElementById("entryAmount").value = entry.amount;
  document.getElementById("entryCategory").value = entry.category || "Food";
  document.getElementById("entryDate").value = entry.date;
  document.getElementById("saveButton").textContent = "Save fix";
  document.getElementById("cancelButton").hidden = false;
  updateCategoryVisibility();
  window.scrollTo(0, 0);
}

function deleteEntry(index) {
  const entry = entries[index];
  if (confirm("Delete this " + entry.type.toLowerCase() + " of " + money(entry.amount) + "?")) {
    entries.splice(index, 1);
    saveData();
    resetForm();
    refresh();
  }
}

// ---------- Budgets ----------

function showBudgets() {
  const inputs = document.querySelectorAll("#budgetForm input");
  for (const input of inputs) {
    const value = budgets[input.dataset.category];
    input.value = value > 0 ? value : "";
  }
}

function saveBudgets(event) {
  event.preventDefault();
  const inputs = document.querySelectorAll("#budgetForm input");
  for (const input of inputs) {
    budgets[input.dataset.category] = Number(input.value) || 0;
  }
  saveData();
  refresh();
}

// ---------- Start the app ----------

loadData();
resetForm();
document.getElementById("reportMonth").value = todayDate().substring(0, 7);
showBudgets();
refresh();

document.getElementById("entryForm").addEventListener("submit", saveEntry);
document.getElementById("cancelButton").addEventListener("click", resetForm);
document.getElementById("entryType").addEventListener("change", updateCategoryVisibility);
document.getElementById("budgetForm").addEventListener("submit", saveBudgets);
document.getElementById("reportMonth").addEventListener("change", showReport);
document.getElementById("allTime").addEventListener("change", showReport);
