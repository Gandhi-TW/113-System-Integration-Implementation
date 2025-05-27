// SNH.cpp
#include "Graph.h"
#include <iostream>
#include <limits>

// Estimate “secondary impact” of removing edge (u,v):
// how many adjacent edges would drop below required support.
int secondaryImpact(const Graph& G, int u, int v) {
    int impact = 0;
    // For each neighbor x of u (except v), if (u,x) loses all triangles
    for(int x : G.adj[u]) if(x != v) {
        int supp = 0;
        for(int w : G.adj[u])
            if(w != x && G.adj[x].count(w)) supp++;
        if(supp < 0) ++impact;  // treat support<0 as “impacted”
    }
    // Similarly for v
    for(int x : G.adj[v]) if(x != u) {
        int supp = 0;
        for(int w : G.adj[v])
            if(w != x && G.adj[x].count(w)) supp++;
        if(supp < 0) ++impact;
    }
    return impact;
}

// SNH: at each iteration, peel the max-truss, pick an edge e' of support t-2,
// gather the two other edges in each triangle {e',f,g}, and delete the one
// whose secondaryImpact is smallest.
std::vector<Edge> runSNH(Graph G, int k) {
    std::vector<Edge> removed;
    while(true) {
        auto [T, t] = G.maxTruss(2);
        if(t < k) break;
        // find e' with support == t-2
        Edge e_prime{-1,-1};
        for(auto &e : T.edges()) {
            int u=e.first, v=e.second, supp=0;
            for(int w:T.adj[u]) if(T.adj[v].count(w)) ++supp;
            if(supp == t-2) { e_prime = e; break; }
        }
        if(e_prime.first<0) e_prime = T.edges()[0];
        int u=e_prime.first, v=e_prime.second;

        // collect candidate edges (u,w) and (v,w) for each triangle (u,v,w)
        std::vector<Edge> cands;
        for(int w : T.adj[u])
            if(w!=v && T.adj[v].count(w)) {
                cands.emplace_back(u,w);
                cands.emplace_back(v,w);
            }

        // pick the one with minimal secondaryImpact
        Edge best = cands.front();
        int bestImp = std::numeric_limits<int>::max();
        for(auto &e : cands) {
            int imp = secondaryImpact(G, e.first, e.second);
            if(imp < bestImp) {
                bestImp = imp;
                best = e;
            }
        }
        G.removeEdge(best.first, best.second);
        removed.push_back(best);
    }
    return removed;
}

int main(int argc, char* argv[]) {
    if(argc < 3) {
        std::cout << "Usage: snh <edge_list> <k>\n";
        return 1;
    }
    Graph G; G.loadFromFile(argv[1]);
    int k = std::stoi(argv[2]);
    auto sol = runSNH(G, k);
    std::cout << "Edges removed: " << sol.size() << "\n";
    return 0;
}

