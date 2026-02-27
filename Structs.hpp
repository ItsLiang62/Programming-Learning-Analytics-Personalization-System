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

    bool isPass(int score) {
        return score >= 50;
    } 
};

struct Attempt {
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

    string toCsv() const {
        return username + "," +
        activity.difficulty + "," +
        to_string(score) + "," +
        (isPass ? "true" : "false");
    }
};

struct LearnerPerformance {
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

    void calcPerf() {

        vector<string> isPass = getRecentPerf(csvPos["isPass"]);

        recentFails = 0;

        for (int i=isPass.size()-1; i>=0; i--) {
            if (isPass[i] == "false") recentFails++;
        }

        vector<string> strScores = getRecentPerf(csvPos["score"]);
        vector<double> scores;
        for (const string& score: strScores) {
            scores.push_back(stod(score));
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

        int recentCount = min((int)perf.size(), 10);
        vector<string> recentPerf(perf.end() - recentCount, perf.end());

        srcFile.close();

        return recentPerf;
    }

    double calcRiskScore() {
        double weightedAvgScore = avgRecentDiffWeight * avgRecentScores;
        double lostWeightedAvgScore = 100 - weightedAvgScore;

        double recentFailPercent = 
        (double) recentFails / getRecentPerf(csvPos["isPass"]).size() * 100;

        return (lostWeightedAvgScore + recentFailPercent) / 2;
    }
};

extern Activity activities[5];

#endif