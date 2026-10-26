// Expense Tracker
// Records income and expenses, sorts expenses into categories,
// keeps monthly budgets and savings goals, and shows how much money is left.
//
// The code is split into a few files:
//   main.cpp     - the main menu (this file)
//   tracker.cpp  - income, expenses, budgets and reports
//   goals.cpp    - savings goals

#include <iostream>
#include <string>
#include "tracker.h"
#include "goals.h"
using namespace std;

int main()
{
    string choice;

    cout << "===== Expense Tracker =====" << endl;

    loadTransactions();
    loadBudgets();
    loadGoals();
    cout << "Loaded " << transactions.size() << " past transaction(s) and "
         << goals.size() << " savings goal(s)." << endl;

    while (true)
    {
        cout << "\nI = income, E = expense, R = month report, B = set budget," << endl;
        cout << "G = savings goals, L = list entries, F = fix entry," << endl;
        cout << "D = delete entry, Q = quit: ";
        getline(cin, choice);

        if (choice == "Q" || choice == "q")
            break;
        else if (choice == "I" || choice == "i")
            addIncome();
        else if (choice == "E" || choice == "e")
            addExpense();
        else if (choice == "R" || choice == "r")
            showMonthReport();
        else if (choice == "B" || choice == "b")
            setBudget();
        else if (choice == "G" || choice == "g")
            goalsMenu();
        else if (choice == "L" || choice == "l")
            listTransactions();
        else if (choice == "F" || choice == "f")
            fixTransaction();
        else if (choice == "D" || choice == "d")
            deleteTransaction();
        else
            cout << "Invalid choice. Try again." << endl;
    }

    printReport("");
    listGoals();

    return 0;
}
