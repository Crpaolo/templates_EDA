#include <bits/stdc++.h>
using namespace std;

// Estructura estatica tipo Range Tree 2D simplificado.
// Puntos (x,y,w). Consulta rectangulo semiabierto:
// [x1,x2) x [y1,y2), devuelve suma de pesos.
// Arbol principal por x; cada nodo guarda y ordenados + prefix sums.
// Construccion O(n log n), espacio O(n log n), query O(log^2 n).

struct Point{ long long x,y,w; };

struct RangeTree2D{
    struct Node{
        int l,r;
        vector<long long> ys, pref; // pref[0]=0
    };
    vector<Point> p;
    vector<Node> tr;
    int n;

    RangeTree2D(vector<Point> pts):p(move(pts)){
        sort(p.begin(),p.end(),[](auto&a,auto&b){
            if(a.x!=b.x)return a.x<b.x;
            return a.y<b.y;
        });
        n=p.size();
        tr.resize(max(1,4*n));
        if(n)build(1,0,n-1);
    }

    void build(int v,int l,int r){
        tr[v].l=l; tr[v].r=r;
        if(l==r){
            tr[v].ys={p[l].y};
            tr[v].pref={0,p[l].w};
            return;
        }
        int m=(l+r)/2;
        build(2*v,l,m); build(2*v+1,m+1,r);
        vector<pair<long long,long long>> tmp;
        tmp.reserve(r-l+1);
        for(int i=l;i<=r;i++)tmp.push_back({p[i].y,p[i].w});
        sort(tmp.begin(),tmp.end());
        tr[v].pref.assign(1,0);
        for(auto [y,w]:tmp){
            tr[v].ys.push_back(y);
            tr[v].pref.push_back(tr[v].pref.back()+w);
        }
    }

    long long sumY(int v,long long y1,long long y2)const{
        auto &ys=tr[v].ys; auto &pr=tr[v].pref;
        int L=lower_bound(ys.begin(),ys.end(),y1)-ys.begin();
        int R=lower_bound(ys.begin(),ys.end(),y2)-ys.begin();
        return pr[R]-pr[L];
    }

    long long queryIndex(int v,int ql,int qr,long long y1,long long y2)const{
        int l=tr[v].l,r=tr[v].r;
        if(qr<l||r<ql)return 0;
        if(ql<=l&&r<=qr)return sumY(v,y1,y2);
        return queryIndex(2*v,ql,qr,y1,y2)
             + queryIndex(2*v+1,ql,qr,y1,y2);
    }

    long long query(long long x1,long long x2,long long y1,long long y2)const{
        if(!n||x1>=x2||y1>=y2)return 0;
        int L=lower_bound(p.begin(),p.end(),x1,[](const Point&a,long long x){return a.x<x;})-p.begin();
        int R=lower_bound(p.begin(),p.end(),x2,[](const Point&a,long long x){return a.x<x;})-p.begin()-1;
        if(L>R)return 0;
        return queryIndex(1,L,R,y1,y2);
    }
};

int main(){
    vector<Point> p={{1,1,5},{2,3,7},{4,2,10},{3,4,2}};
    RangeTree2D rt(p);
    cout<<rt.query(1,4,1,4)<<'\n'; // 12
}
