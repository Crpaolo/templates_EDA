#include <bits/stdc++.h>
using namespace std;

// Fenwick/BIT 0-indexado externamente.
// update(pos,delta), prefix(pos), query(l,r) inclusivo.
template<class T>
struct Fenwick {
    int n;
    vector<T> bit;
    Fenwick(int n=0):n(n),bit(n+1,T{}){}

    void add(int pos,T delta){
        for(int i=pos+1;i<=n;i+=i&-i) bit[i]+=delta;
    }

    T prefix(int pos)const{
        T ans{};
        for(int i=pos+1;i>0;i-=i&-i) ans+=bit[i];
        return ans;
    }

    T query(int l,int r)const{
        if(l>r)return T{};
        return prefix(r)-(l?prefix(l-1):T{});
    }
};

int main(){
    Fenwick<long long> fw(5);
    fw.add(1,3);
    fw.add(3,7);
    cout<<fw.query(1,3)<<'\n'; // 10
}
