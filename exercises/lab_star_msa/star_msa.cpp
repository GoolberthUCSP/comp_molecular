#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include "../lib/threadpool.h"
#include "../lib/pairwise_alignment.h"

using namespace std;

const int POOL_SIZE = thread::hardware_concurrency();
const string out_file = "data\\star_msa.txt";

void star_msa(vector<string> &strings);
string fill(string s, int n, char c = '-');

int main(int argc, char *argv[]){
    if (argc < 4){
        cout << "Usage: " << argv[0] << " <n_strings> <string1> <string2> ... <stringn>" << endl;
        exit(1);
    }

    int n_str = stoi(argv[1]);
    vector<string> strings(n_str);
    for (int i = 0; i < n_str; i++)
        strings[i] = argv[i + 2];
    star_msa(strings);

    return 0;
}

string fill(string s, int n, char c){
    string result = s;
    for (int i = s.size(); i < n; i++)
        result += c;
    return result;
}

void star_msa(vector<string> &strings){
    int size = strings.size();
    vector<int> scores(size, 0);
    vector<vector<Solution>> solutions(size, vector<Solution>(size));
    for (int i = 0; i < size; i++){
        for (int j = i + 1; j < size; j++){
            solutions[i][j] = pairwise_alignment(strings[i], strings[j]);
            scores[i] += solutions[i][j].score;
            scores[j] += solutions[i][j].score;
        }
    }

    int best = 0;
    for (int i = 1; i < size; i++){
        if (scores[best] < scores[i])
            best = i;
    }

    vector<string> result(size);
    for (int i = 0; i < size; i++){
        if (i == best) result[i] = strings[i];
        else result[i] = pairwise_alignment(strings[best], strings[i]).s2;
    }
    int max_len = result[0].size();
    for (int i = 1; i < size; i++){
        if (result[i].size() > max_len)
            max_len = result[i].size();
    }
    for (int i = 0; i < size; i++){
        result[i] = fill(result[i], max_len);
    }


    ofstream fout(out_file);
    for (int i = 0; i < size; i++){
        fout << result[i] << endl;
    }
    fout.close();
}