
#include <bits/stdc++.h>
using namespace std;

static const int DIRS = 4;
static const int dr[DIRS] = {-1,0,1,0};
static const int dc[DIRS] = {0,1,0,-1};
static const char directionChars[DIRS] = {'U','R','D','L'};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int N, Q;
    cin >> N >> Q;

    vector<int> qr(Q), qc(Q);
    vector<char> qt(Q);
    for (int i = 0; i < Q; i++) {
        cin >> qr[i] >> qc[i] >> qt[i];
        qr[i]--; qc[i]--;
    }

    vector<vector<char>> grid(N, vector<char>(N, '?'));
    for (int i = 0; i < Q; i++) {
        grid[qr[i]][qc[i]] = qt[i];
    }

    int totalCells = N * N;
    vector<bool> isFixed(totalCells, false);
    vector<int> fixedDir(totalCells, -1);
    vector<bool> reachable(totalCells, false);
    vector<int> incomingReachCount(totalCells, 0);

    auto idx = [&](int r, int c) {
        return r * N + c;
    };

    auto inBounds = [&](int r, int c) {
        return (r >= 0 && r < N && c >= 0 && c < N);
    };

    for (int r = 0; r < N; r++) {
        for (int c = 0; c < N; c++) {
            int cell = idx(r,c);
            if (grid[r][c] == '?') {
                isFixed[cell] = false;
            } else {
                isFixed[cell] = true;
                for (int d = 0; d < DIRS; d++) {
                    if (directionChars[d] == grid[r][c]) {
                        fixedDir[cell] = d;
                        break;
                    }
                }
            }
        }
    }

    deque<int>q_reach;
    int reachableCount = 0;

    auto markReachable = [&](int cell) {
        if (!reachable[cell]) {
            reachable[cell] = true;
            reachableCount++;
            q_reach.push_back(cell);
        }
    };

    for (int r = 0; r < N; r++) {
        for (int c = 0; c < N; c++) {
            int cell = idx(r,c);
            if (isFixed[cell]) {
                int d = fixedDir[cell];
                int nr = r + dr[d], nc = c + dc[d];
                if (!inBounds(nr,nc)) {
                    markReachable(cell);
                }
            } else {
                bool leadsOut = false;
                for (int d = 0; d < DIRS; d++) {
                    int nr = r + dr[d], nc = c + dc[d];
                    if (!inBounds(nr,nc)) {
                        leadsOut = true;
                        break;
                    }
                }
                if (leadsOut) {
                    markReachable(cell);
                }
            }
        }
    }

    auto processQueue = [&]() {
        while(!q_reach.empty()) {
            int cur = q_reach.front(); q_reach.pop_front();
            int cr = cur / N, cc = cur % N;

            for (int d = 0; d < DIRS; d++) {
                int nr = cr - dr[d], nc = cc - dc[d];
                if (!inBounds(nr,nc)) continue;
                int neigh = idx(nr,nc);
                if (reachable[neigh]) continue; // already reachable

                if (isFixed[neigh]) {
                    int fd = fixedDir[neigh];
                    int rr = nr + dr[fd], rc = nc + dc[fd];
                    if (rr == cr && rc == cc) {
                        markReachable(neigh);
                    }
                } else {
                    if (incomingReachCount[neigh] == 0) {
                        incomingReachCount[neigh] = 1;
                        markReachable(neigh);
                    } else {
                    }
                }
            }
        }
    };

    processQueue();

    vector<int> answers(Q);
    answers[Q-1] = totalCells - reachableCount;

    for (int i = Q-1; i > 0; i--) {
        int r = qr[i], c = qc[i];
        int cell = idx(r,c);
        if (isFixed[cell]) {
            isFixed[cell] = false;
            fixedDir[cell] = -1;
            incomingReachCount[cell] = 0;
            {
                bool canOut = false;
                for (int d = 0; d < DIRS; d++) {
                    int nr = r + dr[d], nc = c + dc[d];
                    if (!inBounds(nr,nc)) {
                        canOut = true;
                        break;
                    }
                }
                if (canOut) {
                    markReachable(cell);
                } else {
                    int countReachNeighbors = 0;
                    for (int d = 0; d < DIRS; d++) {
                        int nr = r + dr[d], nc = c + dc[d];
                        if (inBounds(nr,nc)) {
                            int ncell = idx(nr,nc);
                            if (reachable[ncell]) {
                                countReachNeighbors++;
                            }
                        }
                    }
                    if (countReachNeighbors > 0) {
                        incomingReachCount[cell] = countReachNeighbors;
                        markReachable(cell);
                    }
                }
            }
            processQueue();
        }

        answers[i-1] = totalCells - reachableCount;
    }

    for (int i = 0; i < Q; i++) {
        cout << answers[i] << "\n";
    }

    return 0;
}
