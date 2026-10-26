// goals.cpp
// Savings goals, saved to goals.csv so they are kept between runs.

#include "goals.h"
#include "tracker.h"
#include <iostream>
#include <iomanip>
#include <fstream>
#include <sstream>

const string GOAL_FILE = "goals.csv";
const int BAR_WIDTH = 20;

vector<Goal> goals;

// ---------- Saving and loading ----------

void loadGoals()
{
    ifstream file(GOAL_FILE);
    string line;
    while (getline(file, line))
    {
        stringstream ss(line);
        Goal g;
        string targetText, savedText;
        getline(ss, g.name, ',');
        getline(ss, targetText, ',');
        getline(ss, savedText, ',');

        if (targetText == "" || savedText == "")
            continue; // skip blank or broken lines

        g.target = stod(targetText);
        g.saved = stod(savedText);
        goals.push_back(g);
    }
}

void saveGoals()
{
    ofstream file(GOAL_FILE);
    for (int i = 0; i < (int)goals.size(); i++)
        file << goals[i].name << "," << goals[i].target << "," << goals[i].saved << endl;
}

// ---------- Showing progress ----------

// Returns a bar like [########------------] for how much of the goal is saved
string progressBar(double percent)
{
    int filled = (int)(percent / 100 * BAR_WIDTH);
    if (filled > BAR_WIDTH)
        filled = BAR_WIDTH;

    string bar = "[";
    for (int i = 0; i < BAR_WIDTH; i++)
    {
        if (i < filled)
            bar += "#";
        else
            bar += "-";
    }
    return bar + "]";
}

// Shows every goal with a number, a progress bar and how much is still needed
void listGoals()
{
    if (goals.size() == 0)
    {
        cout << "\nYou have no savings goals yet." << endl;
        return;
    }

    cout << fixed << setprecision(2);
    cout << "\n===== Savings goals =====" << endl;
    for (int i = 0; i < (int)goals.size(); i++)
    {
        Goal g = goals[i];
        double percent = 0;
        if (g.target > 0)
            percent = g.saved / g.target * 100;

        cout << i + 1 << ". " << g.name << endl;
        cout << "   " << progressBar(percent) << " " << setw(6) << percent << "%"
             << "   $" << g.saved << " of $" << g.target << endl;

        if (g.saved >= g.target)
            cout << "   Goal reached!" << endl;
        else
            cout << "   $" << g.target - g.saved << " to go" << endl;
    }
}

// ---------- Menu actions ----------

// Asks which goal to use. Returns its position, or -1 to cancel.
int pickGoal()
{
    if (goals.size() == 0)
    {
        cout << "There are no goals yet." << endl;
        return -1;
    }
    double number = readAmount("Goal number (0 to cancel): ");
    int index = (int)number - 1;
    if (index < 0 || index >= (int)goals.size())
        return -1;
    return index;
}

void addGoal()
{
    Goal g;
    cout << "What are you saving for? ";
    getline(cin, g.name);

    // Commas would break the CSV file, so swap them for spaces
    for (int i = 0; i < (int)g.name.size(); i++)
    {
        if (g.name[i] == ',')
            g.name[i] = ' ';
    }
    if (g.name == "")
        g.name = "My goal";

    g.target = readAmount("How much do you need? $");
    g.saved = readAmount("How much have you saved already? $");
    goals.push_back(g);
    saveGoals();
    cout << "Goal added." << endl;
}

// type is "add" to put money in, or "take" to take money out
void changeSaved(string type)
{
    int index = pickGoal();
    if (index == -1)
        return;

    if (type == "add")
    {
        goals[index].saved += readAmount("Amount to add: $");
        if (goals[index].saved >= goals[index].target)
            cout << "Congratulations, you reached your " << goals[index].name << " goal!" << endl;
    }
    else
    {
        double amount = readAmount("Amount to take out: $");
        if (amount > goals[index].saved)
            amount = goals[index].saved; // can't go below $0
        goals[index].saved -= amount;
    }
    saveGoals();
}

void deleteGoal()
{
    int index = pickGoal();
    if (index == -1)
        return;

    string answer;
    cout << "Delete goal \"" << goals[index].name << "\"? (y/n): ";
    getline(cin, answer);
    if (answer == "y" || answer == "Y")
    {
        goals.erase(goals.begin() + index);
        saveGoals();
        cout << "Goal deleted." << endl;
    }
}

// The savings goals menu. Keeps going until the user presses Enter.
void goalsMenu()
{
    string choice;
    while (true)
    {
        listGoals();
        cout << "\nN = new goal, A = add money, T = take money out, D = delete goal," << endl;
        cout << "or press Enter to go back: ";
        getline(cin, choice);

        if (choice == "")
            break;
        else if (choice == "N" || choice == "n")
            addGoal();
        else if (choice == "A" || choice == "a")
            changeSaved("add");
        else if (choice == "T" || choice == "t")
            changeSaved("take");
        else if (choice == "D" || choice == "d")
            deleteGoal();
        else
            cout << "Invalid choice. Try again." << endl;
    }
}
