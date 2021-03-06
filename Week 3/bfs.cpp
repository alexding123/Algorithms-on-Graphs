#include <iostream>
#include <vector>
#include <queue>

using std::vector;
using std::queue;

int distance(vector<vector<int> > &adj, int s, int t) {
  // -1 = not reached
  vector<int> dist(adj.size(), -1);
  queue<int> nodes;

  // initialize
  dist[s] = 0; // dist from s to s is 0

  if (s == t) return 0;
  
  nodes.push(s);
  while (!nodes.empty()) {
    // get top node from queue
    int u = nodes.front(); nodes.pop();

    // loop through all neighbors
    int m = adj[u].size();
    for (int i = 0; i < m; i++) {
      // if neighbor not found before
      if (dist[adj[u][i]] == -1) {
        dist[adj[u][i]] = dist[u] + 1; // or ++dist[u]
        nodes.push(adj[u][i]); // add to queue for next layer

        if (adj[u][i] == t) return dist[adj[u][i]];
      }
    }
  }
  return -1;
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
  int s, t;
  std::cin >> s >> t;
  s--, t--;
  std::cout << distance(adj, s, t);
}
