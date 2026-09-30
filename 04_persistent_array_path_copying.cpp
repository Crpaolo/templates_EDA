#include <bits/stdc++.h>
using namespace std;

// Persistent Segment Tree para:
// - crear una version desde CUALQUIER version anterior
// - asignar A[pos] = value
// - consultar A[pos] o suma [ql,qr]
// Path copying: O(log n) nodos nuevos por update.
template<class T>
struct PersistentSegmentTree {
    struct Node {
        T sum{};
        Node *l=nullptr, *r=nullptr;
        Node() = default;
        Node(T s, Node* L=nullptr, Node* R=nullptr): sum(s), l(L), r(R) {}
    };

    int n;
    vector<Node*> root;

    Node* build(const vector<T>& a, int tl, int tr) {
        if (tl == tr) return new Node(a[tl]);
        int tm = (tl + tr) / 2;
        Node* L = build(a, tl, tm);
        Node* R = build(a, tm+1, tr);
        return new Node(L->sum + R->sum, L, R);
    }

    PersistentSegmentTree(const vector<T>& a): n((int)a.size()) {
        if (n == 0) root.push_back(nullptr);
        else root.push_back(build(a, 0, n-1));
    }

    Node* assign(Node* old, int tl, int tr, int pos, T value) {
        if (tl == tr) return new Node(value);
        int tm = (tl + tr) / 2;
        if (pos <= tm) {
            Node* L = assign(old->l, tl, tm, pos, value);
            return new Node(L->sum + old->r->sum, L, old->r);
        } else {
            Node* R = assign(old->r, tm+1, tr, pos, value);
            return new Node(old->l->sum + R->sum, old->l, R);
        }
    }

    int assign(int version, int pos, T value) {
        root.push_back(assign(root.at(version), 0, n-1, pos, value));
        return (int)root.size() - 1;
    }

    T query(Node* v, int tl, int tr, int ql, int qr) const {
        if (!v || qr < tl || tr < ql) return T{};
        if (ql <= tl && tr <= qr) return v->sum;
        int tm = (tl + tr) / 2;
        return query(v->l, tl, tm, ql, qr)
             + query(v->r, tm+1, tr, ql, qr);
    }

    T query(int version, int l, int r) const {
        if (l > r) return T{};
        return query(root.at(version), 0, n-1, l, r);
    }

    T get(int version, int pos) const {
        return query(version, pos, pos);
    }
};

int main() {
    vector<long long> a = {1,2,3,4,5,6};
    PersistentSegmentTree<long long> pst(a);
    int v2 = pst.assign(0, 5, 10); // [1,2,3,4,5,10]
    int v3 = pst.assign(v2, 4, 8); // [1,2,3,4,8,10]
    int v4 = pst.assign(0, 4, 30); // branch from initial
    cout << pst.get(0,5) << ' ' << pst.get(v2,5) << ' '
         << pst.get(v3,4) << ' ' << pst.get(v4,4) << '\n';
}
