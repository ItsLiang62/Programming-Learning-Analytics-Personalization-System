// TP076334 WANG LIANG XUAN

#ifndef SESSION_CIRCULAR_QUEUE_HPP
#define SESSION_CIRCULAR_QUEUE_HPP

#include <iostream>
#include <string>
#include <stdexcept>
using namespace std;

const int MAX_SESSIONS = 5;

class SessionCircularQueue {
    private:
        string usernames[MAX_SESSIONS];
        int front, rear;

    public:
        SessionCircularQueue() {
            front = -1;
            rear = -1;
        };

        void enqueue(const string& username) {
            if (hasLearner(username)) {
                return;
            }
            
            if (isFull()) {
                throw std::out_of_range("!! SessionCircularQueue Full !!");
            } else if (isEmpty()) {
                front = rear = 0;
            } else {
                rear = (rear + 1) % MAX_SESSIONS;
            }  
            
            usernames[rear] = username;
        };

        string dequeue() {
            
            if (isEmpty()) {
                throw std::out_of_range("!! SessionCircularQueue Empty !!");
            } else {
                string username = usernames[front];
                if (front == rear) {
                    front = rear = -1;
                } else {
                    front = (front + 1) % MAX_SESSIONS;
                }
                return username;
            }
        };

        bool isFull() const {
            return (rear + 1) % MAX_SESSIONS == front; 
        };

        bool isEmpty() const {
            return front == -1;
        };

        void display() const {
            if (isEmpty()) {
                cout << "--- No Active Sessions ---" << endl;
                return;
            }
            cout << "Active Sessions: ";
            int i = front;
            while (true) {
                cout << usernames[i] << " ";
                if (i == rear) break;
                i = (i + 1) % MAX_SESSIONS;
            }
            cout << endl;
        }

        bool hasLearner(const string& username) {
            if (isEmpty()) return false;

            int i=front;

            while (true) {
                if (usernames[i] == username) return true;
                if (i == rear) break;
                i = (i + 1) % MAX_SESSIONS;
            }
            return false;
        }

        string peek() {
            if (!isEmpty()) {
                return usernames[front];
            } else {
                throw out_of_range("!! SessionCircularQueue Empty !!");
            }
        }
};

extern SessionCircularQueue sessions;

#endif