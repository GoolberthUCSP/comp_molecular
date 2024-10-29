#include<iostream>
#include<string>
#include<vector>
#include<map>

using namespace std;

const int MAX = 1e9;
map<char,char> is_complement = {
  {'A','U'},
  {'U','A'},
  {'C','G'},
  {'G','C'}
};

int score(char a, char b){
  if(a == b){
    return 0;
  }
  else{
    if ((a == 'C' && b == 'G') || (a == 'G' && b == 'C'))
      return -5;
    else if ((a == 'A' && b == 'U') || (a == 'U' && b == 'A'))
      return -4;
    else if ((a == 'G' && b == 'U') || (a == 'U' && b == 'G'))
      return -1;
    else
      return 0;
  }
}

vector<vector<int>> nussinov(const string seq){
  vector<vector<int>> result(seq.size(), vector<int>(seq.size(), 0));
  
  for(int d = 0; d < seq.size(); d++){
    for(int i = 0; i + d < seq.size(); i++){
        int j = i + d;
        int min_ = MAX;
        for (int k = i + 1; k < j; k++)
        min_ = min(min_, result[i][k] + result[k + 1][j]);

        result[i][j] = min(
                      result[i+1][j],
                      min(
                      result[i][j-1],
                      min(
                      result[i+1][j-1] + score(seq[i], seq[j]),
                      min_
                      )
                      )
        );
    }
  }
  return result;
}

int main(){
  
}