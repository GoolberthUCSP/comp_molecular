#include<iostream>
#include<utility>
#include<string>
#include<vector>
#include<fstream>
#include "threadpool.h"

using namespace std;

const int NTH = thread::hardware_concurrency();
ThreadPool pool(NTH);
int k = 0, NSOL;

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
    cout << "Global alignment score: " << dp[m][n] << endl;
}

// Needelman-Wunsch Algorithm
pair<string, string> pairwise_alignment(string s1, string s2){
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
    return {first, second};
}

void recursive(int **dp, int i, int j, string &s1, string &s2, vector<pair<string, string>> &results, string first = "", string second = "") {
    if (k == NSOL){
        return;
    }
    
    if (i == 0 && j == 0){
        first = string(first.rbegin(), first.rend());
        second = string(second.rbegin(), second.rend());
        results.push_back(make_pair(first, second));
        k++;
        return;
    }
    else if (i == 0){ // j > 0
        recursive(dp, i, j-1, s1, s2, results, first + "-", second + s2[j-1]);
    }
    else if (j == 0){ // i > 0
        recursive(dp, i-1, j, s1, s2, results, first + s1[i-1], second + "-");
    }
    else { // i > 0 && j > 0
        if (dp[i][j] == dp[i-1][j-1] + (s1[i-1] == s2[j-1] ? 1 : -1))
            recursive(dp, i-1, j-1, s1, s2, results, first + s1[i-1], second + s2[j-1]);
        if (dp[i][j] == dp[i-1][j] - 2)
            recursive(dp, i-1, j, s1, s2, results, first + s1[i-1], second + "-");
        if (dp[i][j] == dp[i][j-1] - 2)
            recursive(dp, i, j-1, s1, s2, results, first + "-", second + s2[j-1]);
    }
}

void task(int **dp, int m, int n, string &s1, string &s2, vector<pair<string, string>> &results, string first = "", string second = "") {
    if (m == 0 || n == 0){
        first = string(first.rbegin(), first.rend());
        second = string(second.rbegin(), second.rend());
        pool.enqueue([&]{ results.push_back(make_pair(first, second)); });
        return;
    }
    else if (m == 0){
        pool.enqueue([&]{ task(dp, m, n-1, ref(s1), ref(s2), ref(results), first + "-", second + s2[n-1]); });
    }
    else if (n == 0){
        pool.enqueue([&]{ task(dp, m-1, n, ref(s1), ref(s2), ref(results), first + s1[m-1], second + "-"); });
    }
    else {
        if (dp[m][n] == dp[m-1][n-1] + (s1[m-1] == s2[n-1] ? 1 : -1))
            pool.enqueue([&]{ task(dp, m-1, n-1, ref(s1), ref(s2), ref(results), first + s1[m-1], second + s2[n-1]); });
        if (dp[m][n] == dp[m-1][n] - 2)
            pool.enqueue([&]{ task(dp, m-1, n, ref(s1), ref(s2), ref(results), first + s1[m-1], second + "-"); });
        if (dp[m][n] == dp[m][n-1] - 2)
            pool.enqueue([&]{ task(dp, m, n-1, ref(s1), ref(s2), ref(results), first + "-", second + s2[n-1]); });
    }
}

vector<pair<string, string>> n_pairwise_alignment(string s1, string s2){
    int m = s1.size();
    int n = s2.size();
    vector<pair<string, string>> results;
    int **dp = new int*[m+1];
    for(int i = 0; i < m+1; i++){
        dp[i] = new int[n+1];
    }

    fill_dp(dp, s1, s2, m, n);
    // pool.enqueue([&]{ task(dp, m, n, s1, s2, results); });
    // pool.~ThreadPool();
    recursive(dp, m, n, s1, s2, results);
    for (int i = 0; i < m+1; i++){
        delete[] dp[i];
    }
    delete[] dp;
    return results;
}


int main(int argc, char *argv[]) {
    if (argc != 4) {
        cout << "Usage: " << argv[0] << " string1 string2 n_solutions" << endl;
        exit(1);
    }
    NSOL = atoi(argv[3]);
    if (NSOL < 0) {
        cout << "Usage: " << argv[0] << " string1 string2 n_solutions" << endl;
        exit(1);
    }
    
    ofstream fout("output\\result.txt");
    string s1 = argv[1];
    string s2 = argv[2];
    
    // Obtain n_solutions optimal results
    vector<pair<string, string>> results = n_pairwise_alignment(s1, s2);
    fout << results.size();
    for(int i = 0; i < results.size(); i++){
        fout << endl << results[i].first;
        fout << endl << results[i].second;
    }
    fout.close();
    return 0;
}