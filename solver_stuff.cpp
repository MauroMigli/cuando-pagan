mt19937_64 rng(chrono::steady_clock::now().time_since_epoch().count());
// mt19937_64 rng(42);   // fixed seed: same tests every run, to reproduce a failure

long long rnd(long long l, long long r){               // uniform in [l, r]
    return uniform_int_distribution<long long>(l, r)(rng);
}

vector<long long> rnd_array(int n, long long l, long long r){
    vector<long long> A(n);
    for (auto& x : A) x = rnd(l, r);
    return A;
}

vector<int> rnd_perm(int n){                           // permutation of 1..n
    vector<int> P(n);
    iota(P.begin(), P.end(), 1);
    shuffle(P.begin(), P.end(), rng);
    return P;
}

vector<pair<int, int>> rnd_tree(int n){                // n − 1 edges, vertices 1..n
    vector<int> LABEL = rnd_perm(n);                   // relabel so vertex 1 isn't always the root
    vector<pair<int, int>> EDGES;
    for (int i = 2; i <= n; i++) EDGES.push_back({LABEL[rnd(1, i - 1) - 1], LABEL[i - 1]});
    shuffle(EDGES.begin(), EDGES.end(), rng);
    return EDGES;
}

string rnd_string(int n, char lo, char hi){            // letters in [lo, hi]
    string s(n, lo);
    for (auto& c : s) c = (char)rnd(lo, hi);
    return s;
}