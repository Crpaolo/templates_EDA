#include <bits/stdc++.h>
using namespace std;

// Splay Tree de conjunto (sin duplicados).
// search/insert/erase amortizado O(log n).
struct SplayTree{
    struct Node{
        long long key;
        Node *l=nullptr,*r=nullptr,*p=nullptr;
        explicit Node(long long k):key(k){}
    };
    Node* root=nullptr;

    void rotate(Node* x){
        Node* p=x->p;
        Node* g=p?p->p:nullptr;
        if(!p)return;

        if(p->l==x){
            p->l=x->r;
            if(x->r)x->r->p=p;
            x->r=p;
        }else{
            p->r=x->l;
            if(x->l)x->l->p=p;
            x->l=p;
        }
        p->p=x;
        x->p=g;
        if(!g)root=x;
        else if(g->l==p)g->l=x;
        else g->r=x;
    }

    void splay(Node* x){
        if(!x)return;
        while(x->p){
            Node* p=x->p;
            Node* g=p->p;
            if(!g){
                rotate(x); // zig
            }else if((g->l==p)==(p->l==x)){
                rotate(p); // zig-zig
                rotate(x);
            }else{
                rotate(x); // zig-zag
                rotate(x);
            }
        }
    }

    Node* search(long long key){
        Node* cur=root;
        Node* last=nullptr;
        while(cur){
            last=cur;
            if(key==cur->key){splay(cur);return cur;}
            cur=(key<cur->key?cur->l:cur->r);
        }
        if(last)splay(last); // busqueda fallida
        return nullptr;
    }

    bool insert(long long key){
        if(!root){root=new Node(key);return true;}
        if(search(key))return false;
        // root = predecesor o sucesor cercano tras search fallida.
        Node* x=new Node(key);
        if(key<root->key){
            x->l=root->l; if(x->l)x->l->p=x;
            x->r=root; root->l=nullptr; root->p=x;
        }else{
            x->r=root->r; if(x->r)x->r->p=x;
            x->l=root; root->r=nullptr; root->p=x;
        }
        root=x;
        return true;
    }

    // Une A y B suponiendo todas las claves de A < todas las de B.
    Node* join(Node* A,Node* B){
        if(!A){if(B)B->p=nullptr;return B;}
        root=A; A->p=nullptr;
        Node* x=A;
        while(x->r)x=x->r;
        splay(x);
        root->r=B;
        if(B)B->p=root;
        return root;
    }

    bool erase(long long key){
        Node* x=search(key);
        if(!x || root->key!=key)return false;
        Node* A=root->l;
        Node* B=root->r;
        if(A)A->p=nullptr;
        if(B)B->p=nullptr;
        delete root;
        root=join(A,B);
        return true;
    }

    void inorder(Node* v){
        if(!v)return;
        inorder(v->l); cout<<v->key<<' '; inorder(v->r);
    }
};

int main(){
    SplayTree s;
    for(int x:{30,10,20,40,5})s.insert(x);
    s.search(20);
    cout<<"root="<<s.root->key<<'\n';
    s.erase(20);
    s.inorder(s.root); cout<<'\n';
}
