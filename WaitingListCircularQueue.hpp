    #ifndef SESSION_WAITING_LIST_QUEUE_HPP
    #define SESSION_WAITING_LIST_QUEUE_HPP

    #include <iostream>
    #include <string>
    #include <stdexcept>
    using namespace std;

    const int MAX_WAITING = 5;

    class WaitingListCircularQueue {
        private:
            string usernames[MAX_WAITING];
            int front, rear;

        public:
            WaitingListCircularQueue() {
                front = -1;
                rear = -1;
            };

            void enqueue(const string& username) {
                if (hasLearner(username)) {
                    return;
                }

                if (isFull()) {
                    throw std::out_of_range("!! WaitingListCircularQueue Full !!");
                } else if (isEmpty()) {
                    front = rear = 0;
                } else {
                    rear = (rear + 1) % MAX_WAITING;
                }  
                usernames[rear] = username;
            };

            string dequeue() {
                
                if (isEmpty()) {
                    throw std::out_of_range("!! WaitingListCircularQueue Empty !!");
                } else {
                    string username = usernames[front];
                    if (front == rear) {
                        front = rear = -1;
                    } else {
                        front = (front + 1) % MAX_WAITING;
                    }
                    return username;
                }
            };

            bool isFull() const {
                return (rear + 1) % MAX_WAITING == front; 
            };

            bool isEmpty() const {
                return front == -1;
            };

            void display() const {
                if (isEmpty()) {
                    cout << "--- Waiting List Empty ---" << endl;
                    return;
                }
                cout << "Waiting List: ";
                int i = front;
                while (true) {
                    cout << usernames[i] << " ";
                    if (i == rear) break;
                    i = (i + 1) % MAX_WAITING;
                }
                cout << endl;
            }

            bool hasLearner(const string& username) {
                for (const string& u: usernames) {
                    if (u == username) {
                        return true;
                    }
                }
                return false;
            }

            string peek() {
                if (!isEmpty()) {
                    return usernames[front];
                } else {
                    throw out_of_range("!! WaitingListCircularQueue Empty !!");
                }
            }
    };

    extern WaitingListCircularQueue waitingList;

    #endif