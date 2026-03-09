#include <iostream>
#include <string>
#include <regex>
#include <fstream>
#include <limits>
#include "SessionCircularQueue.hpp"
#include "WaitingListCircularQueue.hpp"
#include "RiskyLearnerPriorityQueue.hpp"
#include "ActivityStack.hpp"
#include "Structs.hpp"


using namespace std;

SessionCircularQueue sessions;
WaitingListCircularQueue waitingList;

void appendCsv(string filePath, string csvLine) {
    // TP076334 WANG LIANG XUAN

    fstream file(filePath, ios::out | ios::app);

    if (!file.is_open()) {
        throw runtime_error(filePath + " could not be opened");
    }

    file << csvLine << endl;
    file.close();
}

void registration() {
    // TP076334 WANG LIANG XUAN

    cout << "Please enter your username" << endl;
    string username;
    cin >> username;

    regex alphabetOnly("[a-zA-Z]+");
    bool hasAlphabetOnly = regex_match(username, alphabetOnly);

    ifstream file("learner.csv");
    if (!file.is_open()) {
        throw runtime_error("!! learner.csv Could Not Be Opened !!");
    }

    string csvUsername;
    bool isUnique = true;
    while (getline(file, csvUsername)) {
        if (username == csvUsername) {
            isUnique = false;
        }
    }

    if (hasAlphabetOnly && isUnique) {
        try {
            appendCsv("learner.csv", username);
            cout << "--- Registration Successful. ---" << endl;
        } catch (const runtime_error& r) {
            cout << r.what() << endl;;
        }
    } else {
        cout << "--- Registration Unsuccessful. ---" << endl;
    }
}

string login() {
    // TP076334 WANG LIANG XUAN

    string username;
    cout << "Please enter your username" << endl;
    cin >> username;

    ifstream file("learner.csv");
    if (!file.is_open()) {
        throw runtime_error("learner.csv cound not be opened");
    }

    string csvUsername;
    while (getline(file, csvUsername)) {
        if (csvUsername == username) { // Username found
            if (sessions.hasLearner(username)) { // Username in session
                cout << "--- Login Successful ---" << endl;
                return username;
            }
            // Username not in session
            // First in waiting list highest priority
            if (!waitingList.isEmpty() && waitingList.peek() == username) { 
                // Username is first in waiting list
                try {
                    sessions.enqueue(username);
                } catch (out_of_range) {
                    cout << "--- Login Unsuccessful. Session Full. ---" << endl;
                    return username;
                }
                waitingList.dequeue();
                cout << "--- Login Successful ---" << endl;
                return username;
            }

            // Username is not first in waiting list
            if (waitingList.isEmpty()) { // Because waiting list is empty
                try {
                    sessions.enqueue(username);
                    cout << "--- Login Successful ---" << endl;                
                } catch (out_of_range) {
                    try {
                        waitingList.enqueue(username);
                        cout << "--- Added Into Waiting List ---" << endl;
                    } catch (const out_of_range&) {
                        cout << "--- Login Unsuccessful. Waiting List Full. ---" << endl;
                    }
                }
            // Waiting list is not empty and username is not first in waiting list
            } else {
                try {
                    waitingList.enqueue(username);
                    cout << "--- Added Into Waiting List ---" << endl;
                } catch (const out_of_range&) {
                    cout << "--- Login Unsuccessful. Waiting List Full. ---" << endl;
                }
            }
            return username;
        }
    }
    cout << "--- Login Unsuccessful. Username Not Found. ---" << endl;
    return username;
}

int activity(const string& username) {
    // TP074952 ADRIAN LIEW REN QIAN

    ActivityStack navStack;
    int currentIndex = 0;

    while (true) {
        cout << "\n--- Activities Starting for " << username << "... ---" << endl;
        cout << "Current Activity: " << currentIndex + 1 << endl;
        cout << "Difficulty: " << activities[currentIndex].difficulty << endl;

        cout << "\nPlease select:" << endl;
        cout << "1. Start Current Activity" << endl;
        cout << "2. Next Activity" << endl;
        cout << "3. Back / Undo Previous Activity" << endl;
        cout << "4. View Activity History" << endl;
        cout << "5. Home (remain logged in)" << endl;
        cout << "6. Home (logout if it is your turn)" << endl;

        int selection;
        cin >> selection;

        if (cin.fail()) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "--- Please enter a valid number. ---" << endl;
            continue;
        }

        if (selection == 1) {
            string userAns = "";

            cout << "\n=== Activity " << currentIndex + 1 << " ===" << endl;

            for (int i = 0; i < activities[currentIndex].questions.size(); i++) {
                char ans;
                cout << "\nQ" << i + 1 << ": " << activities[currentIndex].questions[i] << endl;
                cout << "Your answer: ";
                cin >> ans;
                ans = toupper(ans);
                userAns += ans;
            }

            ActivityState state = {currentIndex, username, userAns};
            navStack.push(state);

            double score = activities[currentIndex].calcScore(userAns);

            cout << "\nYour Answers: " << userAns << endl;
            cout << "Correct Answers: " << activities[currentIndex].correctAns << endl;
            cout << "Score: " << score << "%" << endl;
            cout << "Result: " 
                 << (activities[currentIndex].isPass(score) ? "PASS" : "FAIL") 
                 << endl;

            //recordAttempt(username, activities[currentIndex], userAns);
        }
        else if (selection == 2) {
            if (currentIndex < 4) {
                currentIndex++;
                cout << "--- Moved to Next Activity. ---" << endl;
        } else {
                cout << "--- This is the last activity. ---" << endl;
        }
    }
        else if (selection == 3) {
            if (!navStack.isEmpty()) {
                ActivityState previous = navStack.peek();
                currentIndex = previous.activityIndex;
                navStack.pop();
                cout << "--- Returned to Previous Activity. ---" << endl;
            } else {
                cout << "--- No previous activity in history. ---" << endl;
            }
        }
        else if (selection == 4) {
            navStack.display();
        }
        else if (selection == 5) {
            return 1;
        }
        else if (selection == 6) {
            return 2;
        }
        else {
            cout << "--- Invalid selection. ---" << endl;
        }
    }
}

void recordAttempt(const string& username, Activity activity, const string& userAns) {
    // TP077245 GAVIN YONG DEE XIN

}

void displayAtRiskLearners() {
    RiskyLearnerPriorityQueue riskyLearners;

    ifstream file("learner.csv");
    if (!file.is_open()) {
        throw runtime_error("learner.csv could not be opened");
    }

    string csvUsername;
    while (getline(file, csvUsername)) {
        LearnerPerformance learnerPerf(csvUsername);
        riskyLearners.enqueue(learnerPerf);
    }
    
    riskyLearners.display();
}

void home() {
    // TP076334 WANG LIANG XUAN

    while (true) {
        sessions.display();
        waitingList.display();

        cout << "Welcome to Programming Learning Analytics and Personalization System (PLAPS)" << endl;
        cout << "Please select: " << endl;
        cout << "1. Registration" <<  endl;
        cout << "2. Login" << endl;
        cout << "3. Display At-Risk Learners" << endl;
        cout << "4. Exit" << endl;
        
        int selection;
        cin >> selection;

        if (cin.fail()) {
            cin.clear();  
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "--- Please select only 1 or 2. ---" << endl;
            continue;
        }

        if (selection == 1) {
            registration();
            continue;
        } else if (selection == 2) {
            string username = login();
            if (sessions.hasLearner(username)) {
                int signal = activity(username);
                if (!sessions.isEmpty() && signal == 2 && sessions.peek() == username) {
                    sessions.dequeue();
                }
                continue;
            } else {
                continue;
            }
        } else if (selection = 3) {
            displayAtRiskLearners();
            

        } else {
            cout << "--- Please select only 1 or 2. ---" << endl;
            continue;
        }
    }
}

int main() {
    home();
}