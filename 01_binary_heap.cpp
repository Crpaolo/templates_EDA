#include <bits/stdc++.h>
using namespace std;

// Max-Heap 1-indexado.
// parent(i)=i/2, left=2*i, right=2*i+1.
struct MaxHeap {
    vector<long long> a = {0}; // a[0] no se usa

    bool empty() const { return a.size() == 1; }
    int size() const { return (int)a.size() - 1; }
    long long top() const {
        if (empty()) throw runtime_error("heap vacio");
        return a[1];
    }

    void sift_up(int i) {
        while (i > 1 && a[i/2] < a[i]) {
            swap(a[i/2], a[i]);
            i /= 2;
        }
    }

    void max_heapify(int i) {
        int n = size();
        while (true) {
            int l = 2*i, r = 2*i+1, best = i;
            if (l <= n && a[l] > a[best]) best = l;
            if (r <= n && a[r] > a[best]) best = r;
            if (best == i) break;
            swap(a[i], a[best]);
            i = best;
        }
    }

    void push(long long x) {
        a.push_back(x);
        sift_up(size());
    }

    long long extract_max() {
        if (empty()) throw runtime_error("heap vacio");
        long long ans = a[1];
        a[1] = a.back();
        a.pop_back();
        if (!empty()) max_heapify(1);
        return ans;
    }

    void increase_key(int i, long long new_value) {
        if (i < 1 || i > size() || new_value < a[i])
            throw runtime_error("increase_key invalido");
        a[i] = new_value;
        sift_up(i);
    }

    void build(vector<long long> v) {
        a.assign(1, 0);
        a.insert(a.end(), v.begin(), v.end());
        for (int i = size()/2; i >= 1; --i) max_heapify(i);
    }
};

int main() {
    MaxHeap h;
    h.build({23,20,18,17,15,14,9,12,10,8});
    h.push(30);
    cout << h.extract_max() << '\n'; // 30
    cout << h.top() << '\n';         // 23
}
