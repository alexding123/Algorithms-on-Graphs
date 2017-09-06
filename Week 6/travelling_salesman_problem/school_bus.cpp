#include <iostream>
#include <algorithm>
#include <vector>

using namespace std;
using std::vector;
typedef vector<vector<int> > Matrix;
const int INF = 1e9;

Matrix read_data() {
    int vertex_count, edge_count;
    std::cin >> vertex_count >> edge_count;
    Matrix graph(vertex_count, vector<int>(vertex_count, INF));
    for (int i = 0; i < edge_count; ++i) {
        int from, to, weight;
        std::cin >> from >> to >> weight;
        --from, --to;
        graph[from][to] = graph[to][from] = weight;
    }
    return graph;
}

/* takes in a binary map and returns
if the value on digit index is 1 (start with 0) */
bool is_in_binary_map(int map, int index) {
    int input = 1<<(index+1);
    int val = map % input;
    return (val >= (input >> 1)); // true if the first digit left is 1
}
int to_binary_map(vector<int>& values) {
    int sum;
    for (int i = 0; i < values.size(); i++) {
        sum += 1<<(values[i]);
    }
    return sum;
}
/* turn digit'th digit into 0 */
int binary_exclude(int map, int digit) {
    return map ^ (1 << digit); 
}

std::pair<int, vector<int>> optimal_path(const Matrix& graph) {
    int n = graph.size();
    // 1 << n is 2^n
    vector<vector<int>> c(1<<n, vector<int>(n, INF)); // outer layer is binary map, inner is ending node

    // initialize
    vector<int> init_vector = {0};
    c[to_binary_map(init_vector)][0] = 0;

    // loop through
    for (int s = 2; s <= n; s++) {
        // enumerate all combinations of graph with s elements
        vector<bool> bits_for_enumeration(n, 0);
        for (int i = 0; i < s; i++) bits_for_enumeration[i] = 1;
        // s leading 1's and n-s 0's afterwards
        do {
            vector<int> map; 
            for (int enumeration_counter = 0; enumeration_counter < n; enumeration_counter++) {
                if (bits_for_enumeration[enumeration_counter]) map.push_back(enumeration_counter);
                // finish constructing this particular permutation with s elements
            }
            int int_map = to_binary_map(map);
            for (int i_enum = 0; i_enum < map.size(); i_enum++) {
                int i = map[i_enum];
                if (i == 0) continue; // i should not be origin
                for (int j_enum = 0; j_enum < map.size(); j_enum++) {
                    int j = map[j_enum];
                    if (i == j) continue; // i and j should be different nodes
                    c[int_map][i] = min(c[int_map][i], c[binary_exclude(int_map, i)][j] + graph[j][i]);
                }
            }
        } while (prev_permutation(bits_for_enumeration.begin(), bits_for_enumeration.end()));
        // credit: https://stackoverflow.com/a/28698654/7718577
    }

    // find the min{C({1,...,n}, i) + d(i, 1)}
    // i.e. find the minimum cycle including 1 to n, ending at i
    // plus the distance from i to 0
    int best_ans = INF;
    vector<int> convert_from(n);
    for (int i = 0; i < n; i++) convert_from[i] = i;
    int int_map = to_binary_map(convert_from);
    for (int i = 0; i < n; i++) {
        best_ans = min(best_ans, c[int_map][i]+graph[i][0]);
    }
    if (best_ans == INF)
        best_ans = -1;

    vector<int> best_path;/*
    for (size_t i = 0; i < best_path.size(); ++i)
        ++best_path[i];*/
    return std::make_pair(best_ans, best_path);
}

void print_answer(const std::pair<int, vector<int> >& answer) {
    std::cout << answer.first << "\n";
    if (answer.first == -1)
        return;
    const vector<int>& path = answer.second;
    for (size_t i = 0; i < path.size(); ++i)
        std::cout << path[i] << " ";
    std::cout << "\n";
}

int main() {
    print_answer(optimal_path(read_data()));
    return 0;
}
