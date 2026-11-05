#include <bits/stdc++.h>
using namespace std;

mt19937_64 rng;

long long rnd(long long l, long long r){               // uniform in [l, r]
    return uniform_int_distribution<long long>(l, r)(rng);
}

void solve() {
    int n = rnd(1, 5);
    cout << 1 << '\n';
    cout << n << '\n';
    for (int i = 0; i < n; i++) cout << rnd(1, 1'000) << ' ';
    cout << '\n';
}

int main(int argc, char* argv[]) {
    ios_base::sync_with_stdio(false); cin.tie(NULL);
    // seed comes from the loop index, so "FAIL on seed 137" can be regenerated with: gen 137
    rng.seed(argc > 1 ? atoll(argv[1]) : 1);
    solve();
}
