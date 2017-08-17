#include <iostream>
#include <limits>
#include <vector>
#include <queue>

using std::vector;
using std::queue;
using std::pair;
using std::priority_queue;

const long long LL_MAX = std::numeric_limits<long long>::max();
using ll = long long;

bool compare_nodes(ll left, ll right, int cost) {
  if (right == LL_MAX) return false;
  return (left > (right + cost));
}

void shortest_paths(vector<vector<int> > &adj, vector<vector<int> > &cost, int s, vector<long long> &dist, vector<int> &reachable, vector<int> &shortest) {
  int m = adj.size();
  dist[s] = 0;
  reachable[s] = 1;

  // repeat m-1 times
  for (int count = 0; count < m-1; count++) {
    // relax every edge
    for (int i = 0; i < m; i++) {
      for (int j = 0; j < adj[i].size(); j++) {
         if (compare_nodes(dist[adj[i][j]], dist[i], cost[i][j])) { 
           // std::cout << "Aftering comparing " << dist[adj[i][j]] << " with " << dist[i] << " and " << cost[i][j] << ", updates to " << dist[i] + cost[i][j] << std::endl;
           dist[adj[i][j]] = dist[i] + cost[i][j];
           reachable[adj[i][j]] = 1;
         }
      }
    }
  }

  // m'th time
  queue<int> nodes;
  for (int i = 0; i < m; i++) {
    for (int j = 0; j < adj[i].size(); j++) {
       if (compare_nodes(dist[adj[i][j]], dist[i], cost[i][j])) {
         nodes.push(i); 
         // std::cout << "Pushing " << i << " to the queue" << std::endl;
       } 
       break; // just need one node
    }
  }
  vector<bool> found(m, false);
  while (!nodes.empty()) {
    int u = nodes.front(); nodes.pop();
    found[u] = true;
    shortest[u] = 0; 
    for (int i = 0; i < adj[u].size(); i++) {
      if (!found[adj[u][i]])
        nodes.push(adj[u][i]);
    }
  }
}

int main() {
  int n, m, s;
  std::cin >> n >> m;
  vector<vector<int> > adj(n, vector<int>());
  vector<vector<int> > cost(n, vector<int>());
  for (int i = 0; i < m; i++) {
    int x, y, w;
    std::cin >> x >> y >> w;
    adj[x - 1].push_back(y - 1);
    cost[x - 1].push_back(w);
  }
  std::cin >> s;
  s--;
  vector<long long> distance(n, std::numeric_limits<long long>::max());
  vector<int> reachable(n, 0);
  vector<int> shortest(n, 1);
  shortest_paths(adj, cost, s, distance, reachable, shortest);
  for (int i = 0; i < n; i++) {
    if (!reachable[i]) {
      std::cout << "*\n";
    } else if (!shortest[i]) {
      std::cout << "-\n";
    } else {
      std::cout << distance[i] << "\n";
    }
  }
}
