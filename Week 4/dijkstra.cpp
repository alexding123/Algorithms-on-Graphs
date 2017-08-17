#include <iostream>
#include <vector>
#include <queue>

using std::vector;
using std::queue;
using std::pair;
using std::priority_queue;

struct node {
  int point;
  int dist;
  int date;
};

struct node_compare {
  auto operator()(node const& left, node const& right) -> bool {
    if (right.dist == -1) {
      return true;
    } else if (left.dist == -1) {
      return false;
    } else {
      return (left.dist < right.dist);
    }
  }
};

void change_priority(priority_queue<node, vector<node>, node_compare> &q, int point, int dist, vector<int> &up_to_date) {
  up_to_date[point]++;
  node nd = {point, dist, up_to_date[point]};
  q.push(nd);
  return;
}

bool upper_compare(int left, int right, int cost) {
  if (right == -1) {
    return false;
  } else if (left == -1) {
    return true;
  } else {
    return (left > (right + cost));
  }
}

int distance(vector<vector<int> > &adj, vector<vector<int> > &cost, int s, int t) {
  int m = adj.size();
  vector<int> upper(m, -1);
  vector<int> prev(m, -1);

  upper[s] = 0;

  // setup priority_queue
  // -1 is pos infinity
  vector<int> up_to_date(m, 0);
  priority_queue<node, vector<node>, node_compare> q;
  for (int i = 0; i < m; i++) {
    if (i == s) {
      node nd = {i, 0, 0};
      q.push(nd);
    } else {
      node nd = {i, -1, 0};
      q.push(nd);
    }
  }

  while (!q.empty()) {
    node nd = q.top(); q.pop();
    int u = nd.point;
    if (nd.date != up_to_date[u]) continue;
    for (int i = 0; i < adj[u].size(); i++) {
      int v = adj[u][i]; 
      if (upper_compare(upper[v], upper[u], cost[u][i])) {
        upper[v] = upper[u] + cost[u][i];
        prev[v] = u;
        change_priority(q, v, upper[v], up_to_date);
      }
    }
  }

  return upper[t];
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
  int s, t;
  std::cin >> s >> t;
  s--, t--;
  std::cout << distance(adj, cost, s, t);
}
