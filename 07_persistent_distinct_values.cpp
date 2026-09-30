#include <bits/stdc++.h>
using namespace std;

// # valores distintos en A[l..r], offline/preprocesado.
// Invariante de root[i]:
//   hay un 1 SOLO en la ultima aparicion de cada valor dentro de A[1..i].
// Consulta:
//   respuesta = suma(root[r], l, r).
// No necesitas restar root[l-1] si consultas exactamente [l,r].
struct PersistentDistinct {
    struct Node{
        int sum=0;
        Node *l=nullptr,*r=nullptr;
        Node()=default;
        Node(int s,Node*L,Node*R):sum(s),l(L),r(R){}
    };
    int n;
    vector<Node*> root;

    Node* build(int tl,int tr){
        Node* v=new Node();
        if(tl!=tr){
            int tm=(tl+tr)/2;
            v->l=build(tl,tm);
            v->r=build(tm+1,tr);
        }
        return v;
    }

    Node* set_value(Node* old,int tl,int tr,int pos,int value){
        if(tl==tr) return new Node(value,nullptr,nullptr);
        int tm=(tl+tr)/2;
        Node *L=old->l,*R=old->r;
        if(pos<=tm) L=set_value(old->l,tl,tm,pos,value);
        else R=set_value(old->r,tm+1,tr,pos,value);
        return new Node(L->sum+R->sum,L,R);
    }

    int query(Node*v,int tl,int tr,int ql,int qr)const{
        if(qr<tl||tr<ql) return 0;
        if(ql<=tl&&tr<=qr) return v->sum;
        int tm=(tl+tr)/2;
        return query(v->l,tl,tm,ql,qr)+query(v->r,tm+1,tr,ql,qr);
    }

    PersistentDistinct(const vector<long long>& a):n((int)a.size()){
        if(n==0){root.push_back(nullptr);return;}
        root.push_back(build(1,n));
        unordered_map<long long,int> last;
        for(int i=1;i<=n;i++){
            Node* cur=root.back();
            auto it=last.find(a[i-1]);
            if(it!=last.end()) cur=set_value(cur,1,n,it->second,0);
            cur=set_value(cur,1,n,i,1);
            root.push_back(cur);
            last[a[i-1]]=i;
        }
    }

    int distinct(int l,int r)const{
        return query(root[r],1,n,l,r);
    }
};

int main(){
    vector<long long>a={1,2,1,3,2};
    PersistentDistinct pd(a);
    cout<<pd.distinct(2,5)<<'\n'; // {2,1,3} => 3
}
