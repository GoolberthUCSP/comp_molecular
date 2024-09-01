#include<iostream>
#include<string>
#include<vector>
#include<algorithm>
#include<fstream>
#include<utility>

using namespace std;


int main(int argc, char *argv[]){
    string filename = "result.txt";
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

    // TODO: Fill in the rest of the function
}