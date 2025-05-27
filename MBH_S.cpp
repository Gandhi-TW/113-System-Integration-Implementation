// MBHS.cpp
#include "Graph.h"
#include <iostream>
#include <unordered_map>
#include <algorithm>

// Compute and return list of edges removed by MBHS (Algorithm 2):contentReference[oaicite:4]{index=4}.
std::vector<Edge> runMBHS(Graph G, int k) {
    std::vector<Edge> removed;
    while(true) {
        auto [T, t] = G.maxTruss(2);
        if(t < k) break;
        // Find an edge e' in T with support = t-2
        Edge e_prime = {-1,-1};
        for(auto &e: T.edges()) {
            int u=e.first, v=e.second;
            // compute support in T
            int supp = 0;
            for(int x: T.adj[u]) if(T.adj[v].count(x)) supp++;
            if(supp == t-2) { e_prime = e; break; }
        }
        if(e_prime.first<0) {
            // fallback: pick any edge
            e_prime = T.edges()[0];
        }
        int u=e_prime.first, v=e_prime.second;
        // Find all triangles containing e' in T
        std::vector<std::tuple<int,int,int>> triangles;
        for(int w: T.adj[u]) {
            if(w==v) continue;
            if(T.adj[v].count(w)) {
                triangles.emplace_back(u,v,w);
            }
        }
        // Among triangle-edges other than e', pick edge with max support
        Edge bestEdge = e_prime;
        int bestSupp = -1;
        for(auto &tri: triangles) {
            int x,y,z; std::tie(x,y,z) = tri;
            // edges: (x,z), (y,z) other than (x,y)=e_prime
            std::vector<Edge> cand = {{x,z},{y,z}};
            for(auto &e : cand) {
                if(T.adj[e.first].count(e.second)) {
                    // compute support in T
                    int supp = 0;
                    for(int w: T.adj[e.first]) if(T.adj[e.second].count(w)) supp++;
                    if(supp > bestSupp) {
                        bestSupp = supp;
                        bestEdge = e;
                    }
                }
            }
        }
        // Remove bestEdge from original G
        G.removeEdge(bestEdge.first, bestEdge.second);
        removed.push_back(bestEdge);
    }
    return removed;
}

int main(int argc, char *argv[]) {
    if(argc < 4) {
        std::cout << "Usage: mbhs <input_edge_list> <k>\n";
        return 1;
    }
    std::string infile = argv[1];
    int k = std::stoi(argv[2]);
    Graph G; G.loadFromFile(infile);
    auto sol = runMBHS(G, k);
    std::cout << "Edges removed: " << sol.size() << "\n";
    // optionally list edges
    // for(auto &e: sol) std::cout << e.first << " " << e.second << "\n";
    return 0;
}

