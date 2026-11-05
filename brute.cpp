#include <bits/stdc++.h>
using namespace std;

int N;
vector<int> V;

void solve() {
    cin >> N;
    V.resize(N);
    for (auto& x : V) cin >> x;
    long long ans = 0;
    
    for (int i = 0; i < N; i++){
        
    }

    for (auto x : V) ans += x;
    cout << ans << '\n';
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    int T; cin >> T;
    while (T--) solve();
}