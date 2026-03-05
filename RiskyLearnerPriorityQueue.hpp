// TP076334 WANG LIANG XUAN

#ifndef RISKY_LEARNER_PRIORITY_QUEUE_HPP
#define RISKY_LEARNER_PRIORITY_QUEUE_HPP

#include <iostream>
#include <string>
#include <stdexcept>
#include <limits>
#include "Structs.hpp"

using namespace std;

const int MAX_RISKY_LEARNER = 10;

class RiskyLearnerPriorityQueue {
    private:
        LearnerPerformance learnerPerfs[MAX_RISKY_LEARNER];
        int rear;

        bool conditionallyRemoveLowestPriority(double newLearnerRiskScore) {
            if (!isFull()) throw runtime_error("!! RiskyLearnerPriorityQueue Not full !!");

            LearnerPerformance lowestPriority;
            double lowestRiskScore = numeric_limits<double>::max();
            int lowestIndex = -1;

            for (int i = 0; i <= rear; i++) {
                double score = learnerPerfs[i].calcRiskScore(); 
                if (score < lowestRiskScore) {
                    lowestRiskScore = score;
                    lowestPriority = learnerPerfs[i];
                    lowestIndex = i;
                }
            }
            if (newLearnerRiskScore <= lowestRiskScore) {
                return false;
            }
            for (int i=0; i<=rear; i++) {
                if (learnerPerfs[i].username == lowestPriority.username) {
                    if (i+1 == MAX_RISKY_LEARNER) {
                        learnerPerfs[i] = LearnerPerformance();
                        rear--;
                    } else {
                        shiftElementsUp(i+1);
                    }
                    break;
                }
            } 
            return true;
        }

        void shiftElementsUp(int startIndex) {
            if (startIndex <= 0 || startIndex >= MAX_RISKY_LEARNER) return;
            for (int i=startIndex; i<=rear; i++) {
                learnerPerfs[i-1] = learnerPerfs[i];
            }
            learnerPerfs[rear] = LearnerPerformance();
            rear --;
        }

    public:
        RiskyLearnerPriorityQueue() {
            rear = -1;
        }

        void enqueue(LearnerPerformance learnerPerf) {
            double riskScore = learnerPerf.calcRiskScore();
            if (isEmpty()) {
                learnerPerfs[0] = learnerPerf;
                rear = 0;
                return;
            }

            int curr;

            if (isFull()) {
                if (conditionallyRemoveLowestPriority(riskScore)) {
                    learnerPerfs[++rear] = learnerPerf;
                    curr = rear;
                } else {
                    return;
                }
            } else {
                learnerPerfs[++rear] = learnerPerf;
                curr = rear;
            }

            // Bubble up based on priority
            while (curr > 0) {
                double nextHigherRiskScore = learnerPerfs[curr-1].calcRiskScore();
                if (riskScore > nextHigherRiskScore) {
                    LearnerPerformance temp = learnerPerfs[curr];
                    learnerPerfs[curr] = learnerPerfs[curr-1];
                    learnerPerfs[curr-1] = temp;
                    curr--;
                } else {
                    break;
                }
            }
        }

        LearnerPerformance dequeue() {
            if (isEmpty()) {
                throw std::out_of_range("!! RiskyLearnerPriorityQueue Empty !!");
            }
            LearnerPerformance removed = learnerPerfs[0];
            shiftElementsUp(1);
            return removed;
        }

        void display() {
            if (isEmpty()) {
                cout << "--- At-Risk Learners Ranking Empty ---" << endl;
                return;
            }
            cout << "At-Risk Learners Ranking: ";
            for (int i=0; i<=rear; i++) {
                srand(time(0));
                cout << i+1 << ". " << learnerPerfs[i].username 
                << ": " << LearnerPerformance().getRandomRecommendation() << endl;
            }
        }

        bool isEmpty() {
            return rear == -1;
        }

        bool isFull() {
            return rear == MAX_RISKY_LEARNER - 1;
        }
};

#endif