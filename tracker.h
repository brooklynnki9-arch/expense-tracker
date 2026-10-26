// tracker.h
// Everything to do with income, expenses and monthly budgets.

#ifndef TRACKER_H
#define TRACKER_H

#include <string>
#include <vector>
using namespace std;

// One income or expense entry
struct Transaction
{
    string date;     // YYYY-MM-DD
    string type;     // Income or Expense
    string category; // Food, Rent, Transport, Other (or Income)
    double amount;
};

const int NUM_CATEGORIES = 4;

// Shared data (the real variables live in tracker.cpp)
extern vector<Transaction> transactions;
extern string names[NUM_CATEGORIES];
extern double budgets[NUM_CATEGORIES]; // 0 means no budget set

// Small helpers
string todayDate();
string thisMonth();
int categoryIndex(string category);
double readAmount(string prompt);
string readCategory(string prompt);

// Saving and loading
void loadTransactions();
void saveTransactions();
void loadBudgets();
void saveBudgets();

// Menu actions
void addIncome();
void addExpense();
void setBudget();
void showMonthReport();
void listTransactions();
void fixTransaction();
void deleteTransaction();

// Reports (month is "YYYY-MM", or "" for all time)
double spentInMonth(int cat, string month);
void checkBudget(int cat);
void printReport(string month);

#endif
