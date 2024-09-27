class Solution:
    def __init__(self, first, second, score):
        self.first = first
        self.second = second
        self.score = score

def fill_dp(dp, s1, s2, m, n):
    for i in range(m + 1):
        dp[i][0] = -2 * i
    for j in range(n + 1):
        dp[0][j] = -2 * j

    for i in range(1, m + 1):
        for j in range(1, n + 1):
            match = dp[i - 1][j - 1] + (1 if s1[i - 1] == s2[j - 1] else -1)
            delete = dp[i - 1][j] - 2
            insert = dp[i][j - 1] - 2
            dp[i][j] = max(match, delete, insert)

def pairwise_alignment(s1, s2):
    m = len(s1)
    n = len(s2)
    dp = [[0] * (n + 1) for _ in range(m + 1)]
    first = ""
    second = ""

    fill_dp(dp, s1, s2, m, n)

    i, j = m, n
    while i > 0 and j > 0:
        if dp[i][j] == dp[i - 1][j - 1] + (1 if s1[i - 1] == s2[j - 1] else -1):
            first += s1[i - 1]
            second += s2[j - 1]
            i -= 1
            j -= 1
        elif dp[i][j] == dp[i - 1][j] - 2:
            first += s1[i - 1]
            second += "-"
            i -= 1
        elif dp[i][j] == dp[i][j - 1] - 2:
            first += "-"
            second += s2[j - 1]
            j -= 1

    while i > 0:
        first += s1[i - 1]
        second += "-"
        i -= 1
    while j > 0:
        first += "-"
        second += s2[j - 1]
        j -= 1

    first = first[::-1]
    second = second[::-1]

    return Solution(first, second, dp[m][n])