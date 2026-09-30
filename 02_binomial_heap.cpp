#include <bits/stdc++.h>
using namespace std;

// Min Binomial Heap.
// Union = merge por grado + enlaces de arboles del mismo grado.
// Este template prioriza claridad para examen.
struct BinomialHeap {
    struct Node {
        long long key;
        int degree = 0;
        Node *parent = nullptr, *child = nullptr, *sibling = nullptr;
        explicit Node(long long k): key(k) {}
    };

    Node* head = nullptr;

    static Node* merge_root_lists(Node* a, Node* b) {
        Node dummy(0), *tail = &dummy;
        while (a && b) {
            if (a->degree <= b->degree) {
                tail->sibling = a; a = a->sibling;
            } else {
                tail->sibling = b; b = b->sibling;
            }
            tail = tail->sibling;
        }
        tail->sibling = a ? a : b;
        return dummy.sibling;
    }

    static void link(Node* y, Node* z) {
        // Pre: y y z tienen mismo grado y z->key <= y->key.
        y->parent = z;
        y->sibling = z->child;
        z->child = y;
        z->degree++;
    }

    void unite(BinomialHeap& other) {
        head = merge_root_lists(head, other.head);
        other.head = nullptr;
        if (!head) return;

        Node *prev = nullptr, *x = head, *nxt = x->sibling;
        while (nxt) {
            if (x->degree != nxt->degree ||
                (nxt->sibling && nxt->sibling->degree == x->degree)) {
                prev = x;
                x = nxt;
            } else if (x->key <= nxt->key) {
                x->sibling = nxt->sibling;
                link(nxt, x);
            } else {
                if (prev) prev->sibling = nxt;
                else head = nxt;
                link(x, nxt);
                x = nxt;
            }
            nxt = x->sibling;
        }
    }

    void insert(long long key) {
        BinomialHeap one;
        one.head = new Node(key);
        unite(one);
    }

    long long get_min() const {
        if (!head) throw runtime_error("heap vacio");
        long long ans = LLONG_MAX;
        for (Node* x = head; x; x = x->sibling) ans = min(ans, x->key);
        return ans;
    }

    long long extract_min() {
        if (!head) throw runtime_error("heap vacio");

        Node *prevMin = nullptr, *minNode = head;
        Node *prev = nullptr;
        for (Node* x = head; x; prev = x, x = x->sibling) {
            if (x->key < minNode->key) {
                minNode = x;
                prevMin = prev;
            }
        }

        if (prevMin) prevMin->sibling = minNode->sibling;
        else head = minNode->sibling;

        // Revertir lista de hijos para que quede por grado creciente.
        Node *rev = nullptr, *c = minNode->child;
        while (c) {
            Node* nxt = c->sibling;
            c->parent = nullptr;
            c->sibling = rev;
            rev = c;
            c = nxt;
        }

        BinomialHeap children;
        children.head = rev;
        unite(children);

        long long ans = minNode->key;
        delete minNode;
        return ans;
    }
};

int main() {
    BinomialHeap h;
    for (int x : {10,3,7,1,12,5}) h.insert(x);
    while (h.head) cout << h.extract_min() << ' ';
    cout << '\n';
}
