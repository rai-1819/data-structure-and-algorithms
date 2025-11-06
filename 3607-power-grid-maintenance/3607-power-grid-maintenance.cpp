#include <bits/stdc++.h>
using namespace std;

class DSU {
public:
    vector<int> parent, rankv, minOnline;
    DSU(int n) {
        parent.resize(n + 1);
        rankv.assign(n + 1, 0);
        minOnline.assign(n + 1, INT_MAX);
        iota(parent.begin(), parent.end(), 0);
    }

    int find(int x) {
        return parent[x] == x ? x : parent[x] = find(parent[x]);
    }

    void unite(int a, int b) {
        a = find(a);
        b = find(b);
        if (a == b) return;
        if (rankv[a] < rankv[b]) swap(a, b);
        parent[b] = a;
        minOnline[a] = min(minOnline[a], minOnline[b]);
        if (rankv[a] == rankv[b]) rankv[a]++;
    }
};

class Solution {
public:
    vector<int> processQueries(int c, vector<vector<int>>& connections, vector<vector<int>>& queries) {
        DSU dsu(c);
        for (auto &e : connections) {
            dsu.unite(e[0], e[1]);
        }

        int q = queries.size();
        vector<int> offCount(c + 1, 0);
        for (auto &qr : queries) if (qr[0] == 2) offCount[qr[1]]++;

        vector<int> isOnline(c + 1, 0);
        for (int i = 1; i <= c; i++) {
            if (offCount[i] == 0) {
                isOnline[i] = 1;
                int r = dsu.find(i);
                dsu.minOnline[r] = min(dsu.minOnline[r], i);
            }
        }

        vector<int> answers;
        answers.reserve(q);

        for (int i = q - 1; i >= 0; i--) {
            int type = queries[i][0];
            int x = queries[i][1];

            if (type == 2) {
                // forward: x goes offline
                // reverse: undo that offline
                offCount[x]--;
                if (offCount[x] == 0) { // only now it actually becomes online
                    isOnline[x] = 1;
                    int r = dsu.find(x);
                    dsu.minOnline[r] = min(dsu.minOnline[r], x);
                }
            } else {
                // query [1, x]
                if (isOnline[x]) {
                    answers.push_back(x);
                } else {
                    int r = dsu.find(x);
                    int mn = dsu.minOnline[r];
                    answers.push_back(mn == INT_MAX ? -1 : mn);
                }
            }
        }

        reverse(answers.begin(), answers.end());
        return answers;
    }
}
