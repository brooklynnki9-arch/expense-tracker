// tracker.cpp
// Income, expenses, monthly budgets and reports.
// Transactions are saved to transactions.csv and budgets to budgets.csv.

#include "tracker.h"
#include <iostream>
#include <iomanip>
#include <fstream>
#include <sstream>
#include <ctime>
#include <cctype>

const string FILE_NAME = "transactions.csv";
const string BUDGET_FILE = "budgets.csv";

vector<Transaction> transactions;
string names[NUM_CATEGORIES] = {"Food", "Rent", "Transport", "Other"};
double budgets[NUM_CATEGORIES] = {0, 0, 0, 0};

// ---------- Small helpers ----------

// Returns today's date as YYYY-MM-DD
string todayDate()
{
    time_t now = time(0);
    tm* local = localtime(&now);
    char buffer[11];
    strftime(buffer, sizeof(buffer), "%Y-%m-%d", local);
    return buffer;
}

// Returns this month as YYYY-MM
string thisMonth()
{
    return todayDate().substr(0, 7);
}

// Turns "food", "FOOD", etc. into a number 0-3 (anything unknown is Other)
int categoryIndex(string category)
{
    for (int i = 0; i < (int)category.size(); i++)
        category[i] = tolower(category[i]);

    if (category == "food") return 0;
    if (category == "rent") return 1;
    if (category == "transport") return 2;
    return 3;
}

// Reads a number typed by the user
double readAmount(string prompt)
{
    double amount;
    cout << prompt;
    while (!(cin >> amount) || amount < 0)
    {
        cin.clear();
        cin.ignore(1000, '\n');
        cout << "Please type a positive number: ";
    }
    cin.ignore(1000, '\n');
    return amount;
}

// Asks for a category and returns its proper name, e.g. "Food"
string readCategory(string prompt)
{
    string category;
    cout << prompt;
    getline(cin, category);
    return names[categoryIndex(category)];
}

// ---------- Saving and loading ----------

// Reads all past transactions from the file (if it exists)
void loadTransactions()
{
    ifstream file(FILE_NAME);
    string line;
    while (getline(file, line))
    {
        stringstream ss(line);
        Transaction t;
        string amountText;
        getline(ss, t.date, ',');
        getline(ss, t.type, ',');
        getline(ss, t.category, ',');
        getline(ss, amountText, ',');

        if (amountText == "")
            continue; // skip blank or broken lines

        t.amount = stod(amountText);
        transactions.push_back(t);
    }
}

// Writes every transaction back to the file (used after add, delete or fix)
void saveTransactions()
{
    ofstream file(FILE_NAME);
    for (int i = 0; i < (int)transactions.size(); i++)
    {
        Transaction t = transactions[i];
        file << t.date << "," << t.type << "," << t.category << "," << t.amount << endl;
    }
}

void loadBudgets()
{
    ifstream file(BUDGET_FILE);
    string line;
    while (getline(file, line))
    {
        stringstream ss(line);
        string category, amountText;
        getline(ss, category, ',');
        getline(ss, amountText, ',');
        if (amountText != "")
            budgets[categoryIndex(category)] = stod(amountText);
    }
}

void saveBudgets()
{
    ofstream file(BUDGET_FILE);
    for (int i = 0; i < NUM_CATEGORIES; i++)
        file << names[i] << "," << budgets[i] << endl;
}

// ---------- Menu actions ----------

void addIncome()
{
    Transaction t;
    t.amount = readAmount("Income amount: $");
    t.date = todayDate();
    t.type = "Income";
    t.category = "Income";
    transactions.push_back(t);
    saveTransactions();
}

void addExpense()
{
    Transaction t;
    t.amount = readAmount("Expense amount: $");
    t.category = readCategory("Category (Food, Rent, Transport, Other): ");
    t.date = todayDate();
    t.type = "Expense";
    transactions.push_back(t);
    saveTransactions();
    checkBudget(categoryIndex(t.category));
}

void setBudget()
{
    int cat = categoryIndex(readCategory("Category (Food, Rent, Transport, Other): "));
    budgets[cat] = readAmount("Monthly budget for " + names[cat] + " (0 for none): $");
    saveBudgets();
    cout << "Budget saved." << endl;
}

void showMonthReport()
{
    string month;
    cout << "Month (YYYY-MM), or press Enter for this month: ";
    getline(cin, month);
    if (month == "")
        month = thisMonth();
    printReport(month);
}

// Shows every transaction with a number so the user can pick one
void listTransactions()
{
    cout << fixed << setprecision(2);
    cout << "\n#   Date        Type     Category       Amount" << endl;
    for (int i = 0; i < (int)transactions.size(); i++)
    {
        Transaction t = transactions[i];
        cout << left << setw(4) << i + 1 << setw(12) << t.date
             << setw(9) << t.type << setw(10) << t.category
             << right << "$" << setw(10) << t.amount << endl;
    }
}

// Asks which entry to change. Returns its position, or -1 to cancel.
int pickTransaction()
{
    if (transactions.size() == 0)
    {
        cout << "There are no entries yet." << endl;
        return -1;
    }
    listTransactions();
    double number = readAmount("Entry number (0 to cancel): ");
    int index = (int)number - 1;
    if (index < 0 || index >= (int)transactions.size())
        return -1;
    return index;
}

void fixTransaction()
{
    int index = pickTransaction();
    if (index == -1)
        return;

    transactions[index].amount = readAmount("Correct amount: $");
    if (transactions[index].type == "Expense")
        transactions[index].category = readCategory("Correct category (Food, Rent, Transport, Other): ");
    saveTransactions();
    cout << "Entry fixed." << endl;
}

void deleteTransaction()
{
    int index = pickTransaction();
    if (index == -1)
        return;

    string answer;
    cout << "Delete entry " << index + 1 << "? (y/n): ";
    getline(cin, answer);
    if (answer == "y" || answer == "Y")
    {
        transactions.erase(transactions.begin() + index);
        saveTransactions();
        cout << "Entry deleted." << endl;
    }
}

// ---------- Reports ----------

// How much was spent in one category during one month ("YYYY-MM")
double spentInMonth(int cat, string month)
{
    double total = 0;
    for (int i = 0; i < (int)transactions.size(); i++)
    {
        Transaction t = transactions[i];
        if (t.type == "Expense" && categoryIndex(t.category) == cat
            && t.date.substr(0, 7) == month)
            total += t.amount;
    }
    return total;
}

// Prints a warning if this month's spending in a category is over budget
void checkBudget(int cat)
{
    double spent = spentInMonth(cat, thisMonth());
    if (budgets[cat] > 0 && spent > budgets[cat])
    {
        cout << "WARNING: You are over your " << names[cat] << " budget by $"
             << fixed << setprecision(2) << spent - budgets[cat]
             << " this month!" << endl;
    }
}

// Prints totals and the category table.
// month is "YYYY-MM" for one month, or "" for all time.
void printReport(string month)
{
    double totalIncome = 0, totalExpenses = 0;
    double amounts[NUM_CATEGORIES] = {0, 0, 0, 0};

    for (int i = 0; i < (int)transactions.size(); i++)
    {
        Transaction t = transactions[i];
        if (month != "" && t.date.substr(0, 7) != month)
            continue; // not in the month we want

        if (t.type == "Income")
            totalIncome += t.amount;
        else
        {
            amounts[categoryIndex(t.category)] += t.amount;
            totalExpenses += t.amount;
        }
    }

    cout << fixed << setprecision(2);
    if (month == "")
        cout << "\n===== Summary (all time) =====" << endl;
    else
        cout << "\n===== Report for " << month << " =====" << endl;

    cout << "Total income:   $" << setw(10) << totalIncome << endl;
    cout << "Total expenses: $" << setw(10) << totalExpenses << endl;
    cout << "Money left:     $" << setw(10) << totalIncome - totalExpenses << endl;

    // Find the biggest category
    int biggest = 0;
    for (int i = 1; i < NUM_CATEGORIES; i++)
    {
        if (amounts[i] > amounts[biggest])
            biggest = i;
    }

    cout << "\n" << left << setw(12) << "Category"
         << right << setw(12) << "Amount"
         << setw(10) << "Percent";
    if (month != "")
        cout << setw(12) << "Budget";
    cout << endl;
    cout << "----------------------------------------------" << endl;

    for (int i = 0; i < NUM_CATEGORIES; i++)
    {
        double percent = 0;
        if (totalExpenses > 0)
            percent = amounts[i] / totalExpenses * 100;

        cout << left << setw(12) << names[i]
             << right << "$" << setw(11) << amounts[i]
             << setw(9) << percent << "%";

        // Budgets are monthly, so only show them in a month report
        if (month != "")
        {
            if (budgets[i] > 0)
                cout << "  $" << setw(9) << budgets[i];
            else
                cout << setw(12) << "-";
            if (budgets[i] > 0 && amounts[i] > budgets[i])
                cout << "  OVER";
        }

        if (totalExpenses > 0 && i == biggest)
            cout << "  <-- biggest";

        cout << endl;
    }
}
