#include <chrono>
#include <iostream>
#include <limits>
#include <queue>
#include <vector>

using namespace std;

// Max int used to track if a node has been visted
const int INF = 1e9;


struct Edge {
    int to;
    int weight;
};

struct Graph {
    int V;
    vector<vector<Edge>> adj;

    Graph(int V) : V(V), adj(V) {}

    void addEdge(int u, int v, int w) {
        adj[u].push_back({v, w});
    }

    // BFS implementation (Standard Queue)
    void bfs(int startNode) {
        vector<int> dist(V, INF);
        queue<int> q;

        dist[startNode] = 0;
        q.push(startNode);

        while (!q.empty()) {
            int u = q.front();
            q.pop();

            for (auto &edge : adj[u]) {
                if (dist[edge.to] == INF) {
                    dist[edge.to] = dist[u] + 1;
                    q.push(edge.to);
                }
            }
        }
    }

    // Dijkstra implementation (Priority Queue)
    void dijkstra(int startNode) {
        vector<int> dist(V, INF);
        // Min-heap priority queue: pair<distance, vertex>
        priority_queue<pair<int, int>, vector<pair<int, int>>, greater<pair<int, int>>> pq;

        dist[startNode] = 0;
        pq.push({0, startNode});

        while (!pq.empty()) {
            int d = pq.top().first;
            int u = pq.top().second;
            pq.pop();

            if (d > dist[u]) continue;

            for (auto &edge : adj[u]) {
                if (dist[u] + edge.weight < dist[edge.to]) {
                    dist[edge.to] = dist[u] + edge.weight;
                    pq.push({dist[edge.to], edge.to});
                }
            }
        }
    }
};

int main() {
    // We use a larger graph to see a measurable time difference
    int numVertices = 10000;
    int numEdges = 50000;
    Graph g(numVertices);

    // Create a large random graph with unit weights (1)
    for (int i = 0; i < numEdges; ++i) {
        int u = rand() % numVertices;
        int v = rand() % numVertices;
        g.addEdge(u, v, 1); 
    }

    cout << "Testing Shortest Path on a graph with " << numVertices << " nodes and " 
         << numEdges << " edges (all weights = 1)..." << endl;

    // Measure BFS
    auto start = chrono::high_resolution_clock::now();
    g.bfs(0);
    auto end = chrono::high_resolution_clock::now();
    chrono::duration<double, milli> bfsTime = end - start;
    cout << "BFS Time:      " << bfsTime.count() << " ms" << endl;

    // Measure Dijkstra
    start = chrono::high_resolution_clock::now();
    g.dijkstra(0);
    end = chrono::high_resolution_clock::now();
    chrono::duration<double, milli> dijkstraTime = end - start;
    cout << "Dijkstra Time: " << dijkstraTime.count() << " ms" << endl;

    cout << "\nDifference: Dijkstra is approx " << (dijkstraTime.count() / bfsTime.count()) 
         << "x slower than BFS due to Priority Queue overhead." << endl;

    return 0;
}