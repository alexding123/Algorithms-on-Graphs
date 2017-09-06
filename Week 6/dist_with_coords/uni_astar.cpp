#include <cstdio>
#include <cassert>
#include <vector>
#include <queue>
#include <limits>
#include <utility>
#include <cmath>

using namespace std;

typedef vector<vector<int>> Adj;
typedef long long Len;
typedef priority_queue<pair<Len, int>,vector<pair<Len,int>>,greater<pair<Len,int>>> Queue;

const Len INFINI = numeric_limits<Len>::max() / 4;

class AStar {
    int n_;
    int t_;
    Adj adj_;
    Adj cost_;
    vector<Len> distance_;
    vector<bool> visited_;
	vector<int> workset_;
	vector<Len> potential_;
    // Coordinates of the nodes
    vector<pair<Len,Len>> xy_;

public:
    AStar(int n, Adj adj, Adj cost, vector<pair<Len,Len>> xy)
        : n_(n), adj_(adj), cost_(cost), distance_(vector<Len>(n_, INFINI)), visited_(n), xy_(xy), potential_(vector<Len>(n_, -1))
    {workset_.reserve(n);}

    void clear() {
        for (int i = 0; i < workset_.size(); ++i) {
			int v = workset_[i];
            distance_[v] = INFINI;
            visited_[v] = false;
			potential_[v] = -1;
        }
		workset_.clear();
    }
    void visit(Queue& q, int v, Len dist, Len potens) {
        if (distance_[v] > dist) {
            distance_[v] = dist; // update distance
            q.push({distance_[v]+potens, v}); // update queue
            workset_.push_back(v); // add to visited nodes list
        }
    }
    // not using double because it's an ESTIMATE anyway
    // sticking with long long for all distances
    Len euclidean_dist(int u, int v) {
        return sqrt((xy_[v].first - xy_[u].first)*(xy_[v].first - xy_[u].first) + (xy_[v].second - xy_[u].second)*(xy_[v].second - xy_[u].second));
    }
    Len potential(int u) {
        if (potential_[u] == -1) { // if defined
            potential_[u] = euclidean_dist(t_, u); // calculate then store
        }
        return potential_[u];
    }
    // relaxes all neighbors of side, marking it as processed
    void process(Queue& q, int u) {
        // for each neighbor of u
        for (int i = 0; i < adj_[u].size(); i++) {
            // visit once
            int v = adj_[u][i];
            if (visited_[v] != true) visit(q, v, distance_[u] + cost_[u][i], potential(v));
        }
    }
    // Returns the distance from s to t in the graph
    Len query(int s, int t) {
        clear();
        Queue q;
        t_ = t;
        visit(q, s, 0, potential(s));
        
        while (!q.empty()) {
            pair<Len, int> v_pair = q.top(); q.pop(); // extract min
            int v = v_pair.second;
            if (v == t) {
                if (distance_[t] == INFINI) return -1; else return distance_[t];
            }
            if (!visited_[v]) {
                process(q, v); // process v
                visited_[v] = true;
            }
        }
        return -1;
    }
};


int main() {
    int n, m;
    scanf("%d%d", &n, &m);
    vector<pair<Len,Len>> xy(n);
    for (int i=0;i<n;++i){
        int a, b;
        scanf("%d%d", &a, &b);
        xy[i] = make_pair(a,b);
    }
    Adj adj(n);
    Adj cost(n);
    for (int i=0; i<m; ++i) {
        int u, v, c;
        scanf("%d%d%d", &u, &v, &c);
        adj[u-1].push_back(v-1);
        cost[u-1].push_back(c);
    }

    AStar astar(n, adj, cost, xy);

    int t;
    scanf("%d", &t);
    for (int i=0; i<t; ++i) {
        int u, v;
        scanf("%d%d", &u, &v);
        printf("%lld\n", astar.query(u-1, v-1));
    }
}