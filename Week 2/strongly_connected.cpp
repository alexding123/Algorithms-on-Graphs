#include <algorithm>
#include <iostream>
#include <vector>
#include <stack>

using std::vector;
using std::pair;
using std::stack;

vector<vector<int> > reverse(vector<vector<int> > &adj) {
  int m = adj.size();
  vector<vector<int> > adj_r(m, vector<int>());
  for (size_t i = 0; i < m; i++) {
    int n = adj[i].size();
    for (size_t j = 0; j < n; j++) {
      adj_r[adj[i][j]].push_back(i);
    }
  }
  return adj_r;
}
void dfs(vector<vector<int> > &adj, vector<int> &used, int x, stack<int> &post) {
  used[x] = 1;
  int m = adj[x].size();
  for (size_t i = 0; i < m; i++) {
    if (!used[adj[x][i]]) {
      dfs(adj, used, adj[x][i], post);
    }
  }
  post.push(x);
  return;
}

int number_of_strongly_connected_components(vector<vector<int> > adj) {
  int result = 0;
  vector<vector<int> > adj_r = reverse(adj);
  // std::cout << "Reverse done" << std::endl;
  int m = adj.size();
  vector<int> used(m, 0);
  stack<int> post;
  // populate the stack with post orders
  for (int i = 0; i < m; i++) {
    if (!used[i]) {
      dfs(adj_r, used, i, post);
    }
  }

  // reset "used"
  for (int i = 0; i < m; i++) {
    used[i] = 0;
  }
  while (!post.empty()) {
    int x = post.top(); // sink's index
    post.pop();
    if (!used[x]) {
      stack<int> empty;
      dfs(adj, used, x, empty); // dfs using the sink
      result++;
    }
  }
  return result;
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
  // std::cout << "Computing" << std::endl;
  std::cout << number_of_strongly_connected_components(adj);
}
