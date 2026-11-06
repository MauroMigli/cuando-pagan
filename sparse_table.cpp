template<class T>
struct SparseTable {
    vector<vector<T>> ST;
    T combine(T a, T b) { return min(a, b); }
    SparseTable(const vector<T>& A) {
        int n = A.size(), K = __lg(max(n, 1)) + 1;
        ST.assign(K, A);
        for (int k = 1; k < K; k++)
            for (int i = 0; i + (1 << k) <= n; i++)
                ST[k][i] = combine(ST[k - 1][i], ST[k - 1][i + (1 << (k - 1))]);
    }
    T query(int l, int r) {
        int k = __lg(r - l + 1);
        return combine(ST[k][l], ST[k][r - (1 << k) + 1]);
    }
};
