#include <iostream>
#include <fstream>
#include "../lib/threadpool.h"

using namespace std;

const int POOL_SIZE = thread::hardware_concurrency();
const string out_file = "data\\star_msa.txt";

void star_msa(vector<string> &strings);

int main(int argc, char *argv[]){
    if (argc < 4){
        cout << "Usage: " << argv[0] << " <n_strings> <string1> <string2> ... <stringn>" << endl;
        exit(1);
    }

}

void star_msa(vector<string> &strings){

}