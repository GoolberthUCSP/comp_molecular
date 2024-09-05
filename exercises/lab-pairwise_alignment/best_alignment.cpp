#include<iostream>
#include<string>
#include<vector>
#include<algorithm>
#include<fstream>
#include<utility>

using namespace std;

ofstream fout("data\\best_result.txt");
int get_n_cuts(pair<string, string> &pair_);
pair<string, string> get_best_alignment(vector<pair<string, string>> &pairs);

int main(int argc, char *argv[]){
    string filename = "data\\result.txt";
    vector<pair<string, string>> results;
    ifstream file(filename);
    string line, s1, s2;
    getline(file, line);
    int n_pairs = stoi(line);

    for(int i = 0; i < n_pairs; i++){
        getline(file, s1);
        getline(file, s2);
        results.push_back({s1, s2});
    }
    file.close();

    pair<string, string> best = get_best_alignment(results);
    fout << endl << best.first;
    fout << endl << best.second;

    fout.close();
    return 0;
}

int get_n_cuts(pair<string, string> &pair_){
    int i = 0, j = 0, n = 0;
    int size = pair_.first.size();
    while (i < size){
        if (pair_.first[i] == '-'){
            while (i < size && pair_.first[i++] == '-');
            n++;
        }    
        i++;
    }
    while (j < size){
        if (pair_.second[j] == '-'){
            while (j < size && pair_.second[j++] == '-');
            n++;
        }
        j++;
    }
    if (pair_.first[size-1] == '-' || pair_.second[size-1] == '-') n--;
    if (pair_.first[0] == '-' || pair_.second[0] == '-') n--;
    return n;
}

pair<string, string> get_best_alignment(vector<pair<string, string>> &pairs){
    int best = get_n_cuts(pairs[0]), best_idx = 0, n;
    int size = pairs.size();
    for (int i = 1; i < size; i++){
        n = get_n_cuts(pairs[i]);
        if (n < best){
            best = n;
            best_idx = i;
        }
    }
    fout << "Lowest number of cuts: " << best;
    return pairs[best_idx];
}