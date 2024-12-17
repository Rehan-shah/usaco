#include <iostream>
#include <vector>
using namespace std;

const int MOD = 1000000007;

void solve() {
    int N;
    cin >> N;
    string s;
    cin >> s;

    // Initialize L and M arrays
    vector<int> L(N + 1, 0);
    vector<int> M(N + 1, 0);

    // Compute L[i]: max x where [i, i+x-1] are 'R' or 'X'
    for (int i = N - 1; i >= 0; --i) {
        if (s[i] == 'R' || s[i] == 'X') {
            L[i] = 1 + L[i + 1];
        } else {
            L[i] = 0;
        }
    }

    // Compute M[i]: max x where [i, i+x-1] are 'B' or 'X'
    for (int i = N - 1; i >= 0; --i) {
        if (s[i] == 'B' || s[i] == 'X') {
            M[i] = 1 + M[i + 1];
        } else {
            M[i] = 0;
        }
    }

    // Precompute all possible operations starting at each position
    vector<vector<int>> operations(N + 1);
    for (int x = 1; x <= N / 2; ++x) {
        for (int i = 0; i + x <= N; ++i) {
            if (L[i] >= x && M[i + x] >= x) {
                operations[i].push_back(x);
            }
        }
    }

    // Initialize DP array
    vector<int> dp(N + 1, 0);
    dp[0] = 1;

    for (int i = 0; i < N; ++i) {
        // If current point can be left white (coloring 'X')
        if (s[i] == 'X') {
            dp[i + 1] = (dp[i + 1] + dp[i]) % MOD;
        }

        // Apply all operations starting at i + 1
        for (int x : operations[i]) {
            int end = i + 2 * x;
            if (end <= N) {
                dp[end] = (dp[end] + dp[i]) % MOD;
            }
        }
    }

    cout << dp[N] << endl;
}

int main() {
    solve();
    return 0;
}
