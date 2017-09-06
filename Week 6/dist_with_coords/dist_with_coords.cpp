// DOES NOT WORK
// uni_astar passes

#include <cstdio>
#include <cassert>
#include <vector>
#include <queue>
#include <limits>
#include <utility>
#include <cmath>
#include <iostream>

using namespace std;

// See the explanations of these typedefs and constants in the starter for friend_suggestion
typedef vector<vector<vector<int>>> Adj;
typedef long long Len;
typedef vector<priority_queue<pair<Len, int>,vector<pair<Len,int>>,greater<pair<Len,int>>>> Queue;

const Len INFINI = numeric_limits<Len>::max() / 4;

class AStar {
    // See the descriptions of these fields in the starter for friend_suggestion
    int n_;
    int s_;
    int t_;
    Adj adj_;
    Adj cost_;
    vector<vector<Len>> distance_;
    vector<int> workset_;
    vector<vector<bool>> visited_;
    // Coordinates of the nodes
    vector<std::pair<Len,Len>> xy_;
    vector<pair<bool, Len>> potential_;
public:
    AStar(int n, Adj adj, Adj cost, vector<std::pair<Len,Len>> xy)
        : n_(n), adj_(adj), cost_(cost), distance_(2, vector<Len>(n_, INFINI)), visited_(2, vector<bool>(n, 0)), xy_(xy), potential_(n, {false, 0})
    { workset_.reserve(n); }
    // using Len not double because it's an estimation anyway
    Len euclidean_dist(int u, int v) {
        return sqrt(pow(xy_[v].first - xy_[u].first, 2) + pow(xy_[v].second - xy_[u].second, 2));
    }
    Len potential(int side, int u) {
        Len result;
        if (potential_[u].first) { // if defined
            result = potential_[u].second;
        } else {
            result = (euclidean_dist(u, t_) - euclidean_dist(u, s_))/2; // calculate then store
            potential_[u].first = true; potential_[u].second = result; 
        }
        if (side == 0) return result; else return -result;
    }
    // See the description of this method in the starter for friend_suggestion
    void clear() {
        s_ = 0; t_ = 0;
        for (int i = 0; i < workset_.size(); ++i) {
            int v = workset_[i];
            distance_[0][v] = distance_[1][v] = INFINI;
            visited_[0][v] = visited_[1][v] = false;
        }
        for (int i = 0; i < n_; i++) {
            potential_[i] = {false, 0};
        }
        workset_.clear();
    }

    // See the description of this method in the starter for friend_suggestion
    void visit(Queue& q, int side, int v, Len dist, Len potens) {
        // cout << "Side " << v << " Visiting node " << v << " with dist " << dist << " and potential " << potens << endl;
        if (distance_[side][v] > dist) {
            // cout << "Updating distance from " << distance_[side][v] << " to " << dist << endl;
            distance_[side][v] = dist; // update distance
            
            q[side].push({distance_[side][v] + potens, v}); // update queue
            workset_.push_back(v); // add to visited nodes list
        }
    }
    // relaxes all neighbors of side, marking it as processed
    void process(Queue& q, int side, int u) {
        // cout << "Process: Side " << side << " handling " << u << endl;
        // for each neighbor of u
        for (int i = 0; i < adj_[side][u].size(); i++) {
            // visit once
            int v = adj_[side][u][i];
            visit(q, side, v, distance_[side][u] + cost_[side][u][i], potential(side, u) - potential(side, v));
        }
    }

    Len shortest_path() {
        Len best_dist = INFINI; 
        for (int i = 0; i < workset_.size(); i++) {

            int u = workset_[i]; Len attempt = distance_[0][u] + distance_[1][u];
            // cout << "Trying " << u << " = " << distance_[0][u] << " + " << distance_[1][u] << endl;
            if (attempt < best_dist) {
                best_dist = attempt;
                // cout << "Using it" << endl;
            }
        }

        if (best_dist == INFINITY) return -1; // in case there's no working workset_;
        //cout << "Best dist: " << best_dist << endl;
        //cout << "Best index: " << best_index << endl;
        /*for (int i = 0; i < potential_.size(); i++) {
            cout << i << ": " << potential_[i].second  << endl;
        }*/
        return best_dist;
    }

    // Returns the distance from s to t in the graph
    Len query(int s, int t) {
        clear();
        Queue q(2);
        s_ = s; t_ = t;
        visit(q, 0, s, 0, potential(0, s) - potential(0, t));
        visit(q, 1, t, 0, potential(1, s) - potential(1, t));
        // if either empty - cannot be reached
        while (!q[0].empty() && !q[1].empty()) {
            
            pair<Len, int> v_pair = q[0].top(); q[0].pop(); // extract min
            int v = v_pair.second;
            //cout << "Side 0 processing " << v << endl;
            if (visited_[0][v]) {
                continue;
            } // if it's left-over element - i.e. if it's already visited on the same side, ignore
            visited_[0][v] = true;
            process(q, 0, v); // process v
            if (visited_[1][v])  { 
                //cout << "Return trigger side 0 at " << v << endl;
                return shortest_path();
            } // if visited by other side, find shortest path

            // mirror for reversed graph
            pair<Len, int> v_r_pair = q[1].top(); q[1].pop();
            int v_r = v_r_pair.second;
            // cout << "Side 1 processing " << v_r << endl;
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
    std::vector<std::pair<Len,Len>> xy(n);
    for (int i=0;i<n;++i){
        int a, b;
        scanf("%d%d", &a, &b);
        xy[i] = make_pair(a,b);
    }
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

    AStar astar(n, adj, cost, xy);

    int t;
    scanf("%d", &t);
    for (int i=0; i<t; ++i) {
        int u, v;
        scanf("%d%d", &u, &v);
        printf("%lld\n", astar.query(u-1, v-1));
    }
}
