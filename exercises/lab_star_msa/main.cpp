#include <iostream>
#include <fstream>
#include <vector>
#include <windows.h>

using namespace std;

const string filename = "sequences.txt";

int main(){
    ifstream fin(filename);
    if (!fin){
        cout << "File not found" << endl;
        return 1;
    }
    string line;
    getline(fin, line);
    int n = stoi(line);
    vector<string> sequences(n);
    for (int i = 0; i < n; i++){
        getline(fin, sequences[i]);
    }
    fin.close();

    string cmd = "g++ -o star_msa.exe star_msa.cpp";
    cmd += " && star_msa.exe " + to_string(n) + " ";
    for (int i = 0; i < n; i++){
        cmd += sequences[i] + " ";
    }
    system(cmd.c_str());

    return 0;
}