#ifndef PAIRWISE_ALIGNMENT_H
#define PAIRWISE_ALIGNMENT_H

#include <iostream>
#include <utility>
#include <string>
#include <vector>
#include <fstream>
#include "threadpool.h"

using namespace std;

ThreadPool pool(thread::hardware_concurrency());
int k = 0, NSOL = 1000;

struct Solution{
    string s1;
    string s2;
    int score;
    Solution(string s1, string s2, int score) : s1(s1), s2(s2), score(score) {}
    Solution() {}
    bool operator<(const Solution &s) const { return score > s.score; }
    bool operator>(const Solution &s) const { return score < s.score; }
};

void fill_dp(int **dp, string &s1, string &s2, int m, int n){
    for(int i = 0; i < m+1; i++){
        for(int j = 0; j < n+1; j++){
            if (i == 0)
                dp[i][j] = j*(-2);
            else if (j == 0)
                dp[i][j] = i*(-2);
            else
                dp[i][j] = max(dp[i-1][j] - 2, max(dp[i][j-1] - 2, dp[i-1][j-1] + (s1[i-1] == s2[j-1] ? 1 : -1)));
        }
    }
}

// Needelman-Wunsch Algorithm
Solution pairwise_alignment(string s1, string s2){
    int m = s1.size();
    int n = s2.size();
    int **dp = new int*[m+1];
    string first = "", second = "";
    for(int i = 0; i < m+1; i++){
        dp[i] = new int[n+1];
    }

    fill_dp(dp, s1, s2, m, n);

    int i = m, j = n;
    while(i > 0 && j > 0){
        if (dp[i][j] == dp[i-1][j-1] + (s1[i-1] == s2[j-1] ? 1 : -1)){
            first += s1[i-1];
            second += s2[j-1];
            i--;
            j--;
        } else if (dp[i][j] == dp[i-1][j] - 2){
            first += s1[i-1];
            second += "-";
            i--;
        } else if (dp[i][j] == dp[i][j-1] - 2){
            first += "-";
            second += s2[j-1];
            j--;
        }
    }
    while(i > 0){
        first += s1[i-1];
        second += "-";
        i--;
    }
    while(j > 0){
        first += "-";
        second += s2[j-1];
        j--;
    }
    first = string(first.rbegin(), first.rend());
    second = string(second.rbegin(), second.rend());

    for(int i = 0; i < m+1; i++){
        delete[] dp[i];
    }
    delete[] dp;
    return Solution(first, second, dp[m][n]);
}

void recursive(int **dp, int i, int j, string &s1, string &s2, vector<Solution> &results, string first = "", string second = "") {
    if (k == NSOL) {
        return;
    }
    
    if (i == 0 && j == 0) {
        first = string(first.rbegin(), first.rend());
        second = string(second.rbegin(), second.rend());
        results.emplace_back(first, second, dp[i][j]);
        k++;
        return;
    } else if (i == 0) {
        recursive(dp, i, j-1, s1, s2, results, first + "-", second + s2[j-1]);
    } else if (j == 0) {
        recursive(dp, i-1, j, s1, s2, results, first + s1[i-1], second + "-");
    } else {
        if (dp[i][j] == dp[i-1][j-1] + (s1[i-1] == s2[j-1] ? 1 : -1))
            recursive(dp, i-1, j-1, s1, s2, results, first + s1[i-1], second + s2[j-1]);
        if (dp[i][j] == dp[i-1][j] - 2)
            recursive(dp, i-1, j, s1, s2, results, first + s1[i-1], second + "-");
        if (dp[i][j] == dp[i][j-1] - 2)
            recursive(dp, i, j-1, s1, s2, results, first + "-", second + s2[j-1]);
    }
}

vector<Solution> n_pairwise_alignment(string s1, string s2) {
    int m = s1.size();
    int n = s2.size();
    vector<Solution> results;
    int **dp = new int*[m+1];
    for(int i = 0; i < m+1; i++) {
        dp[i] = new int[n+1];
    }

    fill_dp(dp, s1, s2, m, n);
    recursive(dp, m, n, s1, s2, results);

    for (int i = 0; i < m+1; i++) {
        delete[] dp[i];
    }
    delete[] dp;
    return results;
}

#endif