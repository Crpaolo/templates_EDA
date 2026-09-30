#include <bits/stdc++.h>
using namespace std;

// Fractional Cascading para buscar lower_bound(q) en varias listas ordenadas.
// Construccion bottom-up:
// augmented[i] = merge(original[i], cada 2do elemento de augmented[i+1]).
// Cada entrada guarda:
//   origPos  = lower_bound equivalente en original[i]
//   nextPos  = lower_bound equivalente en augmented[i+1]
// Consulta: 1 lower_bound en augmented[0] + O(1) por lista.
//
// Para claridad de examen, se usan valores long long y no se deduplican
// entradas aumentadas; los indices se obtienen con lower_bound.

struct FractionalCascading {
    struct Entry {
        long long value;
        int origPos;
        int nextPos;
        bool operator<(const Entry& o) const { return value < o.value; }
    };

    vector<vector<long long>> orig;
    vector<vector<Entry>> aug;

    FractionalCascading(vector<vector<long long>> lists): orig(move(lists)) {
        int k=orig.size();
        aug.resize(k);
        for(auto &v:orig) sort(v.begin(),v.end());
        if(!k)return;

        for(int i=k-1;i>=0;--i){
            vector<long long> vals=orig[i];
            if(i+1<k){
                // Promover aproximadamente la mitad de la lista aumentada inferior.
                for(int j=1;j<(int)aug[i+1].size();j+=2)
                    vals.push_back(aug[i+1][j].value);
            }
            sort(vals.begin(),vals.end());

            aug[i].reserve(vals.size());
            for(long long x:vals){
                int op=lower_bound(orig[i].begin(),orig[i].end(),x)-orig[i].begin();
                int np=-1;
                if(i+1<k){
                    np=lower_bound(
                        aug[i+1].begin(),aug[i+1].end(),Entry{x,0,0}
                    )-aug[i+1].begin();
                }
                aug[i].push_back({x,op,np});
            }
        }
    }

    vector<int> lower_bounds(long long q) const {
        int k=orig.size();
        vector<int> ans(k);
        if(!k)return ans;

        int pos=lower_bound(
            aug[0].begin(),aug[0].end(),Entry{q,0,0}
        )-aug[0].begin();

        for(int i=0;i<k;i++){
            // Para un template robusto, corregimos localmente alrededor del puente.
            // La propiedad de muestreo garantiza correccion constante.
            int candidate;
            if(pos==(int)aug[i].size()) candidate=orig[i].size();
            else candidate=aug[i][pos].origPos;

            while(candidate>0 && orig[i][candidate-1]>=q) --candidate;
            while(candidate<(int)orig[i].size() && orig[i][candidate]<q) ++candidate;
            ans[i]=candidate;

            if(i+1<k){
                if(pos==(int)aug[i].size()) pos=aug[i+1].size();
                else{
                    pos=aug[i][pos].nextPos;
                    // Como se promovio 1/2, basta ajustar localmente.
                    while(pos>0 && aug[i+1][pos-1].value>=q) --pos;
                    while(pos<(int)aug[i+1].size() && aug[i+1][pos].value<q) ++pos;
                }
            }
        }
        return ans;
    }
};

int main(){
    vector<vector<long long>> L={{2,8,15},{3,5,12},{4,9,13,18,22}};
    FractionalCascading fc(L);
    auto p=fc.lower_bounds(10);
    for(int i=0;i<(int)p.size();i++){
        if(p[i]==(int)L[i].size()) cout<<"END ";
        else cout<<L[i][p[i]]<<' ';
    }
    cout<<'\n'; // 15 12 13
}
