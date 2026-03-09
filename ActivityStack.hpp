#ifndef ACTIVITYSTACK_HPP
#define ACTIVITYSTACK_HPP

#include <iostream>
#include <string>
using namespace std;

const int MAX_ACTIVITY_STACK = 50;

struct ActivityState {
    int activityIndex;
    string username;
    string userAns;
};

class ActivityStack {
private:
    ActivityState arr[MAX_ACTIVITY_STACK];
    int top;

public:
    ActivityStack() {
        top = -1;
    }

    bool isEmpty() const {
        return top == -1;
    }

    bool isFull() const {
        return top == MAX_ACTIVITY_STACK - 1;
    }

    void push(const ActivityState& state) {
        if (isFull()) {
            cout << "--- Activity stack is full. ---" << endl;
            return;
        }
        arr[++top] = state;
    }

    void pop() {
        if (isEmpty()) {
            cout << "--- No previous activity to undo. ---" << endl;
            return;
        }
        top--;
    }

    ActivityState peek() const {
        if (isEmpty()) {
            return {-1, "", ""};
        }
        return arr[top];
    }

    void display() const {
        if (isEmpty()) {
            cout << "--- No activity history available. ---" << endl;
            return;
        }

        cout << "\n=== Activity Navigation History ===" << endl;
        for (int i = top; i >= 0; i--) {
            cout << "Activity " << arr[i].activityIndex + 1
                 << " | User: " << arr[i].username
                 << " | Answers: " << arr[i].userAns << endl;
        }
    }
};

#endif