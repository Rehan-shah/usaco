#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int N; cin >> N;
    vector<int> a(N);
    for (int i = 0; i < N; i++) cin >> a[i];

    // occurrences[label] will store positions (1-based) of 'label'
    vector<vector<int>> occurrences(N+1);
    for (int i = 0; i < N; i++) {
        occurrences[a[i]].push_back(i+1);
    }

    // For large N, this will be slow, but correct.
    // We'll just implement the correct logic.
    // For each x, we calculate the minimal groups by summing over all labels.

    for (int x = 1; x <= N; x++) {
        int total_groups = 0;
        for (int label = 1; label <= N; label++) {
            const auto &pos = occurrences[label];
            if (pos.empty()) continue;

            // Greedily form groups for this label
            int groups = 1;
            int start = pos[0];
            for (int i = 1; i < (int)pos.size(); i++) {
                // Check if we can add pos[i] to the current group
                if (pos[i] - start > x) {
                    // Need to start a new group
                    groups++;
                    start = pos[i];
                }
            }
            total_groups += groups;
        }
        cout << total_groups << "\n";
    }

    return 0;
}
