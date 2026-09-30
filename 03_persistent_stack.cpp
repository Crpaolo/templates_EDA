#include <bits/stdc++.h>
using namespace std;

// Pila totalmente persistente funcional.
// Cada version se identifica por un puntero a su cima.
// push/pop: O(1). El nodo viejo NUNCA se modifica.
template<class T>
struct PersistentStack {
    struct Node {
        T value;
        Node* next;
        Node(T v, Node* n): value(v), next(n) {}
    };

    vector<Node*> root{nullptr};

    int push(int version, T x) {
        root.push_back(new Node(x, root.at(version)));
        return (int)root.size() - 1;
    }

    int pop(int version) {
        Node* r = root.at(version);
        if (!r) throw runtime_error("pop sobre version vacia");
        root.push_back(r->next);
        return (int)root.size() - 1;
    }

    bool empty(int version) const { return root.at(version) == nullptr; }

    T top(int version) const {
        Node* r = root.at(version);
        if (!r) throw runtime_error("top sobre version vacia");
        return r->value;
    }
};

int main() {
    PersistentStack<int> st;
    int v1 = st.push(0, 10);
    int v2 = st.push(v1, 20);
    int v3 = st.pop(v2); // vuelve a [10]
    int v4 = st.push(v1, 99); // rama desde v1
    cout << st.top(v2) << ' ' << st.top(v3) << ' ' << st.top(v4) << '\n';
}
