#include<iostream>
#include<fstream>
#include<string>

using namespace std;

int main(){
    string filename = "output\\best_result.txt";
    ifstream file(filename);
    if (!file){
        cout << "Error opening file" << endl;
        return 1;
    }
    string line, s1, s2;
    getline(file, line);
    getline(file, s1);
    getline(file, s2);
    file.close();

    int size = s1.size();
    char **mtx = new (nothrow) char*[size + 1];
    if(!mtx){
        cout << "Error allocating memory" << endl;
        return 1;
    }
    for(int i = 0; i < size + 1; i++){
        mtx[i] = new (nothrow) char[size + 1];
        if(!mtx[i]){
            cout << "Error allocating memory" << endl;
            return 1;
        }
    }
    mtx[0][0] = ' ';
    for(int i = 0; i < size; i++){
        mtx[i + 1][0] = s1[i];
        mtx[0][i + 1] = s2[i];
    }
    
    for(int i = 1; i < size + 1; i++){
        for(int j = 1; j < size + 1; j++){
            mtx[i][j] = (s1[i - 1] == s2[j - 1] ? '*' : ' ');
        }
    }

    ofstream fout("output\\point_mtx.txt");
    for(int i = 0; i < size + 1; i++){
        line = string(mtx[i], size + 1);
        fout << line << endl;
    }
    fout.close();

    for(int i = 0; i < size + 1; i++){
        delete[] mtx[i];
    }
    delete[] mtx;
    return 0;
}