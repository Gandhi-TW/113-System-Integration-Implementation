#ifndef GRAPH_H
#define GRAPH_H
#include <vector>
#include <unordered_set>
#include <string>

using Edge = std::pair<int,int>;

class Graph {
public:
    // Construct empty graph
    Graph() { }
    // Load graph from edge-list file (one edge per line "u v")
    void loadFromFile(const std::string& filename);
    // Return current max-truss T and its trussness t(G)
    // (We compute iteratively by peeling).
    std::pair<Graph,int> maxTruss(int k_min=2) const;
    // Remove an edge (u,v) from graph
    void removeEdge(int u, int v);
    // Get list of edges and adjacency lists
    std::vector<Edge> edges() const;
    std::vector<std::unordered_set<int>> adj;
};
#endif
