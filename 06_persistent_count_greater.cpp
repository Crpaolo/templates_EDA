#include <bits/stdc++.h>
using namespace std;

// Cuenta # { i in [l,r] : A[i] > k }.
// root[i] = frecuencias del prefijo A[1..i] sobre valores comprimidos.
// FIX importante: la hoja nueva copia old->sum y suma delta.
struct PersistentCountGreater {
    struct Node {
        int sum=0;
        Node *l=nullptr,*r=nullptr;
        Node()=default;
        Node(int s, Node* L, Node* R):sum(s),l(L),r(R){}
    };
    vector<long long> vals;
    vector<Node*> root;
    int M=0;

    Node* build(int tl,int tr){
        Node* v=new Node();
        if(tl!=tr){
            int tm=(tl+tr)/2;
            v->l=build(tl,tm);
            v->r=build(tm+1,tr);
        }
        return v;
    }

    Node* add(Node* old,int tl,int tr,int pos,int delta){
        if(tl==tr) return new Node(old->sum+delta,nullptr,nullptr);
        int tm=(tl+tr)/2;
        Node *L=old->l,*R=old->r;
        if(pos<=tm) L=add(old->l,tl,tm,pos,delta);
        else R=add(old->r,tm+1,tr,pos,delta);
        return new Node(L->sum+R->sum,L,R);
    }

    int query(Node* v,int tl,int tr,int ql,int qr) const{
        if(!v || qr<tl || tr<ql || ql>qr) return 0;
        if(ql<=tl && tr<=qr) return v->sum;
        int tm=(tl+tr)/2;
        return query(v->l,tl,tm,ql,qr)+query(v->r,tm+1,tr,ql,qr);
    }

    PersistentCountGreater(const vector<long long>& a){
        vals=a;
        sort(vals.begin(),vals.end());
        vals.erase(unique(vals.begin(),vals.end()),vals.end());
        M=(int)vals.size();
        if(M==0){ root.push_back(nullptr); return; }
        root.push_back(build(0,M-1));
        for(long long x:a){
            int p=lower_bound(vals.begin(),vals.end(),x)-vals.begin();
            root.push_back(add(root.back(),0,M-1,p,1));
        }
    }

    int count_greater(int l,int r,long long k) const{
        // l,r 1-indexados, inclusivos
        int from=upper_bound(vals.begin(),vals.end(),k)-vals.begin();
        if(from==M) return 0;
        return query(root[r],0,M-1,from,M-1)
             - query(root[l-1],0,M-1,from,M-1);
    }
};

int main(){
    vector<long long> a={5,5,5};
    PersistentCountGreater pst(a);
    cout<<pst.count_greater(1,3,4)<<'\n'; // 3
}
