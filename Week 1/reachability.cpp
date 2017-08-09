#include <iostream>
#include <vector>

using std::vector;
using std::pair;

int _reach(vector<vector<int> > &adj, int x, int y, vector<bool> &explored) {
  explored[x] = true;
  int m = adj[x].size();
  for (size_t i = 0; i < m; i++) {
    if (!explored[adj[x][i]]) {
      if (adj[x][i] == y) return 1;
      if (_reach(adj, adj[x][i], y, explored) == 1) return 1;
    }
  }
  return 0;
}

int reach(vector<vector<int> > &adj, int x, int y) {
  //write your code here
  vector<bool> explored(adj.size(), false);
  return _reach(adj, x, y, explored);
}

int main() {
  size_t n, m;
  std::cin >> n >> m;
  vector<vector<int> > adj(n, vector<int>());
  for (size_t i = 0; i < m; i++) {
    int x, y;
    std::cin >> x >> y;
    adj[x - 1].push_back(y - 1);
    adj[y - 1].push_back(x - 1);
  }
  int x, y;
  std::cin >> x >> y;
  std::cout << reach(adj, x - 1, y - 1);
}
