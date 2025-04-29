#include <bits/stdc++.h>
using namespace std;
using Edge = pair<int,int>;
inline Edge make_edge(int u, int v) { return u < v ? Edge(u,v) : Edge(v,u); }

// Find triangles
vector<array<int, 3>> find_triangles(const vector<unordered_set<int>>& adj) {
    int n = adj.size();
    vector<array<int, 3>> tris;
    for (int u = 0; u < n; u++) {
        for (int v : adj[u]) {
            if (v <= u) continue;
            // Union
            for (int w : adj[u]) {
                if (w <= v) continue;
                if (adj[v].count(w)) tris.push_back({u,v,w});
            }
        }
    }
    return tris;
}

// Calculate support for each edge 
unordered_map<Edge, int, hash<long long>> compute_support(
    const vector<array<int, 3>>& tris) {
    unordered_map<Edge, int, hash<long long>> sup;
    for (auto &t : tris) {
        int a = t[0], b = t[1], c = t[2];
        Edge e1 = make_edge(a, b);
        Edge e2 = make_edge(a, c);
        Edge e3 = make_edge(b, c);
        sup[e1]++;
        sup[e2]++;
        sup[e3]++;
    }
    return sup;
}

// Fetch k-truss subgraph
void k_truss_subgraph(vector<unordered_set<int>>& adj, int k) {
    while (true) {
        auto tris = find_triangles(adj);
        auto sup = compute_support(tris);
        vector<Edge> toerase;
        for (auto &p : sup) {
            if (p.second < k-2) toerase.push_back(p.first);
        }
        if (toerase.empty()) break;
        for (auto &e : toerase) {
            int u = e.first, v = e.second;
            adj[u].erase(v);
            adj[v].erase(u);
        }
    }
}

int main(){
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, m, k;
    cin >> n >> m >> k;
    vector<unordered_set<int>> adj0(n);
    for (int i = 0; i < m; i++) {
        int u,v; cin >> u >> v;
        adj0[u].insert(v);
        adj0[v].insert(u);
    }
    vector<unordered_set<int>> adj = adj0;
    vector<Edge> removed;

    while (true) {
        auto adjk = adj;
        k_truss_subgraph(adjk, k);
        // Find k-truss subgraph
        unordered_set<long long> edges_in_truss;
        for (int u = 0; u < n; ++u) {
            for (int v : adjk[u]) {
                if (u < v)
                    edges_in_truss.insert((long long)u<<32 | v);
            }
        }
        if (edges_in_truss.empty()) break;

        // Calculate support
        auto tris = find_triangles(adjk);
        auto sup = compute_support(tris);
        
        // choose e': support == k-2
        Edge eprime = {-1,-1};
        for (auto &p : sup) {
            if (p.second == k-2) { eprime = p.first; break; }
        }
        if (eprime.first < 0) {
            // choose minimum support
            eprime = min_element(
                sup.begin(), sup.end(), [](auto &a, auto &b){return a.second < b.second;}
            )->first;
        }
        // Find all triangles which include eprime
        vector<Edge> cand;
        for (auto &t : tris) {
            set<int> S = {t[0],t[1],t[2]};
            Edge ep = eprime;
            if (S.count(ep.first) && S.count(ep.second)) {
                for (auto &e : { make_edge(t[0],t[1]), make_edge(t[0],t[2]), make_edge(t[1],t[2]) }) {
                    if (e != ep) cand.push_back(e);
                }
            }
        }
        // choose edge of the maximum support
        Edge esel = *max_element(cand.begin(), cand.end(),
            [&](auto &a, auto &b){ return sup[a] < sup[b]; });

        // delete esel
        removed.push_back(esel);
        adj[esel.first].erase(esel.second);
        adj[esel.second].erase(esel.first);
    }

    cout << removed.size() << "\n";
    for (auto &e : removed) 
        cout << e.first << " " << e.second << "\n";
    return 0;
}
