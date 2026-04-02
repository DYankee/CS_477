#include <iostream>
#include <vector>
#include <queue>
#include <unordered_map>
#include <unordered_set>
#include <climits>


// This program show the difference in performance between BFS and iterative deepening.
// using the graph from slide 36 we can see that BFS in the more time efficient algorithm,
// needing to examine only 22 edges while iterative deepening needed to examine 71.

// The one area where iterative deepening can be more efficient is its space complexity. 
// Because ID uses a depth limited DFS on the branches it discovers, it has a space complexity
// of O(d) where d is the depth limit. While BFS has a space complexity of O(v) where v is the number of vertices.
// This makes ID usefull when traversing large tress that might have shallow paths. 


using namespace std;

// Graph data structure
struct Graph {
    int V;
    vector<vector<int>> adj;

    Graph(int V) : V(V), adj(V) {}

    void addEdge(int u, int v) { adj[u].push_back(v); }
};

// ------------------------------
// BFS
// ------------------------------

struct BFSResult {
    vector<int> dist;   // shortest distance from any source
    vector<int> parent; // parent in shortest-path tree
    int edgesExamined;
};

BFSResult multiSourceBFS(const Graph& g, const vector<int>& sources) {
    BFSResult res;
    res.dist.assign(g.V, -1);
    res.parent.assign(g.V, -1);
    res.edgesExamined = 0;

    queue<int> q;
    for (int s : sources) {
        res.dist[s] = 0;
        q.push(s);
    }

    while (!q.empty()) {
        int u = q.front();
        q.pop();
        for (int v : g.adj[u]) {
            res.edgesExamined++;
            if (res.dist[v] == -1) {
                res.dist[v] = res.dist[u] + 1;
                res.parent[v] = u;
                q.push(v);
            }
        }
    }
    return res;
}

// ------------------------------
// Iterative Deepening 
// ------------------------------

struct IDResult {
    vector<int> dist;
    vector<int> parent;
    int edgesExamined;
    vector<int> edgesPerDepth; // edges examined at each depth limit
};

// Depth-limited DFS from u with remaining depth limit.
// onPath prevents revisiting vertices on the current DFS path.
// When a vertex is first reached at the optimal depth, record it.
static bool dls(
    const Graph& g, int u, int limit, int currentDepth,
    vector<bool>& onPath, vector<int>& dist, vector<int>& parent,
    int& edgesExamined, bool& anyNew
) {
    if (limit == 0) return false;

    for (int v : g.adj[u]) {
        edgesExamined++;
        if (onPath[v]) continue; // avoid cycles on current path

        if (dist[v] == -1) {
            dist[v] = currentDepth + 1;
            parent[v] = u;
            anyNew = true;
        }

        onPath[v] = true;
        dls(g, v, limit - 1, currentDepth + 1, onPath, dist, parent,
            edgesExamined, anyNew);
        onPath[v] = false;
    }
    return false;
}

IDResult multiSourceID(const Graph& g, const vector<int>& sources) {
    IDResult res;
    res.dist.assign(g.V, -1);
    res.parent.assign(g.V, -1);
    res.edgesExamined = 0;

    for (int s : sources) {
        res.dist[s] = 0;
    }

    // Maximum possible depth is V-1
    for (int depthLimit = 1; depthLimit < g.V; depthLimit++) {
        int edgesThisRound = 0;
        bool anyNew = false;

        for (int s : sources) {
            vector<bool> onPath(g.V, false);
            onPath[s] = true;
            dls(g, s, depthLimit, 0, onPath, res.dist, res.parent,
                edgesThisRound, anyNew);
        }

        res.edgesPerDepth.push_back(edgesThisRound);
        res.edgesExamined += edgesThisRound;

        // Stop once no new vertex was discovered in this round
        if (!anyNew) break;
    }
    return res;
}

// ------------------------------
// Helper Functions
// ------------------------------

void printPath(const vector<int>& parent, int target) {
    vector<int> path;
    for (int v = target; v != -1; v = parent[v]) {
        path.push_back(v);
    }
    for (int i = (int)path.size() - 1; i >= 0; i--) {
        cout << path[i];
        if (i > 0) cout << " -> ";
    }
}

// ------------------------------
// Main 
// ------------------------------

int main() {
    // Build the graph from Sedgewick slide 36
    Graph g(13);
    g.addEdge(0, 5);
    g.addEdge(0, 1);
    g.addEdge(2, 0);
    g.addEdge(2, 3);
    g.addEdge(3, 5);
    g.addEdge(3, 2);
    g.addEdge(4, 3);
    g.addEdge(4, 2);
    g.addEdge(5, 4);
    g.addEdge(6, 9);
    g.addEdge(6, 4);
    g.addEdge(6, 8);
    g.addEdge(6, 0);
    g.addEdge(7, 6);
    g.addEdge(7, 9);
    g.addEdge(8, 6);
    g.addEdge(9, 11);
    g.addEdge(9, 10);
    g.addEdge(10, 12);
    g.addEdge(11, 4);
    g.addEdge(11, 12);
    g.addEdge(12, 9);

    vector<int> sources = {1, 7, 10};
    vector<int> targets = {4, 5, 12};

    // BFS 
    cout << "=== Multi-Source BFS ===" << endl;
    BFSResult bfs = multiSourceBFS(g, sources);

    for (int t : targets) {
        cout << "  Vertex " << t << ": dist=" << bfs.dist[t] << "  path=";
        printPath(bfs.parent, t);
        cout << endl;
    }
    cout << "  Total edges examined: " << bfs.edgesExamined << endl;

    // Iterative Deepening 
    cout << "\n=== Multi-Source Iterative Deepening ===" << endl;
    IDResult id = multiSourceID(g, sources);

    for (int t : targets) {
        cout << "  Vertex " << t << ": dist=" << id.dist[t] << "  path=";
        printPath(id.parent, t);
        cout << endl;
    }

    int cumulative = 0;
    for (int d = 0; d < (int)id.edgesPerDepth.size(); d++) {
        cumulative += id.edgesPerDepth[d];
        cout << "  Depth limit " << (d + 1) << ": "
             << id.edgesPerDepth[d] << " edges this round, "
             << cumulative << " cumulative" << endl;
    }
    cout << "  Total edges examined: " << id.edgesExamined << endl;

    // Side-by-side
    cout << "\n=== Comparison ===" << endl;
    cout << "  Target | BFS dist | BFS edges | ID edges" << endl;
    cout << "  -------|----------|-----------|----------" << endl;
    cout << "  (all)  |    -     |   " << bfs.edgesExamined
         << "      |   " << id.edgesExamined << endl;

    return 0;
}