// MBHC.cpp
#include "Graph.h"
#include <iostream>
#include <algorithm>

// Compute and return list of edges removed by MBHC (cluster-preserving variant):contentReference[oaicite:5]{index=5}.
std::vector<Edge> runMBHC(Graph G, int k) {
    std::vector<Edge> removed;
    while(true) {
        auto [T, t] = G.maxTruss(2);
        if(t < k) break;
        // Pick e' with support = t-2, same as MBHS
        Edge e_prime = {-1,-1};
        for(auto &e: T.edges()) {
            int u=e.first, v=e.second;
            int supp=0;
            for(int w: T.adj[u]) if(T.adj[v].count(w)) supp++;
            if(supp == t-2) { e_prime = e; break; }
        }
        if(e_prime.first<0) e_prime = T.edges()[0];
        int u=e_prime.first, v=e_prime.second;
        // Triangles containing e'
        std::vector<std::tuple<int,int,int>> triangles;
        for(int w: T.adj[u]) if(w!=v && T.adj[v].count(w)) {
            triangles.emplace_back(u,v,w);
        }
        // For each triangle-edge candidate, compute ratio |TRI>=k|/|TRI<k|
        Edge bestEdge = e_prime;
        double bestRatio = -1.0;
        for(auto &tri: triangles) {
            int x,y,z; std::tie(x,y,z) = tri;
            std::vector<Edge> cand = {{x,z},{y,z}};
            for(auto &e: cand) {
                // skip if already removed
                if(!G.adj[e.first].count(e.second)) continue;
                int a=e.first, b=e.second;
                // total triangles in G: count common neighbors
                std::unordered_set<int> common;
                for(int w: G.adj[a]) if(G.adj[b].count(w)) common.insert(w);
                int total = common.size();
                // triangles in T: count common neighbors also adjacent in T
                int inT = 0;
                for(int w: common) {
                    if(T.adj[a].count(w) && T.adj[b].count(w)) inT++;
                }
                int outT = total - inT;
                double ratio;
                if(outT == 0) ratio = double(inT);
                else ratio = double(inT) / double(outT);
                if(ratio > bestRatio) {
                    bestRatio = ratio;
                    bestEdge = e;
                }
            }
        }
        // Remove bestEdge from G
        G.removeEdge(bestEdge.first, bestEdge.second);
        removed.push_back(bestEdge);
    }
    return removed;
}

int main(int argc, char *argv[]) {
    if(argc < 4) {
        std::cout << "Usage: mbhc <input_edge_list> <k>\n";
        return 1;
    }
    std::string infile = argv[1];
    int k = std::stoi(argv[2]);
    Graph G; G.loadFromFile(infile);
    auto sol = runMBHC(G, k);
    std::cout << "Edges removed: " << sol.size() << "\n";
    return 0;
}

