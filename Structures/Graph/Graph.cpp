#pragma once
#include <vector>
#include "../Utils.cpp"
using std::vector;

// All graph represntations assume 1-indexed vertices
 
constexpr bool undirected = false;
constexpr bool directed = true;
 
// Weight can be a simple int weight for example, 
// or a payload structure containing more data about the edge, or be absent (Unweighted)
template<typename Weight>
struct OutgoingEdge { 
    int to;
    [[no_unique_address]] Weight w;
};


// Dynamic Linked Adjacency List (also known as Chained Forward Star).
// 1st param is the weight type, "none" meaning unweighted. 
// 2nd param is to indicate if it is directed or undirected.
// Unweighted graph is analogous to vector<vector<int>>.
// Weighted graph is analogous to vector<vector<pair<int, Weight>>>.
// 1-indexed vertices.
template<typename Weight = none, bool Dir = directed>
struct LinkedAdjList {
    using OutEdge = OutgoingEdge<Weight>;
    vector<OutEdge> pool;
    vector<int> next;
    vector<int> head;
    
    void add_edge(int u, int v, Weight w = {}) { 
        add_directed_edge(u, v, w); 
        if constexpr (Dir == undirected) add_directed_edge(v, u, w); 
    }
    void delete_edge(int u, int v) { 
        delete_directed_edge(u, v); 
        if constexpr (Dir == undirected) delete_directed_edge(v, u); 
    }
    // next_free is the index of the next free slot in the pool, or -1 if there are no free slots
    int next_free = -1;
    void add_directed_edge(int u, int v, Weight w = {}) {
        if (next_free == -1) {
            int pool_id = pool.size();
            pool.push_back({v, w});
            next.push_back(head[u]);
            head[u] = pool_id;
        } else {
            int i = next_free;
            next_free = next[next_free];
            pool[i] = {v, w};
            next[i] = head[u];
            head[u] = i;
        }
    }
    void delete_directed_edge(int u, int v) {
        int i = head[u], prev = -1; 
        while (i != -1 && pool[i].to != v) {
            prev = i; 
            i = next[i];
        }
        if (i == -1) return; // edge not found
        if (prev == -1) head[u] = next[i]; // edge is the first in the list
        else next[prev] = next[i];
        next[i] = next_free;
        next_free = i;
    }
    struct Iterator {
        LinkedAdjList* graph;
        int i;
        decltype(auto) operator*() const { 
            if constexpr (is_same_v<Weight, none>) {
                return graph->pool[i].to;
            } else {
                return graph->pool[i];
            }
        }
        Iterator& operator++() { i = graph->next[i]; return *this; }
        bool operator==(const Iterator& other) const { return i == other.i; }
    }; 

    LinkedAdjList(int vcount, int ecount = 0) : head(vcount + 1, -1) {  
        pool.reserve(ecount);
        next.reserve(ecount);
    }

    auto operator[](int u) { 
        return ranges::subrange(Iterator{this, head[u]}, Iterator{this, -1});
    } 
    using iterator = Iterator;
    auto begin() { return Iterator{this, head[1]}; }
    auto end() { return Iterator{this, -1}; }
};


// Flat Adjacency List. Fast access. Builds from edge list. 1-indexed vertices. 
// Static once built, unless presized to leave some buffer space, in which case deletions/additions are possible.
template<typename Weight = none>
struct FlatAdjList {
    using OutEdge = OutgoingEdge<Weight>; 
    vector<OutEdge> pool;
    vector<int> head;
    vector<int> degree;
    int vcount;
    FlatAdjList(int vcount, int ecount = 0) : head(vcount + 1, 0), degree(vcount + 1, 0), vcount(vcount) {
        pool.reserve(ecount);
    }
    void build(auto& edgelist) { 
    }
    void presize(auto& edgelist) {
        for (auto [u, v] : edgelist) { 
            degree[u]++;
        }
    }
    void presize(span<int> sizes) {
        assert(sizes.size() == vcount);
        for (int i = 1; i <= vcount; i++) {
            head[i] = head[i-1] + sizes[i-1];
            degree[i] = sizes[i-1];
        }
    }
};

