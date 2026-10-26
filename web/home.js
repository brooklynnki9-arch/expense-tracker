// Home page: if you already have entries saved by the tracker,
// show a quick look at this month's totals.

const savedEntries = localStorage.getItem("entries");

if (savedEntries) {
  const entries = JSON.parse(savedEntries);

  // This month as "YYYY-MM" in your own time zone
  const now = new Date();
  const thisMonth = now.getFullYear() + "-" + String(now.getMonth() + 1).padStart(2, "0");

  let income = 0;
  let expenses = 0;
  for (const entry of entries) {
    if (entry.date.startsWith(thisMonth)) {
      if (entry.type === "Income") {
        income += entry.amount;
      } else {
        expenses += entry.amount;
      }
    }
  }

  if (entries.length > 0) {
    document.getElementById("quickIncome").textContent = "$" + income.toFixed(2);
    document.getElementById("quickExpenses").textContent = "$" + expenses.toFixed(2);
    document.getElementById("quickLeft").textContent = "$" + (income - expenses).toFixed(2);
    document.getElementById("quickLook").hidden = false;
  }
}
