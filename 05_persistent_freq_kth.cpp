#include <bits/stdc++.h>
using namespace std;

// K-esimo menor en A[l..r], 1-indexado externamente.
// root[i] = histograma del prefijo A[1..i] sobre valores comprimidos.
// Consulta kth usa root[r] - root[l-1].
// O((n+q) log M), espacio O(n log M).
struct PersistentKth {
    struct Node {
        int sum = 0;
        Node *l = nullptr, *r = nullptr;
        Node() = default;
        Node(int s, Node* L, Node* R): sum(s), l(L), r(R) {}
    };

    vector<long long> values;
    vector<Node*> root;
    int M = 0;

    Node* build(int tl, int tr) {
        Node* v = new Node();
        if (tl != tr) {
            int tm=(tl+tr)/2;
            v->l=build(tl,tm);
            v->r=build(tm+1,tr);
        }
        return v;
    }

    Node* add(Node* old, int tl, int tr, int pos, int delta) {
        if (tl == tr) return new Node(old->sum + delta, nullptr, nullptr);
        int tm=(tl+tr)/2;
        Node *L=old->l, *R=old->r;
        if (pos <= tm) L=add(old->l,tl,tm,pos,delta);
        else R=add(old->r,tm+1,tr,pos,delta);
        return new Node(L->sum + R->sum, L, R);
    }

    PersistentKth(const vector<long long>& a) {
        values=a;
        sort(values.begin(),values.end());
        values.erase(unique(values.begin(),values.end()),values.end());
        M=(int)values.size();

        if (M == 0) { root.push_back(nullptr); return; }
        root.push_back(build(0,M-1));
        for (long long x : a) {
            int p=lower_bound(values.begin(),values.end(),x)-values.begin();
            root.push_back(add(root.back(),0,M-1,p,1));
        }
    }

    int kth_index(Node* leftRoot, Node* rightRoot, int tl, int tr, int k) const {
        if (tl == tr) return tl;
        int leftCount = rightRoot->l->sum - leftRoot->l->sum;
        int tm=(tl+tr)/2;
        if (k <= leftCount)
            return kth_index(leftRoot->l,rightRoot->l,tl,tm,k);
        return kth_index(leftRoot->r,rightRoot->r,tm+1,tr,k-leftCount);
    }

    long long kth(int l, int r, int k) const {
        // l,r 1-indexados e inclusivos
        if (l<1 || r>=(int)root.size() || l>r || k<1 || k>r-l+1)
            throw runtime_error("consulta kth invalida");
        int idx=kth_index(root[l-1],root[r],0,M-1,k);
        return values[idx];
    }
};

int main() {
    vector<long long> a={8,3,8,5};
    PersistentKth pst(a);
    cout << pst.kth(2,4,2) << '\n'; // 5
}
