#pragma GCC optimize("O3,unroll-loops")
#include <bits/stdc++.h>
using namespace std;
typedef long long ll;
typedef pair<int, int> p;
#define rep(i, n) for (int i = 0; i < (int)(n); ++i)
#define repk(i, k, n) for (int i = (int)(k); i < (int)(n); ++i)

void fft(vector<cd>& a, bool invertir) {
    int n = a.size();

    // Reordenamiento bit-reversal
    for (int i = 1, j = 0; i < n; i++) {
        int bit = n >> 1;
        for (; j & bit; bit >>= 1) j ^= bit;
        j ^= bit;
        if (i < j) swap(a[i], a[j]);
    }

    // Mariposas
    for (int largo = 2; largo <= n; largo <<= 1) {
        double angulo = 2 * PI / largo * (invertir ? -1 : 1);
        int mitad = largo / 2;
        vector<cd> raices(mitad);
        for (int k = 0; k < mitad; k++)
            raices[k] = cd(cos(angulo * k), sin(angulo * k));  // precisas
        for (int i = 0; i < n; i += largo) {
            for (int k = 0; k < mitad; k++) {
                cd u = a[i + k];
                cd v = a[i + k + mitad] * raices[k];
                a[i + k] = u + v;
                a[i + k + mitad] = u - v;
            }
        }
    }

    if (invertir)
        for (cd& x : a) x /= n;
}

// API pública: 
vector<ll> multiplicar(const vector<ll>& a, const vector<ll>& b) {
    if (a.empty() || b.empty()) return {};
    int largo = a.size() + b.size() - 1;
    int tam = 1;
    while (tam < largo) tam <<= 1;

    vector<cd> fa(a.begin(), a.end()), fb(b.begin(), b.end());
    fa.resize(tam);
    fb.resize(tam);
    fft(fa, false);
    fft(fb, false);
    for (int i = 0; i < tam; i++) fa[i] *= fb[i];
    fft(fa, true);

    vector<ll> res(largo);
    for (int i = 0; i < largo; i++) res[i] = llround(fa[i].real());
    return res;
}

// res[p] = sum_j texto[p+j] * patron[j], para p = 0 .. n-m.
// "Poner el patrón sobre el texto en la posición p y sumar productos".
// Internamente: invierte el patrón y lee la convolución en p + m - 1.
vector<ll> correlacionar(const vector<ll>& texto, const vector<ll>& patron) {
    int n = texto.size(), m = patron.size();
    if (m == 0 || m > n) return {};
    vector<ll> inv(patron.rbegin(), patron.rend());
    vector<ll> conv = multiplicar(texto, inv);
    return vector<ll>(conv.begin() + (m - 1), conv.begin() + n);
}