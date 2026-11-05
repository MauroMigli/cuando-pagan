#include <bits/stdc++.h>
using namespace std;
typedef long long ll;
#define rep(i, n) for (int i = 0; i < (int)(n); ++i)
#define repk(i, k, n) for (int i = (int)(k); i < (int)(n); ++i)
#define all(v) (v).begin(), (v).end()

vector<pair<ll, ll>> prime_factorization(ll n){
    vector<pair<ll, ll>> pf;

    for(ll i = 2; i * i <= n; i++){
        ll exp = 0;

        while(n % i == 0){
            exp++;
            n /= i;
        }

        if (exp){
            pf.push_back({i, exp});
        }
    }

    if (n > 1){
        pf.push_back({n, 1});
    }
    return pf;
}

ll mod_binpow(ll a, ll e, ll mod) {
    ll r = 1;
    a %= mod;
    while (e > 0) {
        if (e & 1) r = (r * a) % mod;
        a = (a * a) % mod;
        e >>= 1;
    }
    return r;
}

ll modInverse(ll a, ll mod) {
    return mod_binpow(a, mod - 2, mod);
}

ll extended_gcd(ll a, ll b, ll &x, ll &y) {
    if (b == 0) {
        x = 1;
        y = 0;
        return a;
    }
    ll x1, y1;
    ll g = extended_gcd(b, a % b, x1, y1);
    x = y1;
    y = x1 - y1 * (a / b);
    return g;
}


ll mod_inverse_fermat(ll a, ll p) {
    return mod_binpow(a, p - 2, p);
}