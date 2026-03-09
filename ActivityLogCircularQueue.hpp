#ifndef ACTIVITYLOGCIRCULARQUEUE_HPP
#define ACTIVITYLOGCIRCULARQUEUE_HPP

#include "Structs.hpp"
#include <iostream>
#include <fstream>

class ActivityLogCircularQueue {
private:
    Attempt* logArray;
    int front;
    int rear;
    int maxSize;
    int currentSize;

public:
    ActivityLogCircularQueue(int size = 100) {
        maxSize = size;
        logArray = (Attempt*)operator new[](maxSize * sizeof(Attempt)); // Memory allocation without calling default constructor
        front = 0;
        rear = -1;
        currentSize = 0;
    }

    // Task 3: Store activity attempt (Overwrite if full)
    void enqueue(const Attempt& attempt) {
        if (currentSize == maxSize) {
            // Log is full, overwrite the oldest (front)
            front = (front + 1) % maxSize;
        } else {
            currentSize++;
        }
        rear = (rear + 1) % maxSize;
        logArray[rear] = attempt;
        
        // Task 3: Export to CSV immediately for persistence
        saveToCsv(attempt);
    }

    // Task 3: View all current activity logs
    void displayAll() const {
        if (currentSize == 0) {
            std::cout << "No activity logs available." << std::endl;
            return;
        }
        int temp = front;
        for (int i = 0; i < currentSize; i++) {
            std::cout << "User: " << logArray[temp].username 
                      << " | Score: " << logArray[temp].score 
                      << " | Result: " << (logArray[temp].isPass ? "Pass" : "Fail") << std::endl;
            temp = (temp + 1) % maxSize;
        }
    }

    // Task 3: Filter logs for a specific learner
    void filterByLearner(const std::string& username) const {
        bool found = false;
        int temp = front;
        for (int i = 0; i < currentSize; i++) {
            if (logArray[temp].username == username) {
                std::cout << "Topic: " << logArray[temp].activity.difficulty 
                          << " | Score: " << logArray[temp].score << std::endl;
                found = true;
            }
            temp = (temp + 1) % maxSize;
        }
        if (!found) std::cout << "No logs found for user: " << username << std::endl;
    }

    // Task 3: Export records to CSV
    void saveToCsv(const Attempt& attempt) const {
        std::ofstream file("attempt.csv", std::ios::app);
        if (file.is_open()) {
            file << attempt.toCsv() << std::endl;
            file.close();
        }
    }
};

#endif