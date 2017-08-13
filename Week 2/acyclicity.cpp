#include <iostream>
#include <vector>

using std::vector;
using std::pair;

int explore(vector<vector<int> > &adj, int x, int y, vector<bool> &explored) {
  explored[x] = true;
  int m = adj[x].size();
  for (size_t i = 0; i < m; i++) {
    if (!explored[adj[x][i]]) {
      if (explore(adj, adj[x][i], y, explored) == 1) return 1;
    } else if (adj[x][i] == y) {
      return 1;
    }
  }
  return 0; 
}
int acyclic(vector<vector<int> > &adj) {
  vector<bool> explored(adj.size(), false);
  int m = adj.size();
  for (size_t i = 0; i < m; i++) {
    if (!explored[i]) {
        if (explore(adj, i, i, explored) == 1) return 1;
    }
  }
  return 0;
}


int main() {
  size_t n, m;
  std::cin >> n >> m;
  vector<vector<int> > adj(n, vector<int>());
  for (size_t i = 0; i < m; i++) {
    int x, y;
    std::cin >> x >> y;
    adj[x - 1].push_back(y - 1);
  }
  std::cout << acyclic(adj);
}
