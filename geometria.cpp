#pragma GCC optimize("O3,unroll-loops")
#include <bits/stdc++.h>
using namespace std;

// ============================================================
//  0. TYPES & MACROS
// ============================================================
typedef long long ll;
typedef long double ld;
typedef pair<int, int> pii;
#define rep(i, n) for (int i = 0; i < (int)(n); ++i)
#define repk(i, k, n) for (int i = (int)(k); i < (int)(n); ++i)

// Coordinate type: ll for integer problems (exact), ld for real ones.
// Everything below works with either; only this line changes.
typedef ll T;

const ld EPS = 1e-9;
// sign with tolerance (for ll, EPS changes nothing: exact sign)
int sgn(T x) { return (x > EPS) - (x < -EPS); }


// ============================================================
//  1. POINT / VECTOR
// ============================================================
struct pt {
    T x, y;

    pt operator+(pt A) const { return {x + A.x, y + A.y}; }
    pt operator-(pt A) const { return {x - A.x, y - A.y}; }
    pt operator*(T k)  const { return {x * k, y * k}; }
    pt& operator+=(pt A) { x += A.x; y += A.y; return *this; }
    pt& operator-=(pt A) { x -= A.x; y -= A.y; return *this; }
    pt& operator*=(T k)  { x *= k;   y *= k;   return *this; }

    bool operator==(pt A) const { return sgn(x - A.x) == 0 && sgn(y - A.y) == 0; }
    // lexicographic: by x, then by y
    bool operator<(pt A) const { return sgn(x - A.x) ? x < A.x : sgn(y - A.y) < 0; }
};

T dot(pt a, pt b)   { return a.x * b.x + a.y * b.y; }
T cross(pt a, pt b) { return a.x * b.y - a.y * b.x; }

// side of vector b relative to vector a: 1 left (CCW), -1 right (CW), 0 collinear
int dir(pt a, pt b) { return sgn(cross(a, b)); }

// turn at b going a -> b -> c: 1 left, -1 right, 0 straight
int turn(pt a, pt b, pt c) { return sgn(cross(b - a, c - b)); }


// ============================================================
//  2. SEGMENTS
// ============================================================
// is p on the closed segment [a, b]?
bool on_segment(pt p, pt a, pt b) {
    return sgn(cross(b - a, p - a)) == 0 && sgn(dot(a - p, b - p)) <= 0;
}

// do closed segments [p1, p2] and [q1, q2] intersect? (touching counts)
bool segments_intersect(pt p1, pt p2, pt q1, pt q2) {
    pt l1 = p2 - p1, l2 = q2 - q1;
    int d1 = dir(l2, p1 - q1), d2 = dir(l2, p2 - q1);
    int d3 = dir(l1, q1 - p1), d4 = dir(l1, q2 - p1);

    if (d1 == 0 && d2 == 0 && d3 == 0 && d4 == 0) {   // all collinear
        if (p2 < p1) swap(p1, p2);
        if (q2 < q1) swap(q1, q2);
        return !(p2 < q1 || q2 < p1);                  // 1D overlap
    }
    return d1 != d2 && d3 != d4;
}


// ============================================================
//  3. POLYGONS  (vertices in order, CW or CCW)
// ============================================================
// returns 2 * area (exact for integer coords)
T polygon_area_x2(const vector<pt>& P) {
    int n = (int)P.size();
    T a = 0;
    rep(i, n) a += cross(P[i], P[(i + 1) % n]);
    return a < 0 ? -a : a;
}

// Pick's theorem: A = I + B/2 - 1. Integer coords only.
// returns {interior lattice points, boundary lattice points}
pair<ll, ll> polygon_lattice_points(const vector<pt>& P) {
    int n = (int)P.size();
    ll B = 0;
    rep(i, n) {
        pt d = P[(i + 1) % n] - P[i];
        B += gcd((ll)d.x, (ll)d.y);
    }
    ll Ax2 = (ll)polygon_area_x2(P);
    return {(Ax2 - B + 2) / 2, B};
}

// 0 = outside, 1 = inside, 2 = on boundary
int locate_in_polygon(const vector<pt>& P, pt p) {
    int n = (int)P.size();
    bool inside = false;
    rep(i, n) {
        pt a = P[i], b = P[(i + 1) % n];
        if (on_segment(p, a, b)) return 2;
        // edge crosses the horizontal line through p ...
        if ((a.y > p.y) != (b.y > p.y)) {
            // ... and the crossing is to the right of p
            int s = sgn(cross(a - p, b - p));
            if (s == (b.y > a.y ? 1 : -1)) inside = !inside;
        }
    }
    return inside;
}


// ============================================================
//  4. CONVEX HULL  (Andrew's monotone chain, O(n log n))
// ============================================================
// returns hull in CCW order, starting at the lowest-leftmost point.
// keep_collinear = true keeps points lying on hull edges.
vector<pt> convex_hull(vector<pt> P, bool keep_collinear = true) {
    sort(P.begin(), P.end());
    P.erase(unique(P.begin(), P.end()), P.end());
    int n = (int)P.size();
    if (n <= 2) return P;

    // all points collinear: the two chains would duplicate the middle points
    bool all_col = true;
    repk(i, 2, n) if (turn(P[0], P[1], P[i]) != 0) { all_col = false; break; }
    if (all_col) return keep_collinear ? P : vector<pt>{P[0], P[n - 1]};

    // pop on right turns (and on straight turns if not keeping collinear)
    auto bad = [&](pt a, pt b, pt c) {
        int t = turn(a, b, c);
        return keep_collinear ? t < 0 : t <= 0;
    };

    vector<pt> h;
    rep(i, n) {                                        // lower hull
        while ((int)h.size() >= 2 && bad(h[h.size() - 2], h.back(), P[i])) h.pop_back();
        h.push_back(P[i]);
    }
    int lower = (int)h.size();
    for (int i = n - 2; i >= 0; i--) {                 // upper hull
        while ((int)h.size() > lower && bad(h[h.size() - 2], h.back(), P[i])) h.pop_back();
        h.push_back(P[i]);
    }
    h.pop_back();                                      // P[0] was added twice
    return h;
}


// ============================================================
//  5. UTILITIES
// ============================================================
void print128(__int128_t x) {
    if (x < 0) { putchar('-'); x = -x; }
    if (x > 9) print128(x / 10);
    putchar(x % 10 + '0');
}


// ============================================================
int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    return 0;
}