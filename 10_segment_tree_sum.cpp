#include <bits/stdc++.h>
using namespace std;

// Segment Tree iterativo:
// point assignment + range sum [l,r).
// O(log n) por operacion.
struct SegTree{
    int n=1;
    vector<long long> t;
    SegTree(int sz){
        while(n<sz)n<<=1;
        t.assign(2*n,0);
    }
    SegTree(const vector<long long>&a):SegTree((int)a.size()){
        for(int i=0;i<(int)a.size();i++)t[n+i]=a[i];
        for(int i=n-1;i;i--)t[i]=t[2*i]+t[2*i+1];
    }
    void setval(int p,long long x){
        p+=n; t[p]=x;
        for(p>>=1;p;p>>=1)t[p]=t[2*p]+t[2*p+1];
    }
    long long query(int l,int r)const{
        long long L=0,R=0;
        for(l+=n,r+=n;l<r;l>>=1,r>>=1){
            if(l&1)L+=t[l++];
            if(r&1)R+=t[--r];
        }
        return L+R;
    }
};

int main(){
    vector<long long>a={4,1,7,3,9};
    SegTree st(a);
    cout<<st.query(1,4)<<'\n'; // 11
}
