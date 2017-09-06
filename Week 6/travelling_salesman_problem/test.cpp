#include <iostream>
#include <vector>

using namespace std;
using std::vector;

int to_binary_map(vector<int>& values) {
    int sum;
    for (int i = 0; i < values.size(); i++) {
        sum += 1<<(values[i]);
    }
    return sum;
}

int main() {
    vector<int> tes = {0,1,2,4};
    cout << to_binary_map(tes);
}