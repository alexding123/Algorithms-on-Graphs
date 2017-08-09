#include <iostream>
#include <vector>

using std::vector;
using std::pair;

void _reach(vector<vector<int> > &adj, int x, vector<bool> &explored) {
  explored[x] = true;
  int m = adj[x].size();
  for (size_t i = 0; i < m; i++) {
    if (!explored[adj[x][i]]) {
      _reach(adj, adj[x][i], explored);
    }
  }
  return; 
}

int number_of_components(vector<vector<int> > &adj) {
  int res = 0;
  int m = adj.size();
  vector<bool> explored(m, false);
  for (size_t i = 0; i < m; i++) {
    if (!explored[i]) {
      res++;
      _reach(adj, i, explored);
    }
  }
  //write your code here
  return res;
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
  std::cout << number_of_components(adj);
}
