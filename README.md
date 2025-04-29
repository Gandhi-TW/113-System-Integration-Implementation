# 113-System-Integration-Implementation
## Project Overview

This project implements the Max-Truss Breaking Heuristic (MBHS) for the Minimum k-Truss Breaking Set problem. Given an undirected graph, the goal is to remove a minimal set of edges so that the resulting graph contains no k-truss subgraph. The implementation supports large graphs and uses efficient data structures for dynamic maintenance of triangles and truss decompositions.

## Basic Terms and Definitions

- Undirected Graph (G = (V, E)): A graph where V is the set of vertices and E is the set of edges (unordered pairs of vertices).

- Triangle: A set of three vertices with all three pairwise edges present.

- Support: For a given subgraph S and edge e, the support sup_S(e) is the number of triangles in S that contain e.

- k-Truss: A subgraph S of G is a k-truss if every edge in S has support ≥ k−2. The maximal k-truss is the largest (by edges) subgraph satisfying this.

- Trussness: For an edge e, its trussness t(e) is the maximum k such that e belongs to a k-truss in G.

- k-Core: A subgraph S of G is a k-core if every vertex in S has degree ≥ k. The maximal k-core is the largest subgraph satisfying this.
