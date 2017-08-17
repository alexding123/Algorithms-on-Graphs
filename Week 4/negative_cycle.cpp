#include <iostream>
#include <vector>
#include "limits.h"

using std::vector;

int negative_cycle(vector<vector<int> > &adj, vector<vector<int> > &cost) {
  int m = adj.size();
  vector<int> dist(m, 0);
  vector<int> prev(m, -1);
  dist[0] = 0;
  for (int count = 0; count < m-1; count++) {
    for (int i = 0; i < m; i++) {
      for (int j = 0; j < adj[i].size(); j++) {
         if (dist[adj[i][j]] > (dist[i] + cost[i][j])) { 
           dist[adj[i][j]] = (dist[i] + cost[i][j]);
           prev[adj[i][j]] = i;
         }
      }
    }
  }
  // last iteration
  for (int i = 0; i < m; i++) {
      for (int j = 0; j < adj[i].size(); j++) {
         if (dist[adj[i][j]] > (dist[i] + cost[i][j])) { 
           return 1;
         }
      }
    }
  return 0;
}

int main() {
  int n, m;
  std::cin >> n >> m;
  vector<vector<int> > adj(n, vector<int>());
  vector<vector<int> > cost(n, vector<int>());
  for (int i = 0; i < m; i++) {
    int x, y, w;
    std::cin >> x >> y >> w;
    adj[x - 1].push_back(y - 1);
    cost[x - 1].push_back(w);
  }
  std::cout << negative_cycle(adj, cost);
}
