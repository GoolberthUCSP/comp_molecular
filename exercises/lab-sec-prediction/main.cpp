#include<iostream>
#include<string>
#include<vector>
#include<algorithm>
#include<map>

using namespace std;

const int MAX = 1e9;
map<char,char> complement = {
  {'A','U'},
  {'U','A'},
  {'C','G'},
  {'G','C'}
};

int score1(char a, char b){
  if ((a == 'C' && b == 'G') || (a == 'G' && b == 'C'))
    return -5;
  else if ((a == 'A' && b == 'U') || (a == 'U' && b == 'A'))
    return -4;
  else if ((a == 'G' && b == 'U') || (a == 'U' && b == 'G'))
    return -1;
  else
    return 0;
}

int score2(char a, char b){
  return a == complement[b] ? -1 : 0;
}

vector<vector<int>> nussinov(string seq){
  vector<vector<int>> result(seq.size(), vector<int>(seq.size(), 0));
  
  for(int d = 1; d < seq.size(); d++){
    for(int i = 0; i + d < seq.size(); i++){
        int j = i + d;
        int min_ = MAX;
        for (int k = i; k < j; k++)
          min_ = min(min_, result[i][k] + result[k + 1][j]);

        result[i][j] = min({
                      result[i+1][j], // Not align seq_i
                      result[i][j-1], // Not align seq_j
                      result[i+1][j-1] + score1(seq[i], seq[j]), // Align seq_i and seq_j
                      min_});
    }
  }
  return result;
}

void traceback(const vector<vector<int>>& result, const string& seq, int i, int j, string& structure) {
    if (i >= j) return;

    if (result[i][j] == result[i + 1][j]) {
        structure[i] = '.'; // Join i
        traceback(result, seq, i + 1, j, structure);
    } else if (result[i][j] == result[i][j - 1]) {
        structure[j] = '.'; // Join j
        traceback(result, seq, i, j - 1, structure);
    } else if (result[i][j] == result[i + 1][j - 1] + score1(seq[i], seq[j]) && complement[seq[i]] == seq[j]) {
        structure[i] = '('; // Join i and j
        structure[j] = ')';
        traceback(result, seq, i + 1, j - 1, structure);
    } else {
        for (int k = i; k < j; k++) {
            if (result[i][j] == result[i][k] + result[k + 1][j]) {
                traceback(result, seq, i, k, structure);
                traceback(result, seq, k + 1, j, structure);
                break;
            }
        }
    }
}

void print_structure(const string& seq, const vector<vector<int>>& result) {
    string structure(seq.size(), '.'); // Initialize with '.'
    traceback(result, seq, 0, seq.size() - 1, structure);
    cout << "Sequence : " << seq << endl;
    cout << "Structure: " << structure << endl;
}

int main(){
  string seq1 = "GGAAAUCC";
  string seq2 = "ACUCGAUUCCGAG";

  vector<vector<int>> result = nussinov(seq1);
  print_structure(seq1, result);
  cout << endl;
  result = nussinov(seq2);
  print_structure(seq2, result);
}