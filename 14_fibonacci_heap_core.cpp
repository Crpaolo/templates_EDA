#include <bits/stdc++.h>
using namespace std;

// Fibonacci Heap (min) - template compacto con insert, get_min,
// decrease_key y extract_min.
// Costos amortizados esperados:
// insert O(1), decrease_key O(1), extract_min O(log n).
//
// Nodo:
// degree = # hijos directos
// mark   = perdio un hijo desde que fue enlazado como hijo
// left/right = lista circular doblemente enlazada
struct FibonacciHeap {
    struct Node {
        long long key;
        int degree=0;
        bool mark=false;
        Node *parent=nullptr,*child=nullptr,*left=this,*right=this;
        explicit Node(long long k):key(k){}
    };

    Node* mn=nullptr;
    int n=0;

    static void remove_from_list(Node* x){
        x->left->right=x->right;
        x->right->left=x->left;
        x->left=x->right=x;
    }

    static void insert_after(Node* a,Node* x){
        x->right=a->right;
        x->left=a;
        a->right->left=x;
        a->right=x;
    }

    void add_root(Node* x){
        x->parent=nullptr;
        x->mark=false;
        if(!mn){
            x->left=x->right=x;
            mn=x;
        }else{
            insert_after(mn,x);
            if(x->key<mn->key)mn=x;
        }
    }

    Node* insert(long long key){
        Node* x=new Node(key);
        add_root(x); ++n;
        return x;
    }

    long long get_min() const{
        if(!mn)throw runtime_error("heap vacio");
        return mn->key;
    }

    void link(Node* y,Node* x){
        // y deja de ser raiz y pasa a ser hijo de x.
        remove_from_list(y);
        y->parent=x;
        y->mark=false;
        if(!x->child){
            x->child=y;
            y->left=y->right=y;
        }else insert_after(x->child,y);
        x->degree++;
    }

    vector<Node*> roots_snapshot() const{
        vector<Node*> v;
        if(!mn)return v;
        Node* x=mn;
        do{v.push_back(x);x=x->right;}while(x!=mn);
        return v;
    }

    void consolidate(){
        if(!mn)return;
        int D=0;
        while((1LL<<D)<=max(1,n))++D;
        vector<Node*> A(D+5,nullptr);

        auto roots=roots_snapshot();
        for(Node* w:roots){
            // w puede haber sido enlazado ya por una colision anterior.
            if(w->parent)continue;
            Node* x=w;
            int d=x->degree;
            while(A[d]){
                Node* y=A[d];
                if(x->key>y->key)swap(x,y);
                link(y,x);
                A[d]=nullptr;
                d++;
                if(d>=(int)A.size())A.resize(d+2,nullptr);
            }
            A[d]=x;
        }

        mn=nullptr;
        for(Node* x:A)if(x){
            x->left=x->right=x;
            add_root(x);
        }
    }

    void cut(Node* x,Node* y){
        if(y->child==x){
            if(x->right==x)y->child=nullptr;
            else y->child=x->right;
        }
        remove_from_list(x);
        y->degree--;
        add_root(x);
    }

    void cascading_cut(Node* y){
        Node* z=y->parent;
        if(!z)return;
        if(!y->mark)y->mark=true;
        else{
            cut(y,z);
            cascading_cut(z);
        }
    }

    void decrease_key(Node* x,long long k){
        if(k>x->key)throw runtime_error("nueva llave mayor");
        x->key=k;
        Node* y=x->parent;
        if(y && x->key<y->key){
            cut(x,y);
            cascading_cut(y);
        }
        if(!mn || x->key<mn->key)mn=x;
    }

    long long extract_min(){
        if(!mn)throw runtime_error("heap vacio");
        Node* z=mn;

        // Snapshot de hijos antes de moverlos.
        vector<Node*> children;
        if(z->child){
            Node* c=z->child;
            do{children.push_back(c);c=c->right;}while(c!=z->child);
        }
        for(Node* c:children){
            remove_from_list(c);
            add_root(c);
        }
        z->child=nullptr;

        Node* next=z->right;
        bool only=(z->right==z);
        remove_from_list(z);
        --n;

        if(only) mn=nullptr;
        else{
            mn=next;
            consolidate();
        }

        long long ans=z->key;
        delete z;
        return ans;
    }
};

int main(){
    FibonacciHeap h;
    auto a=h.insert(10);
    auto b=h.insert(3);
    auto c=h.insert(7);
    h.decrease_key(a,1);
    cout<<h.get_min()<<'\n';       // 1
    cout<<h.extract_min()<<'\n';   // 1
    cout<<h.get_min()<<'\n';       // 3
}
