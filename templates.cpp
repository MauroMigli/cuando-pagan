struct LazySegTree {
    struct Node {
        long long sum;
    };

    const Node neutral = {0};

    Node merge(Node a, Node b){
        return {a.sum + b.sum};
    }

    int n;
    vector<Node> tree;
    vector<long long> lazy; // pending add, not yet passed to the children

    LazySegTree(vector<long long> &A){
        n = A.size();
        tree.assign(4 * n, neutral);
        lazy.assign(4 * n, 0);
        build(A, 1, 0, n - 1);
    }

    void build(vector<long long> &A, int k, int l, int r){
        if (l == r){
            tree[k] = {A[l]};
            return;
        }
        int m = (l + r) / 2;
        build(A, 2 * k, l, m);
        build(A, 2 * k + 1, m + 1, r);
        tree[k] = merge(tree[2 * k], tree[2 * k + 1]);
    }

    // add val to every position of node k (len positions) and remember it for the children
    void apply(int k, int len, long long val){
        tree[k].sum += val * len;
        lazy[k] += val;
    }

    void push(int k, int l, int m, int r){
        if (lazy[k] == 0) return;
        apply(2 * k, m - l + 1, lazy[k]);
        apply(2 * k + 1, r - m, lazy[k]);
        lazy[k] = 0;
    }

    void update(int ql, int qr, long long val, int k, int l, int r){
        if (qr < l || r < ql) return;
        if (ql <= l && r <= qr){
            apply(k, r - l + 1, val);
            return;
        }
        int m = (l + r) / 2;
        push(k, l, m, r);
        update(ql, qr, val, 2 * k, l, m);
        update(ql, qr, val, 2 * k + 1, m + 1, r);
        tree[k] = merge(tree[2 * k], tree[2 * k + 1]);
    }

    Node query(int ql, int qr, int k, int l, int r){
        if (qr < l || r < ql) return neutral;
        if (ql <= l && r <= qr) return tree[k];
        int m = (l + r) / 2;
        push(k, l, m, r);
        return merge(query(ql, qr, 2 * k, l, m), query(ql, qr, 2 * k + 1, m + 1, r));
    }

    void update(int l, int r, long long val){ update(l, r, val, 1, 0, n - 1); } // add val on [l, r]
    Node query(int l, int r){ return query(l, r, 1, 0, n - 1); } // inclusive [l, r], 0-indexed
};

struct SegTree {
    struct Node {
        long long sum;
    };

    const Node neutral = {0};

    Node merge(Node a, Node b){
        return {a.sum + b.sum};
    }

    int n;
    vector<Node> tree;

    SegTree(vector<long long> &A){
        n = A.size();
        tree.assign(4 * n, neutral);
        build(A, 1, 0, n - 1);
    }

    void build(vector<long long> &A, int k, int l, int r){
        if (l == r){
            tree[k] = {A[l]};
            return;
        }
        int m = (l + r) / 2;
        build(A, 2 * k, l, m);
        build(A, 2 * k + 1, m + 1, r);
        tree[k] = merge(tree[2 * k], tree[2 * k + 1]);
    }

    void update(int pos, long long val, int k, int l, int r){
        if (l == r){
            tree[k] = {val};
            return;
        }
        int m = (l + r) / 2;
        if (pos <= m) update(pos, val, 2 * k, l, m);
        else update(pos, val, 2 * k + 1, m + 1, r);
        tree[k] = merge(tree[2 * k], tree[2 * k + 1]);
    }

    Node query(int ql, int qr, int k, int l, int r){
        if (qr < l || r < ql) return neutral;
        if (ql <= l && r <= qr) return tree[k];
        int m = (l + r) / 2;
        return merge(query(ql, qr, 2 * k, l, m), query(ql, qr, 2 * k + 1, m + 1, r));
    }

    void update(int pos, long long val){ update(pos, val, 1, 0, n - 1); }
    Node query(int l, int r){ return query(l, r, 1, 0, n - 1); } // inclusive [l, r], 0-indexed
};

struct DSU {
    long long n;
    vector<long long> size;
    vector<long long> dad;
    DSU(long long node_amount){
        n = node_amount;
        size.assign(n, 1);
        dad.resize(n);
        iota(dad.begin(), dad.end(), 0);
    }

    long long find(long long u){
        if (dad[u] == u) return u;
        return dad[u] = find(dad[u]);
    }

    void unite(long long u, long long v){
        u = find(u);
        v = find(v);

        if (u == v) return;
        if (size[u] < size[v]) swap(u, v);
        dad[v] = u;
        size[u] += size[v];
    }
};

int lca(int u, int v){
    if (depth[u] < depth[v]) swap(u,v);
    for (int j = l; j >= 0; --j) {
        if (depth[up[u][j]] >= depth[v]) {
            u = up[u][j];
        }
    }

    if(u == v) return u;

    for (int i = l; i >= 0; --i) {
        if (up[u][i] != up[v][i]) {
            u = up[u][i];
            v = up[v][i];
        }
    }
    return up[u][0];
}

inv[1] = 1;
for (int i = 2; i < MAXN; i++) inv[i] = (MOD - MOD / i) * inv[MOD % i] % MOD;

struct Dinic {
    struct Edge { int to; long long cap; };
    int n;
    vector<Edge> e;
    vector<vector<int>> g;
    vector<int> level, it;

    Dinic(int n) : n(n), g(n), level(n), it(n) {}

    void reset(){
        e.clear();
        for (int i = 0; i < n; i++){
            g[i].clear();
            level[i] = it[i] = 0;
        }
    }

    // Returns the id of the forward edge; its reverse is id ^ 1.
    int addEdge(int u, int v, long long c) {
        g[u].push_back(e.size()); e.push_back({v, c});
        g[v].push_back(e.size()); e.push_back({u, 0});
        return (int)e.size() - 2;
    }

    // Reverse edge started at 0 and gained exactly what was pushed forward.
    long long flowOn(int id) const { return e[id ^ 1].cap; }

    bool bfs(int s, int t) {
        fill(level.begin(), level.end(), -1);
        queue<int> q; q.push(s); level[s] = 0;
        while (!q.empty()) {
            int u = q.front(); q.pop();
            for (int id : g[u]) if (e[id].cap > 0 && level[e[id].to] < 0) {
                level[e[id].to] = level[u] + 1; q.push(e[id].to);
            }
        }
        return level[t] >= 0;
    }

    long long dfs(int u, int t, long long f) {
        if (u == t) return f;
        // it[u] persists across calls in a phase so dead edges are never rescanned.
        for (int &i = it[u]; i < (int)g[u].size(); i++) {
            int id = g[u][i], v = e[id].to;
            if (e[id].cap > 0 && level[v] == level[u] + 1) {
                long long got = dfs(v, t, min(f, e[id].cap));
                if (got > 0) { e[id].cap -= got; e[id ^ 1].cap += got; return got; }
            }
        }
        return 0;
    }

    long long maxflow(int s, int t) {
        long long total = 0;
        while (bfs(s, t)) {
            fill(it.begin(), it.end(), 0);
            while (long long f = dfs(s, t, LLONG_MAX)) total += f;
        }
        return total;
    }
};

#include <bits/stdc++.h>
#include <ext/pb_ds/assoc_container.hpp>
#include <ext/pb_ds/tree_policy.hpp>
using namespace std;
using namespace __gnu_pbds;

template <class T>
using ordered_set = tree<T, null_type, less<T>, rb_tree_tag, tree_order_statistics_node_update>;

// S.find_by_order(k)  -> iterator to the k-th smallest (0-indexed)
// S.order_of_key(x)   -> number of elements strictly less than x
// insert / erase / lower_bound / upper_bound / prev / next work as in std::set


const int MAXN = 200000;            // size must be a compile-time constant
bitset<MAXN> A, B;

A.set(i); A.reset(i); A.flip(i);    // single bit: on, off, toggle
A.set(); A.reset();                 // whole bitset: all on, all off
A.test(i); A[i];                    // read bit i (test also checks the bound)

A & B; A | B; A ^ B; ~A;            // whole-set ops, O(MAXN / 64)
A &= B; A |= B; A ^= B;
A << k; A >> k;                     // shift the whole set by k positions

A.count();                          // number of set bits
A.any(); A.none(); A.all();

// iterate set bits only (GCC extension; returns MAXN when none is left)
for (int i = A._Find_first(); i < MAXN; i = A._Find_next(i)) { }

vector<bitset<MAXN>> ROWS(n);       // one bitset per node / color / row
bitset<8> S("1011");                // from a string; S.to_string(), S.to_ullong()

struct Fenwick {
    int n;
    vector<long long> BIT;
    Fenwick(int n) : n(n), BIT(n + 1, 0) {}
    void add(int i, long long v){ for (i++; i <= n; i += i & -i) BIT[i] += v; }
    long long pref(int i){ long long s = 0; for (i++; i > 0; i -= i & -i) s += BIT[i]; return s; } // sum of [0, i]
    long long query(int l, int r){ return pref(r) - pref(l - 1); }
};

const int MOD = 1e9 + 7;
const int MAXM = 100 + 1;

long long T[MAXM][MAXM]{}, TMP[MAXM][MAXM]{};
long long V[MAXM]{}, VTMP[MAXM]{};

void MMult(long long A[][MAXM], long long B[][MAXM], long long C[][MAXM]){
    for (int i = 0; i < M; i++){
        for (int j = 0; j < M; j++) C[i][j] = 0;
        for (int k = 0; k < M; k++){
            long long a = A[i][k];
            for (int j = 0; j < M; j++){
                long long aux = (a * B[k][j]) % MOD;
                C[i][j] = (C[i][j] + aux) % MOD;
            }
        }
    }
}

void apply(){
    for (int i = 0; i < M; i++){
        VTMP[i] = 0;
        for (int k = 0; k < M; k++){
            long long aux = (T[i][k] * V[k]) % MOD;
            VTMP[i] = (VTMP[i] + aux) % MOD;
        }
    }
    swap(V, VTMP);
}

void binpow(){
    while (N){
        if (N & 1) apply();
        N >>= 1;
        if (N){
            MMult(T, T, TMP);
            swap(T, TMP);
        }
    }
}
