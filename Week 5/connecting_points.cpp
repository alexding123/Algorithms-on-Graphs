#include <algorithm>
#include <iostream>
#include <iomanip>
#include <vector>
#include <cmath>

using std::vector;

class disjoint_set {
  private:
    vector<int> _parents; // -1 = root
    vector<int> _ranks;
    int _n;
  public: 
  disjoint_set(int n) { _n = n; _parents.assign(n, -1); _ranks.assign(n, 0);}
  int find(int index) {
    if (_parents[index] == -1) return index;
    _parents[index] = find(_parents[index]); // heuristic by lifting up elements along "find"'s path
    return _parents[index];
  }
  void merge(int i, int j) {
    int i_root = find(i);
    int j_root = find(j);

    if (i_root == j_root) return; // if in same tree, no merge needed

    // attach tree with lower rank to one with higher rank
    if (_ranks[i_root] > _ranks[j_root]) {
      _parents[j_root] = i_root;
    } else if (_ranks[i_root] < _ranks[j_root]) {
      _parents[i_root] = j_root;
    } else {
      // if the same height, move i under j, increment j's height by one
      _parents[i_root] = j_root;
      _ranks[j_root]++;
    }
  }
};

struct edge {
  int first;
  int second;
  double length;
  edge(int x, int y, double len) {first = x; second = y; length = len;}
  edge() {first = 0; second = 0; length = 0.0;}
  static bool comp(edge left, edge right) {
    return (left.length < right.length);
  }
};

double edge_length(int x1, int y1, int x2, int y2) {
  int x_diff = (x2 - x1); int y_diff = (y2 - y1);
  int x_sqr = x_diff * x_diff; int y_sqr = y_diff * y_diff;
  // std::cout << "edge_length: " << x_sqr << " + " << y_sqr << " -> " << sqrt(x_sqr + y_sqr) << std::endl;
  return sqrt(x_sqr + y_sqr);
}
double minimum_distance(vector<int> x, vector<int> y) {
  double result = 0.;
  
  // make a set for each node (x, y)
  int n = x.size();
  disjoint_set set(n);

  // bunch of edges
  vector<edge> edges(n*(n-1)/2);
  int index = 0;
  for (int i = 0; i < (n-1); i++) {
    for (int j = (i+1); j < n; j++) {
      edge to_assign(i, j, edge_length(x[i], y[i], x[j], y[j]));
      edges[index] = to_assign;
      index++;
    }
  }
  // sorting these edges
  std::sort(edges.begin(), edges.end(), edge::comp);
  /* for (int i = 0; i < edges.size(); i++) {
    std::cout << edges[i].first << " " << edges[i].second << ": " << edges[i].length << std::endl;
  } */

  for (int i = 0; i < edges.size(); i++) {
    int u_root = set.find(edges[i].first); int v_root = set.find(edges[i].second);
    if (u_root != v_root) { // then join the two roots
      result += edges[i].length;
      set.merge(u_root, v_root);
    }
  }
  return result;
} 

int main() {
  size_t n;
  std::cin >> n;
  vector<int> x(n), y(n);
  for (size_t i = 0; i < n; i++) {
    std::cin >> x[i] >> y[i];
  }
  std::cout << std::setprecision(10) << minimum_distance(x, y) << std::endl;
}
