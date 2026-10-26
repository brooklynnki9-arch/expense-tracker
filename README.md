# Expense Tracker

A simple C++ console app that records your income and expenses, sorts spending into categories, lets you set monthly budgets, tracks savings goals, and shows how much money you have left.

## Features

- Add income and expenses (each entry is stamped with today's date)
- Sort expenses into four categories: **Food**, **Rent**, **Transport**, **Other**
- Set a monthly budget for each category and get a warning when you go over
- Monthly reports with totals, a category breakdown, percentages, and budget status
- Savings goals with a progress bar showing how close you are to each one
- List, fix, or delete past entries
- Everything is saved to CSV files, so your data is still there next time you run it
- An all-time summary and your savings goals are printed when you quit

## How to compile and run

You need a C++ compiler such as `g++` (included with most Linux systems, with Xcode Command Line Tools on macOS, or with MinGW/MSYS2 on Windows).

Open a terminal in this folder and run:

```bash
g++ -std=c++11 -o expenseTracker main.cpp tracker.cpp goals.cpp
```

Then start the program:

```bash
./expenseTracker        # macOS / Linux
expenseTracker.exe      # Windows
```

All three `.cpp` files must be listed, because the program is split across them.

## How the code is organized

| File | What's inside |
|------|---------------|
| `main.cpp` | The main menu. Each letter calls one function from the other files. |
| `tracker.h` / `tracker.cpp` | Income, expenses, budgets, reports, and saving them to CSV files |
| `goals.h` / `goals.cpp` | Savings goals and the savings goals menu |

The `.h` (header) files list the functions and structs each `.cpp` file provides, so the other files can use them.

## Menu options

When the program starts, it shows this menu. Type a letter and press Enter (upper or lower case both work).

| Key | What it does |
|-----|--------------|
| `I` | Add income. You type the amount. |
| `E` | Add an expense. You type the amount and a category (Food, Rent, Transport, or Other). Anything else counts as Other. |
| `R` | Show a report for one month. Type a month like `2026-10`, or just press Enter for the current month. |
| `B` | Set a monthly budget for a category. Type `0` to remove the budget. |
| `G` | Open the savings goals menu (see below). |
| `L` | List all entries with a number next to each one. |
| `F` | Fix an entry: pick its number, then type the correct amount (and category, for expenses). |
| `D` | Delete an entry: pick its number and confirm with `y`. |
| `Q` | Quit and print an all-time summary. |

### Savings goals menu

Press `G` to see your goals and manage them. Press Enter on its own to go back to the main menu.

| Key | What it does |
|-----|--------------|
| `N` | New goal. Type what you're saving for, the target amount, and how much you've saved already. |
| `A` | Add money to a goal. |
| `T` | Take money out of a goal (it never goes below $0). |
| `D` | Delete a goal: pick its number and confirm with `y`. |

Each goal shows a progress bar:

```
===== Savings goals =====
1. New laptop
   [#####---------------]  25.00%   $200.00 of $800.00
   $600.00 to go
2. Trip
   [####################] 100.00%   $500.00 of $500.00
   Goal reached!
```

Savings goals are tracked on their own. Adding money to a goal does not change your income, expenses, or "Money left" total.

## Example

```
===== Report for 2026-10 =====
Total income:   $   2000.00
Total expenses: $   1250.00
Money left:     $    750.00

Category          Amount   Percent      Budget
----------------------------------------------
Food        $     200.00     16.00%  $   150.00  OVER
Rent        $     900.00     72.00%  $   900.00  <-- biggest
Transport   $     100.00      8.00%            -
Other       $      50.00      4.00%            -
```

## Where your data is saved

The program creates three files in the folder you run it from:

- `transactions.csv` holds every income and expense entry (date, type, category, amount)
- `budgets.csv` holds your monthly budget for each category
- `goals.csv` holds each savings goal (name, target, amount saved)

You can open these in a spreadsheet program. They are listed in `.gitignore` so your personal data isn't uploaded to GitHub.
