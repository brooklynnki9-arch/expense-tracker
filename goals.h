// goals.h
// Savings goals: something you are saving up for, and how close you are.

#ifndef GOALS_H
#define GOALS_H

#include <string>
#include <vector>
using namespace std;

// One savings goal, e.g. "Laptop", $800 target, $250 saved so far
struct Goal
{
    string name;
    double target;
    double saved;
};

extern vector<Goal> goals;

void loadGoals();
void saveGoals();
void listGoals();
void goalsMenu();

#endif
