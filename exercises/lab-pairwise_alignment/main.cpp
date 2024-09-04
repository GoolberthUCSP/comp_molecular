#include<iostream>
#include<string>
#include<fstream>
#include<algorithm>
#include<vector>
#include<set>
#include<windows.h>

using namespace std;

int main(int argc, char *argv[]){
    if (argc != 3){
        cout << "Usage: " << argv[0] << " index index" << endl;
        cout << "index: [0-2] for Bacteria, Sars-Cov, Influenza" << endl;
        return 1;
    }

    int index1 = stoi(argv[1]), index2 = stoi(argv[2]);
    if (index1 < 0 || index1 > 2 || index2 < 0 || index2 > 2){
        cout << "index: [0-2] for Bacteria, Sars-Cov, Influenza" << endl;
        return 1;
    }

    vector<string> species = {"Bacteria", "Sars-Cov", "Influenza"};
    vector<string> sequences(3);
    string filename = "sequences.txt";
    ifstream file(filename);
    string line;
    int idx = 0;
    while(getline(file, line)){
        if (find(species.begin(), species.end(), line) != species.end()){
            string sequence = "";
            for (int i = 0; i < 18; i++){
                getline(file, line);
                line = string(line.begin() + 4, line.end());
                line.erase(remove(line.begin(), line.end(), ' '), line.end());
                sequence += line;
            }
            sequences[idx] = sequence;
            idx++;
        }
    }
    file.close();
    cout << "Comparison between " << species[index1] << " and " << species[index2] << endl;
    string cmd = "g++ -o pairwise_alignment.exe pairwise_alignment.cpp";
    cmd += " && pairwise_alignment.exe ";
    cmd += sequences[index1];
    cmd += " ";
    cmd += sequences[index2];
    cmd += " 1000 && del pairwise_alignment.exe";
    system(cmd.c_str());

    return 0;
}