#include <iostream>
#include <list>
#include <utility>

using namespace std;

pair<string, string> pairwise_alignment(string a, string b) {
    int n = a.size();
    int m = b.size();
    int **dp = new int *[n + 1];
    for (int i = 0; i < n + 1; i++) {
        dp[i] = new int[m + 1]{0};
    }

    for (int i = 1; i < n + 1; i++) {
        for (int j = 1; j < m + 1; j++) {
            if (a[i - 1] == b[j - 1]) {
                dp[i][j] = dp[i - 1][j - 1] + 1;
            } else {
                dp[i][j] = max(dp[i - 1][j], dp[i][j - 1]);
            }
        }
    }
    
    int i = n;
    int j = m;
    string x = "";
    string y = "";
    while (i > 0 && j > 0) {
        if (a[i - 1] == b[j - 1]) {
            x = a[i - 1] + x;
            y = b[j - 1] + y;
            i--;
            j--;
        } else if (dp[i - 1][j] > dp[i][j - 1]) {
            x = a[i - 1] + x;
            y = "-" + y;
            i--;
        } else {
            x = "-" + x;
            y = b[j - 1] + y;
            j--;
        }
    }
    while (i > 0) {
        x = a[i - 1] + x;
        y = "-" + y;
        i--;
    }
    while (j > 0) {
        x = "-" + x;
        y = b[j - 1] + y;
        j--;
    }
    return make_pair(x, y);
}

int main() {
    string a= "AGGTAB";
    string b= "GXTXAYB";
    pair<string, string> result = pairwise_alignment(a, b);
    cout << result.first << endl;
    cout << result.second << endl;
    return 0;
}