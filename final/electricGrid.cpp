#include <iostream>
#include <vector>
#include <string>
#include <map>
#include <set>
#include <queue>
#include <limits>
#include <algorithm>
#include <iomanip>

using namespace std;

// Structure to represent a city node
struct City {
    string name;
    double demand_MW;
    double generation_MW;

    // Surplus/deficit calculation
    double getSurplus() const {
        return generation_MW - demand_MW;
    }

    bool isSelfSufficient() const {
        return generation_MW >= demand_MW;
    }
};

// Structure to represent an edge in the grid
struct Edge {
    string from;
    string to;
    double distance_km;
    double capacity_MW;

    Edge(string f, string t, double d, double c)
        : from(f), to(t), distance_km(d), capacity_MW(c) {}
};

typedef CityMap CityMap;

// Main Graph class for electrical grid
class ElectricalGrid {
private:
    CityMap cities;
    map<string, vector<Edge>> adjacencyList;

public:
    // Add city to the graph
    void addCity(
        const string& name, double demand, double generation
    ) {
        City city;
        city.name = name;
        city.demand_MW = demand;
        city.generation_MW = generation;
        cities[name] = city;

        if (adjacencyList.find(name) == adjacencyList.end()) {
            adjacencyList[name] = vector<Edge>();
        }
    }

    // Add bidirectional edge (for both power and cars)
    void addEdge(
        const string& from,
        const string& to,
        double distance,
        double capacity
    ) {
        adjacencyList[from].push_back(
            Edge(from, to, distance, capacity)
        );
        adjacencyList[to].push_back(
            Edge(to, from, distance, capacity)
        );
    }

    // Get city information
    City getCity(const string& name) const {
        return cities.at(name);
    }

    // Get all cities
    CityMap getAllCities() const { return cities; }

    // Get all neighbors of a city
    vector<Edge> getNeighbors(const string& city) const {
        if (adjacencyList.find(city) != adjacencyList.end()) {
            return adjacencyList.at(city);
        }
        return vector<Edge>();
    }

    // Get Self sufficient cities
    CityMap getSelfSufficient() const {
        CityMap selfSufficient;
        for (const auto& [key, value] : cities) {
            if (value.isSelfSufficient()) {
                selfSufficient[key] = value;
            }
        }
        return selfSufficient;
    }

    // Get Deficit cities
    CityMap getDeficit() const {
        CityMap deficit;
        for (const auto& [key, value] : cities) {
            if (!value.isSelfSufficient()) {
                deficit[key] = value;
            }
        }
        return deficit;
    }

    // Print grid statistics
    void printStatistics() const {
        cout << "\n=== Electrical Grid Statistics ===" << endl;
        cout << "Total cities: " << cities.size() << endl;

        int selfSufficient = 0;
        double totalSurplus = 0;

        for (const auto& pair : cities) {
            const City& city = pair.second;
            if (city.isSelfSufficient()) {
                selfSufficient++;
                totalSurplus += city.getSurplus();
            }
        }

        cout << "Self-sufficient cities: " << selfSufficient
             << endl;
        cout << "Total surplus from self-sufficient cities: "
             << totalSurplus << " MW" << endl;
        cout << "==================================\n" << endl;
    }

    /*         Algorithms           */
    /*
        Breadth-First Search for Augmenting Paths

        This performs a BFS traversal to find an augmenting 
        path from source to sink in the residual graph with 
        available capacity. Used as a subroutine in the 
        Edmonds-Karp max flow algorithm.

        Parameters:
        - source: starting node for BFS
        - sink: destination node to reach
        - residual: residual graph with remaining capacities
        - parent: output map storing parent pointers for 
          path reconstruction

        Returns:
        - true if a path from source to sink exists with 
          positive capacity
        - false otherwise

        Time Complexity: O(V + E)
    */
    bool bfsMaxFlow(
        const string& source,
        const string& sink,
        map<string, map<string, double>>& residual,
        map<string, string>& parent
    ) const {
        set<string> visited;
        queue<string> q;

        q.push(source);
        visited.insert(source);

        while (!q.empty()) {
            string u = q.front();
            q.pop();

            // Explore all neighbors with remaining capacity
            for (const auto& edge : residual[u]) {
                string v = edge.first;
                double cap = edge.second;

                if (
                    visited.find(v) == visited.end() &&
                    cap > 1e-9
                ) {
                    visited.insert(v);
                    parent[v] = u;

                    if (v == sink) {
                        return true; // Found path to sink
                    }
                    q.push(v);
                }
            }
        }
        return false; // No path found
    }

    /*
        Dijkstra's Shortest Path Algorithm

        Computes the shortest path between two cities based 
        on distance. Uses a priority queue to efficiently 
        select the next closest unvisited node.

        Parameters:
        - start: source city name
        - end: destination city name

        Returns:
        - pair containing:
            shortest distance from start to end (in km)
            vector of city names representing the path from 
            start to end

        Time Complexity: O((V + E) log V) with binary heap
    */
    pair<double, vector<string>> 
    dijkstra( const string& start, const string& end) const {
        map<string, double> distances;
        map<string, string> previous;

        // Initialize distances
        for (const auto& pair : cities) {
            distances[pair.first] =
                numeric_limits<double>::infinity();
        }
        distances[start] = 0;

        // Priority queue: (distance, city)
        priority_queue<
            pair<double, string>,
            vector<pair<double, string>>,
            greater<pair<double, string>>>
            pq;
        pq.push({0, start});

        while (!pq.empty()) {
            auto [dist, current] = pq.top();
            pq.pop();

            if (dist > distances[current]) continue;
            if (current == end) break;

            for (const Edge& edge : getNeighbors(current)) {
                double newDist =
                    distances[current] + edge.distance_km;

                if (newDist < distances[edge.to]) {
                    distances[edge.to] = newDist;
                    previous[edge.to] = current;
                    pq.push({newDist, edge.to});
                }
            }
        }

        // Reconstruct path
        vector<string> path;
        string current = end;

        if (
            previous.find(end) != previous.end() ||
            start == end
        ) {
            while (current != start) {
                path.push_back(current);
                current = previous[current];
            }
            path.push_back(start);
            reverse(path.begin(), path.end());
        }

        return {distances[end], path};
    }

    /*
        Maximum Flow using Edmonds-Karp Algorithm

        This computes the maximum flow from source to sink
        using BFS-based Ford-Fulkerson method (Edmonds-Karp).

        Parameters:
        - flowGraph: map of edge capacities to add to the
          network
        - source: starting node
        - sink: destination node

        Returns:
        - pair of (max flow value, flow on each edge)

        Time Complexity: O(V * E^2)
    */
    pair<double, map<pair<string, string>, double>>
    computeMaxFlow(
        const map<pair<string, string>, double>& flowGraph,
        const string& source,
        const string& sink
    ) const {
        // Build residual graph
        map<string, map<string, double>> residual;
        map<pair<string, string>, double> capacity;

        // Initialize residual graph with given capacities
        for (const auto& edge : flowGraph) {
            string from = edge.first.first;
            string to = edge.first.second;
            double cap = edge.second;

            capacity[{from, to}] = cap;
            residual[from][to] += cap;

            // Initialize reverse edge with 0 capacity
            if (
                residual[to].find(from) ==
                residual[to].end()
            ) {
                residual[to][from] = 0;
            }
        }

        double maxFlow = 0;
        map<string, string> parent;

        // Find augmenting paths until none exist
        while (
            bfsMaxFlow(source, sink, residual, parent)
        ) {
            // Find minimum capacity along the path
            double pathFlow =
                numeric_limits<double>::infinity();
            string v = sink;

            while (v != source) {
                string u = parent[v];
                pathFlow = min(pathFlow, residual[u][v]);
                v = u;
            }

            // Update residual capacities along the path
            v = sink;
            while (v != source) {
                string u = parent[v];
                residual[u][v] -= pathFlow;
                residual[v][u] += pathFlow;
                v = u;
            }

            maxFlow += pathFlow;
            parent.clear();
        }

        // Calculate actual flow on each edge
        map<pair<string, string>, double> flow;
        for (const auto& edge : capacity) {
            string from = edge.first.first;
            string to = edge.first.second;
            double originalCap = edge.second;
            double remainingCap = residual[from][to];
            flow[{from, to}] = originalCap - remainingCap;
        }

        return {maxFlow, flow};
    }

    /*
    Minimum Cost Flow using Successive Shortest Paths
    (SPFA-based Bellman-Ford)

    Uses edge-list representation to correctly handle
    bidirectional edges (each direction gets its own
    forward/reverse edge pair).

    Parameters:
    - flowGraph: map of (from, to) -> capacity
    - costGraph: map of (from, to) -> cost per unit flow
    - source: starting node
    - sink: destination node
    - requiredFlow: the amount of flow to push

    Returns:
    - tuple of (total cost, total flow achieved,
      flow on each edge)

    Time Complexity: O(F * V * E)
    */
    tuple<double, double, map<pair<string, string>, double>>
    computeMinCostFlow(
        const map<pair<string, string>, double>& flowGraph,
        const map<pair<string, string>, double>& costGraph,
        const string& source,
        const string& sink,
        double requiredFlow
    ) const {
        // Map string node names to integer indices
        map<string, int> nodeIndex;
        vector<string> nodeName;

        auto getIndex = [&](const string& name) -> int {
            auto it = nodeIndex.find(name);
            if (it == nodeIndex.end()) {
                int idx = (int)nodeName.size();
                nodeIndex[name] = idx;
                nodeName.push_back(name);
                return idx;
            }
            return it->second;
        };

        // Collect all nodes
        for (const auto& e : flowGraph) {
            getIndex(e.first.first);
            getIndex(e.first.second);
        }

        int N = (int)nodeName.size();
        int S = nodeIndex[source];
        int T = nodeIndex[sink];

        // Edge structure with reverse pointer
        struct MCFEdge {
            int to, rev;
            double cap, cost;
        };

        vector<vector<MCFEdge>> graph(N);

        // Adds a directed edge and its reverse (cancel) edge
        auto addMCFEdge = [&](
            int u, int v, double cap, double cost
        ) {
            graph[u].push_back(
                {v, (int)graph[v].size(), cap, cost}
            );
            graph[v].push_back(
                {u, (int)graph[u].size() - 1, 0.0, -cost}
            );
        };

        // Track original edge locations for flow reconstruction
        vector<pair<int, int>> edgeLocs;

        for (const auto& e : flowGraph) {
            int u = nodeIndex[e.first.first];
            int v = nodeIndex[e.first.second];
            double cap = e.second;
            double cost = 0;
            auto it = costGraph.find(e.first);
            if (it != costGraph.end()) cost = it->second;

            edgeLocs.push_back({u, (int)graph[u].size()});
            addMCFEdge(u, v, cap, cost);
        }

        double totalFlow = 0, totalCost = 0;
        const double INF = numeric_limits<double>::infinity();

        // Successive shortest paths
        while (totalFlow < requiredFlow - 1e-9) {
            // SPFA (Bellman-Ford with queue) to find
            // shortest cost path
            vector<double> dist(N, INF);
            vector<int> prevv(N, -1), preve(N, -1);
            vector<bool> inQueue(N, false);
            vector<int> relaxCount(N, 0);

            dist[S] = 0;
            queue<int> q;
            q.push(S);
            inQueue[S] = true;

            bool negativeCycle = false;

            while (!q.empty() && !negativeCycle) {
                int u = q.front();
                q.pop();
                inQueue[u] = false;

                for (int i = 0; i < (int)graph[u].size();
                     i++) {
                    const MCFEdge& e = graph[u][i];
                    if (
                        e.cap > 1e-9 &&
                        dist[u] + e.cost < dist[e.to] - 1e-9
                    ) {
                        dist[e.to] = dist[u] + e.cost;
                        prevv[e.to] = u;
                        preve[e.to] = i;
                        if (!inQueue[e.to]) {
                            q.push(e.to);
                            inQueue[e.to] = true;
                            relaxCount[e.to]++;
                            if (relaxCount[e.to] >= N) {
                                negativeCycle = true;
                                break;
                            }
                        }
                    }
                }
            }

            // No augmenting path or negative cycle detected
            if (negativeCycle || dist[T] >= INF / 2) break;

            // Find bottleneck along the path
            double pushFlow = requiredFlow - totalFlow;
            for (int v = T; v != S; v = prevv[v]) {
                pushFlow = min(
                    pushFlow, graph[prevv[v]][preve[v]].cap
                );
            }

            // Augment flow along the path
            for (int v = T; v != S; v = prevv[v]) {
                MCFEdge& e = graph[prevv[v]][preve[v]];
                e.cap -= pushFlow;
                graph[v][e.rev].cap += pushFlow;
            }

            totalFlow += pushFlow;
            totalCost += pushFlow * dist[T];
        }

        // Reconstruct flow on original edges
        map<pair<string, string>, double> flowResult;
        int idx = 0;
        for (const auto& e : flowGraph) {
            auto [nodeIdx, edgeIdx] = edgeLocs[idx];
            double origCap = e.second;
            double remainCap = graph[nodeIdx][edgeIdx].cap;
            flowResult[e.first] = origCap - remainCap;
            idx++;
        }

        return {totalCost, totalFlow, flowResult};
    }
};