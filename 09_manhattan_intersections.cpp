#include <bits/stdc++.h>
using namespace std;

// Cuenta intersecciones VERTICAL-HORIZONTAL con extremos INCLUSIVOS.
// Sweep por x:
// Horizontal: ADD en x1, REMOVE en x2+1
// Vertical: QUERY en x, rango y1..y2
// Coordenadas enteras. O(n log n).
struct Fenwick{
    int n;
    vector<int>b;
    Fenwick(int n):n(n),b(n+1,0){}
    void add(int p,int v){for(++p;p<=n;p+=p&-p)b[p]+=v;}
    int pref(int p)const{int s=0;for(++p;p>0;p-=p&-p)s+=b[p];return s;}
    int sum(int l,int r)const{return l>r?0:pref(r)-(l?pref(l-1):0);}
};

struct Event{
    long long x;
    int type; // 0=REMOVE, 1=ADD, 2=QUERY
    long long y1,y2;
    bool operator<(const Event&o)const{
        if(x!=o.x)return x<o.x;
        return type<o.type;
    }
};

int main(){
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n; cin>>n;
    vector<Event> ev;
    vector<long long> ys;

    for(int i=0;i<n;i++){
        long long x1,y1,x2,y2;
        cin>>x1>>y1>>x2>>y2;
        if(x1==x2){ // vertical -> query
            if(y1>y2)swap(y1,y2);
            ev.push_back({x1,2,y1,y2});
            ys.push_back(y1); ys.push_back(y2);
        }else{ // horizontal
            if(x1>x2){swap(x1,x2); swap(y1,y2);}
            long long y=y1; // pre: horizontal
            ev.push_back({x1,1,y,y});
            ev.push_back({x2+1,0,y,y});
            ys.push_back(y);
        }
    }

    sort(ys.begin(),ys.end());
    ys.erase(unique(ys.begin(),ys.end()),ys.end());

    auto lower_idx=[&](long long y){
        return int(lower_bound(ys.begin(),ys.end(),y)-ys.begin());
    };
    auto upper_idx=[&](long long y){
        return int(upper_bound(ys.begin(),ys.end(),y)-ys.begin())-1;
    };

    sort(ev.begin(),ev.end());
    Fenwick fw((int)ys.size());
    long long ans=0;

    for(auto &e:ev){
        if(e.type==1) fw.add(lower_idx(e.y1),+1);
        else if(e.type==0) fw.add(lower_idx(e.y1),-1);
        else{
            int l=lower_idx(e.y1), r=upper_idx(e.y2);
            if(l<=r && l<(int)ys.size() && r>=0) ans+=fw.sum(l,r);
        }
    }
    cout<<ans<<'\n';
}
