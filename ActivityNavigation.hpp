#ifndef ACTIVITYNAVIGATION_HPP
#define ACTIVITYNAVIGATION_HPP

#include "ActivityStack.hpp"
#include "Structs.hpp"

void doActivity(ActivityStack& navStack, const string& username) {
    int choice;

    while (true) {
        cout << "\n=== Activity Navigation Menu ===\n";
        cout << "1. Start Activity 1\n";
        cout << "2. Start Activity 2\n";
        cout << "3. Start Activity 3\n";
        cout << "4. Start Activity 4\n";
        cout << "5. Start Activity 5\n";
        cout << "6. Go Back / Undo Last Activity\n";
        cout << "7. View Navigation History\n";
        cout << "8. Exit\n";
        cout << "Enter choice: ";
        cin >> choice;

        if (choice >= 1 && choice <= 5) {
            int actIndex = choice - 1;
            string userAns = "";

            ActivityState state;
            state.activityIndex = actIndex;
            state.username = username;
            state.userAns = "";

            navStack.push(state);

            cout << "\n=== Activity " << choice << " ===\n";
            for (int i = 0; i < activities[actIndex].questions.size(); i++) {
                char ans;
                cout << "\nQ" << i + 1 << ": " << activities[actIndex].questions[i] << endl;
                cout << "Your answer: ";
                cin >> ans;
                ans = toupper(ans);
                userAns += ans;
            }

            ActivityState latest = navStack.peek();
            latest.userAns = userAns;
            navStack.pop();
            navStack.push(latest);

            double score = activities[actIndex].calcScore(userAns);

            cout << "\nYour answers: " << userAns << endl;
            cout << "Correct answers: " << activities[actIndex].correctAns << endl;
            cout << "Score: " << score << "%" << endl;

            if (activities[actIndex].isPass(score)) {
                cout << "Result: PASS\n";
            } else {
                cout << "Result: FAIL\n";
            }

           // recordAttempt(username, activities[actIndex], userAns);
        }
        else if (choice == 6) {
            navStack.pop();
        }
        else if (choice == 7) {
            navStack.display();
        }
        else if (choice == 8) {
            cout << "Exiting Activity Navigation...\n";
            break;
        }
        else {
            cout << "Invalid choice.\n";
        }
    }
}

#endif