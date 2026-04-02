#include <chrono>
#include <iostream>
#include <limits>
#include <queue>
#include <vector>

using namespace std;

// very large int to mark nodes that have not been visted
const int INF = 1e9;

// Directed edge with weight
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

    // BFS implementation
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
                    dist[edge.to] = dist[u] + edge.weight;
                    q.push(edge.to);
                }
            }
        }
    }

    // Dijkstra implementation
    void dijkstra(int startNode) {
        vector<int> dist(V, INF);

        // Min-heap priority queue
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
    // We use a larger graph than the slide to see a measurable time difference
    int numVertices = 10000;
    int numEdges = 50000;
    int numRuns = 5;
    Graph g(numVertices);

    // Create a large random graph with unit weights (1)
    for (int i = 0; i < numEdges; ++i) {
        int u = rand() % numVertices;
        int v = rand() % numVertices;
        g.addEdge(u, v, 1); 
    }

    double totalBfsTime = 0.0;
    double totalDijkstraTime = 0.0;

    for (int run = 1; run <= numRuns; ++run) {
        // Measure BFS
        auto start = chrono::high_resolution_clock::now();
        g.bfs(0);
        auto end = chrono::high_resolution_clock::now();
        chrono::duration<double, milli> bfsTime = end - start;

        // Measure Dijkstra
        start = chrono::high_resolution_clock::now();
        g.dijkstra(0);
        end = chrono::high_resolution_clock::now();
        chrono::duration<double, milli> dijkstraTime = end - start;

        totalBfsTime += bfsTime.count();
        totalDijkstraTime += dijkstraTime.count();

        cout << "Run " << run << ": BFS = " << bfsTime.count()
             << " ms, Dijkstra = " << dijkstraTime.count() << " ms"
             << endl;
    }

    double avgBfs = totalBfsTime / numRuns;
    double avgDijkstra = totalDijkstraTime / numRuns;

    cout << "\n--- Averages over " << numRuns << " runs ---" << endl;
    cout << "BFS Time:      " << avgBfs << " ms" << endl;
    cout << "Dijkstra Time: " << avgDijkstra << " ms" << endl;
    cout << "Difference: Dijkstra is approx " << (avgDijkstra / avgBfs)
         << "x slower than BFS due to Priority Queue overhead." << endl;

    return 0;
}