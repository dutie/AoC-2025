#include "solution.hpp"
#include "common/registry.hpp"
#include "common/utils.hpp"
#include <vector>
#include <queue>

namespace aoc::y2025 {

struct UnionFind {
    std::vector<int> parent;
    std::vector<int> rank;
    
    UnionFind(int n) : parent(n), rank(n, 0) {
        for (int i = 0; i < n; i++) parent[i] = i;
    }
    
    int find(int x) {
        if (parent[x] != x) {
            parent[x] = find(parent[x]); 
        }
        return parent[x];
    }
    
    void unite(int x, int y) {
        int px = find(x);
        int py = find(y);
        if (px == py) return;  // already same circuit
        if (rank[px] < rank[py]) std::swap(px, py);
        parent[py] = px;
        if (rank[px] == rank[py]) rank[px]++;
    }
};

struct edge {
    int a, b;           
    long long weight;   // squared Euclidean distance - fuck you sqrt
    
    bool operator>(const edge& other) const {
        return weight > other.weight;
    }
};

std::string Day08::part1(const std::string& input) {

    // Trying kruskall and disjoint set union to find the minimum spanning tree
    // So edge weights graph is priority queue sorted by weight
    // Use disjoint set union to create sets of sets such that each item is in one and only one set. (circuits)
    // Then iterate through edges in order of weight, adding them to the MST if they connect two different sets.
    // Stop when we have 1000 edges and get the three largest circuits.
    // Return the sum of their sizes.

    auto lines = split(input);
    int max_pairs = 1000;
    
    std::vector<std::array<int, 3>> nodes;  // simple x,y,z storage
    for (const auto& line : lines) {
        auto coords = split(line, ',');
        nodes.push_back({stoi(coords[0]), stoi(coords[1]), stoi(coords[2])});
    }
    
    std::priority_queue<edge, std::vector<edge>, std::greater<edge>> edge_queue;
    int n = nodes.size();
    for (int i = 0; i < n; i++) {
        for (int j = i + 1; j < n; j++) {
            long long dx = nodes[i][0] - nodes[j][0];
            long long dy = nodes[i][1] - nodes[j][1];
            long long dz = nodes[i][2] - nodes[j][2];
            edge_queue.push({i, j, dx*dx + dy*dy + dz*dz});
        }
    }
    
    UnionFind uf(n);
    int connections_made = 0;
    while (connections_made < max_pairs && !edge_queue.empty()) {
        edge e = edge_queue.top();
        edge_queue.pop();
        uf.unite(e.a, e.b);  // join circuits (nothng if already same)
        connections_made++;
    }
    
    std::unordered_map<int, int> circuit_sizes;
    for (int i = 0; i < n; i++) {
        circuit_sizes[uf.find(i)]++;
    }
    
    std::vector<int> sizes;
    for (auto& p : circuit_sizes) {
        sizes.push_back(p.second);
    }
    std::sort(sizes.rbegin(), sizes.rend());
    
    long long result = (long long)sizes[0] * sizes[1] * sizes[2];
    return std::to_string(result);
}

std::string Day08::part2(const std::string& input) {
    auto lines = split(input);
    
    std::vector<std::array<int, 3>> nodes;
    for (const auto& line : lines) {
        if (line.empty()) continue;  // skip empty lines
        auto coords = split(line, ',');
        nodes.push_back({stoi(coords[0]), stoi(coords[1]), stoi(coords[2])});
    }
    
    int n = nodes.size();
    if (n <= 1) return "0";
    
    std::priority_queue<edge, std::vector<edge>, std::greater<edge>> edge_queue;
    for (int i = 0; i < n; i++) {
        for (int j = i + 1; j < n; j++) {
            long long dx = nodes[i][0] - nodes[j][0];
            long long dy = nodes[i][1] - nodes[j][1];
            long long dz = nodes[i][2] - nodes[j][2];
            edge_queue.push({i, j, dx*dx + dy*dy + dz*dz});
        }
    }
    
    UnionFind uf(n);
    int num_circuits = n;
    edge last_edge{-1, -1, 0};
    
    while (!edge_queue.empty()) {
        edge e = edge_queue.top();
        edge_queue.pop();

        if (uf.find(e.a) != uf.find(e.b)) {
            uf.unite(e.a, e.b);
            num_circuits--;
            last_edge = e;
            
            if (num_circuits == 1) {
                break;
            }
        }
    }
    
    long long result = (long long)nodes[last_edge.a][0] * nodes[last_edge.b][0];
    return std::to_string(result);
}

REGISTER_SOLUTION(Day08)

} // namespace aoc::y2025
