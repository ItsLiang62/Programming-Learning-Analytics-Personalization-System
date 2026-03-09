#ifndef STRUCTS_HPP
#define STRUCTS_HPP

#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>
#include <unordered_map>
#include <algorithm>
#include <numeric>

using namespace std;

struct Activity {
    // TP074952 ADRIAN LIEW REN QIAN
    
    string difficulty;
    vector<string> questions;
    string correctAns;

    Activity(
        const string& difficulty, 
        const vector<string>& questions, 
        const string& correctAns
    ) : 
    difficulty(difficulty), 
    questions(questions), 
    correctAns(correctAns) { }

    double calcScore(const string& userAns) {
        int numCorrect = 0;

        for (int i=0; i<userAns.length(); i++) {
            if (userAns[i] == correctAns[i]) numCorrect++;
        }

        return (double) numCorrect / questions.size() * 100;  
    }

    bool isPass(double score) {
        return score >= 50;
    } 
};

struct Attempt {
    // TP077245 GAVIN YONG DEE XIN
    string username;
    Activity activity;
    string userAns;
    double score;
    bool isPass;

    Attempt(
        const string& username, 
        Activity activity, 
        const string& userAns
    ) :
    username(username),
    activity(activity),
    userAns(userAns),
    score(activity.calcScore(userAns)),
    isPass(activity.isPass(score)) {}

    Attempt() :
    username(""),
    activity(Activity("", {}, "")),
    userAns(""),
    score(0),
    isPass(false) {}

    string toCsv() const {
        return username + "," +
        activity.difficulty + "," +
        to_string(score) + "," +
        (isPass ? "true" : "false");
    }
};

struct LearnerPerformance {
    // TP076334 WANG LIANG XUAN

    string username;
    unordered_map<string, int>csvPos;
    int recentFails;
    double avgRecentScores;
    double avgRecentDiffWeight;
    string srcPath = "attempt.csv";
    
    
    LearnerPerformance(const string& username) :
    username(username) {
        csvPos["username"] = 0;
        csvPos["difficulty"] = 1;
        csvPos["score"] = 2;
        csvPos["isPass"] = 3;
        calcPerf();
    }

    LearnerPerformance() {
        csvPos["username"] = 0;
        csvPos["difficulty"] = 1;
        csvPos["score"] = 2;
        csvPos["isPass"] = 3;
    }

    void calcPerf() {

        vector<string> isPass = getRecentPerf(csvPos["isPass"]);

        recentFails = 0;

        for (int i=isPass.size()-1; i>=0; i--) {
            if (isPass[i] == "false") recentFails++;
        }

        vector<string> strScores = getRecentPerf(csvPos["score"]);
        vector<double> scores;
        for (const string& scoreStr : strScores) {
            try {
                if (!scoreStr.empty()) {
                    // stod can fail if there are hidden non-numeric chars
                    scores.push_back(stod(scoreStr));
                }
            } catch (const std::invalid_argument& e) {
                // This will tell you exactly what string caused the crash
                cerr << "Error: Invalid number found in CSV: [" << scoreStr << "]" << endl;
                continue; 
            }
        }

        if (!scores.empty()) {
            avgRecentScores = accumulate(scores.begin(), scores.end(), 0.0) / scores.size();
        } else {
            avgRecentScores = 0.0;
        }
        

        vector<string> difficulties = getRecentPerf(csvPos["difficulty"]);
        avgRecentDiffWeight = 0;

        for (int i=difficulties.size()-1; i>=0; i--) {
            if (difficulties[i] == "advanced") {
                avgRecentDiffWeight += 1;
            } else if (difficulties[i] == "intermediate") {
                avgRecentDiffWeight += 0.8;
            } else if (difficulties[i] == "beginner") {
                avgRecentDiffWeight += 0.6;
            }
        }

        if (!difficulties.empty()) {
            avgRecentDiffWeight /= difficulties.size();
        } else {
            avgRecentDiffWeight = 0.0;
        }
    }

    vector<string> getRecentPerf(int perfCsvPos) {
        ifstream srcFile(srcPath);
        if (!srcFile.is_open()) {
            throw runtime_error(srcPath + " could not be opened");
        }

        string csvLine;
        std::vector<string> perf; 

        while (getline(srcFile, csvLine)) {
            stringstream ss(csvLine);
            string value;
            vector<string> values;

            while (getline(ss, value, ',')) {
                values.push_back(value);
            }

            if (values.size() > perfCsvPos &&
            values[csvPos["username"]] == username) {
                perf.push_back(values[perfCsvPos]);
            }
        }

        if (perf.empty()) return {};

        int recentCount = min((int)perf.size(), 10);
        vector<string> recentPerf(perf.end() - recentCount, perf.end());

        srcFile.close();

        return recentPerf;
    }

    double calcRiskScore() {
        int attemptCount = getRecentPerf(csvPos["isPass"]).size();

        if (attemptCount == 0) {
            return 0;
        }

        double weightedAvgScore = avgRecentDiffWeight * avgRecentScores;
        double lostWeightedAvgScore = 100 - weightedAvgScore;

        double recentFailPercent = 
        (double) recentFails / getRecentPerf(csvPos["isPass"]).size() * 100;
        

        return (lostWeightedAvgScore + recentFailPercent) / 2;
    }

    string getRandomRecommendation() {
        vector<string> recommendations = {
            "Review basic syntax and practice with simple coding exercises",
            "Focus on understanding variables, data types, and control structures",
            "Complete additional beginner-level coding challenges daily",
            "Watch tutorial videos on fundamental programming concepts",
            "Practice with online coding platforms like HackerRank or Codecademy",
            "Work on small projects to apply object-oriented programming concepts",
            "Study data structures and algorithms with practical implementations",
            "Review class designs and practice inheritance and polymorphism",
            "Debug existing code to improve problem-solving skills",
            "Participate in peer programming sessions for collaborative learning",
            "Analyze complex algorithms and optimize time/space complexity",
            "Study design patterns and refactor existing code.",
            "Contribute to open-source projects for real-world experience",
            "Focus on system design and architecture principles.",
            "Practice with advanced topics like multithreading and memory management",
            "Schedule one-on-one consultation with the instructor",
            "Form a study group with peers for regular practice sessions",
            "Review previous failed attempts and understand the mistakes",
            "Take a step back and rebuild fundamentals before advancing",
            "Use spaced repetition techniques to reinforce learning",
            "Set small, achievable goals and track daily progress",
            "Take breaks when stuck - fresh perspective often helps",
            "Remember that struggling is part of the learning process",
            "Celebrate small victories to maintain motivation",
            "Practice regularly - consistency beats intensity"
        };
        
        int randomIndex = rand() % recommendations.size();
        return recommendations[randomIndex];
    }
};

extern Activity activities[5];

#endif