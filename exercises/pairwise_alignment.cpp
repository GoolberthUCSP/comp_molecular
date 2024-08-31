#include<iostream>
#include<utility>
#include<string>
#include<vector>

using namespace std;

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
pair<string, string> pairwise_alignment(string s1, string s2){
    int m = s1.size();
    int n = s2.size();
    pair<string, string> result;
    int **dp = new int*[m+1];
    for(int i = 0; i < m+1; i++){
        dp[i] = new int[n+1];
    }

    fill_dp(dp, s1, s2, m, n);

    int i = m, j = n;
    while(i > 0 && j > 0){
        if (dp[i][j] == dp[i-1][j-1] + (s1[i-1] == s2[j-1] ? 1 : -1)){
        result.first += s1[i-1];
        result.second += s2[j-1];
        i--;
        j--;
        } else if (dp[i][j] == dp[i-1][j] - 2){
        result.first += s1[i-1];
        result.second += "-";
        i--;
        } else if (dp[i][j] == dp[i][j-1] - 2){
        result.first += "-";
        result.second += s2[j-1];
        j--;
        }
    }
    while(i > 0){
        result.first += s1[i-1];
        result.second += "-";
        i--;
    }
    while(j > 0){
        result.first += "-";
        result.second += s2[j-1];
        j--;
    }
    result.first = string(result.first.rbegin(), result.first.rend());
    result.second = string(result.second.rbegin(), result.second.rend());
    return result;
}

void recursive(int **dp, int i, int j, string &s1, string &s2, vector<pair<string, string>> &results, pair<string, string> result = pair<string, string>("", "")){
    if (i == 0 && j == 0){
        result.first = string(result.first.rbegin(), result.first.rend());
        result.second = string(result.second.rbegin(), result.second.rend());
        results.push_back(result);
        return;
    }
    else if (i == 0){ // j > 0
        result.first += "-";
        result.second += s2[j-1];
        recursive(dp, i, j-1, s1, s2, results, result);
    }
    else if (j == 0){ // i > 0
        result.first += s1[i-1];
        result.second += "-";
        recursive(dp, i-1, j, s1, s2, results, result);
    }
    else { // i > 0 && j > 0
        if (dp[i][j] == dp[i-1][j-1] + (s1[i-1] == s2[j-1] ? 1 : -1)){
            result.first += s1[i-1];
            result.second += s2[j-1];
            recursive(dp, i-1, j-1, s1, s2, results, result);
            result.first.pop_back();
            result.second.pop_back();
        }
        if (dp[i][j] == dp[i-1][j] - 2){
            result.first += s1[i-1];
            result.second += "-";
            recursive(dp, i-1, j, s1, s2, results, result);
            result.first.pop_back();
            result.second.pop_back();
        }
        if (dp[i][j] == dp[i][j-1] - 2){
            result.first += "-";
            result.second += s2[j-1];
            recursive(dp, i, j-1, s1, s2, results, result);
            result.first.pop_back();
            result.second.pop_back();
        }
    }
}

vector<pair<string, string>> all_pairwise_alignment(string s1, string s2){
    int m = s1.size();
    int n = s2.size();
    vector<pair<string, string>> results;
    int **dp = new int*[m+1];
    for(int i = 0; i < m+1; i++){
        dp[i] = new int[n+1];
    }

    fill_dp(dp, s1, s2, m, n);

    cout << "Global alignment score: " << dp[m][n] << endl;
    recursive(dp, m, n, s1, s2, results);
    return results;
}



int main(){
    // Obtain the unique result
    pair<string, string> result = pairwise_alignment("AAAC", "AGC");
    cout << result.first << endl;
    cout << result.second << endl << endl;

    // Obtain all the optimal results
    vector<pair<string, string>> results = all_pairwise_alignment("AAAC", "AGC");
    for(int i = 0; i < results.size(); i++){
        cout << results[i].first << endl;
        cout << results[i].second << endl << endl;
    }
    return 0;
}