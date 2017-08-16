#include <iostream>
#include <vector>
#include <queue>

using std::vector;
using std::queue;

int flip(int i) {
  if (i == 0) {
    return 1;
  } else {
    return 0;
  }
}

int bipartite(vector<vector<int> > &adj) {
  // -1 = not reached; 0 & 1 are group numbers
  vector<int> group(adj.size(), -1);
  queue<int> nodes;

  if (adj.size() < 2) return 0; // well you need two nodes to make a bipartite

  // initialize
  group[0] = 0; // first node is group 0
  nodes.push(0);
  while (!nodes.empty()) {
    // get top node from queue
    int u = nodes.front(); nodes.pop();
    // loop through all neighbors
    int m = adj[u].size();
    for (int i = 0; i < m; i++) {
      // if neighbor not found before
      if (group[adj[u][i]] == -1) {
        group[adj[u][i]] = flip(group[u]);
        nodes.push(adj[u][i]); // add to queue for next layer
      } else {
        // if found, check if breaks rule
        if (group[adj[u][i]] == group[u]) return 0;
      }
    }
  }
  return 1;
}

int main() {
  int n, m;
  std::cin >> n >> m;
  vector<vector<int> > adj(n, vector<int>());
  for (int i = 0; i < m; i++) {
    int x, y;
    std::cin >> x >> y;
    adj[x - 1].push_back(y - 1);
    adj[y - 1].push_back(x - 1);
  }
  std::cout << bipartite(adj);
}
