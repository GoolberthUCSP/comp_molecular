#include<iostream>
#include<utility>
#include<string>

using namespace std;

// Needelman-Wunsch Algorithm
pair<string, string> pairwise_alignement(string s1, string s2){
  int m = s1.size();
  int n = s2.size();
  pair<string, string> result;
  int **dp = new int*[m+1];
  for(int i = 0; i < m+1; i++){
    dp[i] = new int[n+1];
  }

  for(int i = 0; i < m+1; i++){
    for(int j = 0; j < n+1; j++){
      if (i == 0) dp[i][j] = j*(-2);
      else if (j == 0) dp[i][j] = i*(-2);
      else{
        dp[i][j] = max(dp[i-1][j] - 2, 
                   max(dp[i][j-1] - 2, 
                   dp[i-1][j-1] + (s1[i-1] == s2[j-1] ? 1 : -1)));
      }
    }
  }

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

int main(){
  pair<string, string> result = pairwise_alignement("AAAC", "AGC");
  cout << result.first << endl;
  cout << result.second << endl;
  return 0;
}