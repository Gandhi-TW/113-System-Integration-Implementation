// Graph.cpp
#include "Graph.h"
#include <fstream>
#include <queue>
#include <algorithm>

// Load graph from a file of edges (assumes 0-based vertices).
void Graph::loadFromFile(const std::string& filename) {
    std::ifstream in(filename);
    int u,v;
    int maxv = 0;
    std::vector<std::pair<int,int>> eds;
    while(in >> u >> v) {
        if(u==v) continue;
        eds.emplace_back(u,v);
        eds.emplace_back(v,u);
        maxv = std::max({maxv,u,v});
    }
    adj.assign(maxv+1, {});
    for(auto &e: eds) {
        adj[e.first].insert(e.second);
    }
}

// Remove an edge (u,v) and (v,u)
void Graph::removeEdge(int u, int v) {
    adj[u].erase(v);
    adj[v].erase(u);
}

// Return all edges (u<v) in the graph.
std::vector<Edge> Graph::edges() const {
    std::vector<Edge> list;
    for(int u=0; u<adj.size(); u++) {
        for(int v: adj[u]) {
            if(u < v) list.emplace_back(u,v);
        }
    }
    return list;
}

// Compute the maximal t-truss >= k_min. Returns (T,t), where T is the max-truss subgraph and t its trussness.
std::pair<Graph,int> Graph::maxTruss(int k_min) const {
    // Copy the graph to peel.
    Graph H = *this;
    int t = k_min;
    Graph lastNonEmpty;
    int last_t = k_min-1;
    // Peel for k = k_min, k_min+1, ... until empty.
    for(int k = k_min; ; k++) {
        bool changed = true;
        // Iteratively remove edges with support < k-2
        while(changed) {
            changed = false;
            std::vector<Edge> toRemove;
            // Compute support of each edge in H
            for(auto &e : H.edges()) {
                int u=e.first, v=e.second;
                // count common neighbors
                int supp = 0;
                for(int nei : H.adj[u]) if(H.adj[v].count(nei)) supp++;
                if(supp < k-2) {
                    toRemove.emplace_back(u,v);
                }
            }
            if(!toRemove.empty()) {
                changed = true;
                for(auto &e : toRemove) {
                    H.removeEdge(e.first, e.second);
                }
            }
        }
        if(H.edges().empty()) {
            // No edges remain at this k; break
            break;
        }
        // Save this as last non-empty and continue
        lastNonEmpty = H;
        last_t = k;
    }
    return { lastNonEmpty, last_t };
}
