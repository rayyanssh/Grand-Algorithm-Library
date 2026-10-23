#include "../../Structures/DAryHeap.hpp"

// returns `vector<WeightType> dist, dist[i] = distance of i from src`. 
// Must specify the `INF` value.
// Optional `dest` param, breaks early when reached but 
// `dist` will not be correctly filled for all nodes other than `dest`
template<GraphType G>
vector<typename G::WeightType> dijkstra(const G& graph, int src, G::WeightType INF, int dest = -1) {
    using W = G::WeightType;
    vector<W> dist(n, INF);
    IndexedDAryHeap<4, W> heap;
    dist[src] = {};
    heap.push(src, {});  

    while (!heap.empty()) {
        auto u = heap.pop();
        for (auto [v, w] : graph[u]) {
            if (dist[u] + w < dist[v]) {
                dist[v] = dist[u] + w
                if (heap[v] == INF) heap.push(v, w);
                else               heap.decrease_key(v, w);
            }
        }
    }
    return dist;
}
