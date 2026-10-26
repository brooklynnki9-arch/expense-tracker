# Expense Tracker

A simple C++ console app that records your income and expenses, sorts spending into categories, lets you set monthly budgets, and shows how much money you have left.

## Features

- Add income and expenses (each entry is stamped with today's date)
- Sort expenses into four categories: **Food**, **Rent**, **Transport**, **Other**
- Set a monthly budget for each category and get a warning when you go over
- Monthly reports with totals, a category breakdown, percentages, and budget status
- List, fix, or delete past entries
- Everything is saved to CSV files, so your data is still there next time you run it
- An all-time summary is printed when you quit

## How to compile and run

You need a C++ compiler such as `g++` (included with most Linux systems, with Xcode Command Line Tools on macOS, or with MinGW/MSYS2 on Windows).

Open a terminal in this folder and run:

```bash
g++ -std=c++11 -o expenseTracker expenseTracker.cpp
```

Then start the program:

```bash
./expenseTracker        # macOS / Linux
expenseTracker.exe      # Windows
```

## Menu options

When the program starts, it shows this menu. Type a letter and press Enter (upper or lower case both work).

| Key | What it does |
|-----|--------------|
| `I` | Add income. You type the amount. |
| `E` | Add an expense. You type the amount and a category (Food, Rent, Transport, or Other). Anything else counts as Other. |
| `R` | Show a report for one month. Type a month like `2026-10`, or just press Enter for the current month. |
| `B` | Set a monthly budget for a category. Type `0` to remove the budget. |
| `L` | List all entries with a number next to each one. |
| `F` | Fix an entry: pick its number, then type the correct amount (and category, for expenses). |
| `D` | Delete an entry: pick its number and confirm with `y`. |
| `Q` | Quit and print an all-time summary. |

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

The program creates two files in the folder you run it from:

- `transactions.csv` holds every income and expense entry (date, type, category, amount)
- `budgets.csv` holds your monthly budget for each category

You can open these in a spreadsheet program. They are listed in `.gitignore` so your personal data isn't uploaded to GitHub.

## Website

The `web/` folder is a small website built around a browser version of the same tracker. The tracker page has the same features as the C++ program (income, expenses, the four categories, monthly budgets with warnings, monthly and all-time reports, and fixing or deleting entries) and needs no compiler.

To try it, open `web/index.html` in any web browser. Your entries are saved in that browser (using `localStorage`), so they stay there when you come back, but they are not shared with anyone and are not mixed with the C++ program's CSV files.

The pages are:

- `web/index.html` is the home page, with a quick look at this month's totals
- `web/tracker.html` is the tracker itself
- `web/tips.html` has budgeting tips
- `web/about.html` explains the project and how your data stays private

The other files are:

- `web/style.css` holds the colors and spacing for every page
- `web/app.js` holds the tracker's program logic
- `web/home.js` fills in the quick look on the home page

### Put it online with GitHub Pages

1. On GitHub, open this repository and click **Settings**, then **Pages**.
2. Under **Build and deployment**, set **Source** to **Deploy from a branch**.
3. Pick the `main` branch and the `/ (root)` folder, then click **Save**.
4. After a minute or two, the website is live at `https://<your-username>.github.io/expense-tracker/web/`.

Everyone who opens that link gets their own private copy of the data in their own browser.
