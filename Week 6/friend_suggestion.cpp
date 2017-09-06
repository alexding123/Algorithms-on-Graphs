#include <cstdio>
#include <iostream>
#include <cassert>
#include <vector>
#include <queue>
#include <limits>
#include <utility>

using namespace std;

// External vector of size 2 - for forward and backward search.
// Internal 2-dimensional vector is vector of adjacency lists for each node.
typedef vector<vector<vector<int>>> Adj;

// Distances can grow out of int type
typedef long long Len;

// Vector of two priority queues - for forward and backward searches.
// Each priority queue stores the closest unprocessed node in its head.
typedef vector<priority_queue<pair<Len, int>,vector<pair<Len,int>>,greater<pair<Len,int>>>> Queue;

const Len INFINITY = numeric_limits<Len>::max() / 4;

class Bidijkstra {
    // Number of nodes
    int n_;
    // Graph adj_[0] and cost_[0] correspond to the initial graph,
    // adj_[1] and cost_[1] correspond to the reversed graph.
    // Graphs are stored as vectors of adjacency lists corresponding
    // to nodes.
    // Adjacency list itself is stored in adj_, and the corresponding
    // edge costs are stored in cost_.
    Adj adj_;
    Adj cost_;
    // distance_[0] stores distances for the forward search,
    // and distance_[1] stores distances for the backward search.
    vector<vector<Len>> distance_;
    // Stores all the nodes visited either by forward or backward search.
    vector<int> workset_;
    // Stores a flag for each node which is True iff the node was visited
    // either by forward/backward search.
    vector<vector<bool>> visited_;

public:
    Bidijkstra(int n, Adj adj, Adj cost)
        : n_(n), adj_(adj), cost_(cost), distance_(2, vector<Len>(n, INFINITY)), visited_(2, vector<bool>(n, false))
    { workset_.reserve(n); }

    // Initialize the data structures before new query,
    // clear the changes made by the previous query.
    void clear() {
        for (int i = 0; i < workset_.size(); i++) {
            int v = workset_[i];
            distance_[0][v] = INFINITY; distance_[1][v] = INFINITY;
            visited_[0][v] = false; visited_[1][v] = false;
        }
        workset_.clear();
    }

    // Processes visit of either forward or backward search 
    // (determined by value of side), to node v trying to
    // relax the current distance by dist.
    void visit(Queue& q, int side, int v, Len dist) {
        // cout << "Trying distance: side " << side << " index " << v << " from " << distance_[side][v] << " into " << dist << endl;
        if (distance_[side][v] > dist) {
            // cout << "Updated" <<endl;
            distance_[side][v] = dist; // update distance
            q[side].push({distance_[side][v], v}); // update queue
            workset_.push_back(v); // add to visited nodes list
        }
    }
    // relaxes all neighbors of side, marking it as processed
    void process(Queue& q, int side, int u) {
        // cout << "Processing side " << side << " index " << u << endl;
        // for each neighbor of u
        for (int i = 0; i < adj_[side][u].size(); i++) {
            // visit once
            int v = adj_[side][u][i];
            visit(q, side, v, distance_[side][u] + cost_[side][u][i]);
        }

    }
    Len shortest_path() {
        Len best_dist = INFINITY; 
        for (int i = 0; i < workset_.size(); i++) {
            int u = workset_[i]; Len attempt = distance_[0][u] + distance_[1][u];
            if (attempt < best_dist) {
                best_dist = attempt;
            }
        }
        if (best_dist == INFINITY) return -1; // in case there's no working workset_;
        return best_dist;
    }
    // Returns the distance from s to t in the graph.
    Len query(int s, int t) {
        clear();
        Queue q(2);
        visit(q, 0, s, 0);
        visit(q, 1, t, 0);

        // if either empty - cannot be reached
        while (!q[0].empty() && !q[1].empty()) {
            pair<Len, int> v_pair = q[0].top(); q[0].pop(); // extract min
            int v = v_pair.second;
            if (visited_[0][v]) {
                // cout << "Skipping " << v << endl; 
                continue;
            } // if it's left-over element - i.e. if it's already visited on the same side, ignore
            visited_[0][v] = true;
            process(q, 0, v); // process v
            if (visited_[1][v])  { 
                // cout << "Return trigger side 0 at " << v << endl;
                return shortest_path();
            } // if visited by other side, find shortest path

            // mirror for reversed graph
            pair<Len, int> v_r_pair = q[1].top(); q[1].pop();
            int v_r = v_r_pair.second;
            if (visited_[1][v_r]) continue;
            visited_[1][v_r] = true;
            process(q, 1, v_r);
            if (visited_[0][v_r]) { 
                // cout << "Return trigger side 1 at " << v << endl;
                return shortest_path();
            }
        }
        return -1;
    }
};

int main() {
    int n, m;
    scanf("%d%d", &n, &m);
    Adj adj(2, vector<vector<int>>(n));
    Adj cost(2, vector<vector<int>>(n));
    for (int i=0; i<m; ++i) {
        int u, v, c;
        scanf("%d%d%d", &u, &v, &c);
        adj[0][u-1].push_back(v-1);
        cost[0][u-1].push_back(c);
        adj[1][v-1].push_back(u-1);
        cost[1][v-1].push_back(c);
    }

    Bidijkstra bidij(n, adj, cost);

    int t;
    scanf("%d", &t);
    for (int i=0; i<t; ++i) {
        int u, v;
        scanf("%d%d", &u, &v);
        printf("%lld\n", bidij.query(u-1, v-1));
    }
}
