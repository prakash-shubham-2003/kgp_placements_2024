#include<bits/stdc++.h>
using namespace std;
#define ll long long
#define int long long
#define nl '\n'
#define all(v) ((v).begin()), ((v).end())
#define sz(v) (int)((v).size())
#define IOS ios_base::sync_with_stdio(false); cin.tie(NULL); cout.tie(NULL);
const int mod = 1000000007;

#ifndef ONLINE_JUDGE
#include "debug.cpp"
#else
#define debug(...)
#endif

class SegmentTree{
    vector<vector<int>> tree;
    vector<int> lazy;
    int n;
    public:
    SegmentTree(int n){
        this->n = n;
        lazy.assign(4*n, 0);
        tree.assign(4*n, vector<int>(31));
    }
    vector<int> comb(vector<int> &a, vector<int> &b){
        vector<int> res(31);
        for(int i = 0; i < 31; i++){
            res[i] = a[i] + b[i];
        }
        return res;
    }
    vector<int> getBitRepresentation(int x){
        vector<int> res(31);
        for(int i = 0; i < 31; i++){
            res[i] = (x >> i) & 1;
        }
        return res;
    }
    void build(vector<int> &a, int v, int tl, int tr){
        if(tl == tr){
            tree[v] = getBitRepresentation(a[tl]);
        }else{
            int tm = (tl + tr) / 2;
            build(a, 2*v, tl, tm);
            build(a, 2*v+1, tm+1, tr);
            tree[v] = comb(tree[2*v], tree[2*v+1]);
        }
    }
    void push(int v, int tl, int tr){
        if(lazy[v] != 0){
            for(int i = 0; i < 31; i++){
                if(lazy[v] & (1 << i)){
                    tree[v][i] = (tr - tl + 1) - tree[v][i];
                }
            }
            if(tl != tr){
                lazy[2*v] ^= lazy[v];
                lazy[2*v+1] ^= lazy[v];
            }
            lazy[v] = 0;
        }
    }
    vector<int> sum(int v, int tl, int tr, int l, int r){
        if(l > r) return vector<int>(31);
        push(v, tl, tr);
        if(l == tl && r == tr) return tree[v];
        int tm = (tl + tr) / 2;
        auto left = sum(2*v, tl, tm, l, min(r, tm));
        auto right = sum(2*v+1, tm+1, tr, max(l, tm+1), r);
        return comb(left, right);
    }
    void update(int v, int tl, int tr, int l, int r, int val){
        if(l > r) return;
        push(v, tl, tr);
        if(l == tl && r == tr){
            lazy[v] = val;
            push(v, tl, tr);
        }else{
            int tm = (tl + tr) / 2;
            update(2*v, tl, tm, l, min(r, tm), val);
            update(2*v+1, tm+1, tr, max(l, tm+1), r, val);
            tree[v] = comb(tree[2*v], tree[2*v+1]);
        }
    }
};

void Heisenberg(){
    int n, q, answer = 0;
    cin >> n >> q;
    vector<int> a(n);
    for(int &x : a) cin >> x;
    SegmentTree st(n);
    st.build(a, 1, 0, n-1);
    while(q--){
        int x, y, z; cin >> x >> y >> z;
        x--;
        int lo = x, hi = n - 1, mid, ans = -1;
        while(lo <= hi){
            mid = (lo + hi) / 2;
            auto res = st.sum(1, 0, n-1, x, mid);
            if(res[y] == mid - x + 1){
                ans = mid;
                lo = mid + 1;
            }else{
                hi = mid - 1;
            }
        }
        if(ans != -1){
            answer += ans - x + 1;
            st.update(1, 0, n-1, x, ans, z);
        }
    }
    cout << answer;
}
signed main(){
    IOS
    { Heisenberg(); cout << '\n'; }
}