// 
// 234218 Data Structures 1.
// Semester: 2025A (Winter).
// Wet Exercise #1.
// 
// The following main file is necessary to link and run your code.
// This file is READ ONLY: even if you submit something else, the compiler will use our file.
// 

#include "plains25a1.h"
#include <string>
#include <iostream>

using namespace std;

void print(string cmd, StatusType res);
void print(string cmd, output_t<int> res);
void print(string cmd, output_t<bool> res);

int main()
{
    
    int d1, d2;

    // Init
    Plains *obj = new Plains();
    
    // Execute all commands in file
    string op;
    while (cin >> op)
    {
        if (!op.compare("add_herd")) {
            cin >> d1;
            print(op, obj->add_herd(d1));
        } else if (!op.compare("remove_herd")) {
            cin >> d1;
            print(op, obj->remove_herd(d1));
        } else if (!op.compare("add_horse")) {
            cin >> d1 >> d2;
            print(op, obj->add_horse(d1, d2));
        } else if (!op.compare("join_herd")) {
            cin >> d1 >> d2;
            print(op, obj->join_herd(d1, d2));
        } else if (!op.compare("follow")) {
            cin >> d1 >> d2;
            print(op, obj->follow(d1, d2));
        } else if (!op.compare("leave_herd")) {
            cin >> d1;
            print(op, obj->leave_herd(d1));
        } else if (!op.compare("get_speed")) {
            cin >> d1;
            print(op, obj->get_speed(d1));
        } else if (!op.compare("leads")) {
            cin >> d1 >> d2;
            print(op, obj->leads(d1, d2));
        } else if (!op.compare("can_run_together")) {
            cin >> d1;
            print(op, obj->can_run_together(d1));
        } else {
            cout << "Unknown command: " << op << endl;
            return -1;
        }
        // Verify no faults
        if (cin.fail()){
            cout << "Invalid input format" << endl;
            return -1;
        }
    }

    // Quit 
    delete obj;
    return 0;
}

// Helpers
static const char *StatusTypeStr[] =
{
    "SUCCESS",
    "ALLOCATION_ERROR",
    "INVALID_INPUT",
    "FAILURE"
};

void print(string cmd, StatusType res) 
{
    cout << cmd << ": " << StatusTypeStr[(int) res] << endl;
}

void print(string cmd, output_t<int> res)
{
    if (res.status() == StatusType::SUCCESS) {
        cout << cmd << ": " << StatusTypeStr[(int) res.status()] << ", " << res.ans() << endl;
    } else {
        cout << cmd << ": " << StatusTypeStr[(int) res.status()] << endl;
    }
}

void print(string cmd, output_t<bool> res)
{
    if (res.status() == StatusType::SUCCESS) {
        cout << cmd << ": " << StatusTypeStr[(int) res.status()] << ", " << (res.ans() ? "True" : "False") << endl;
    } else {
        cout << cmd << ": " << StatusTypeStr[(int) res.status()] << endl;
    }
}
