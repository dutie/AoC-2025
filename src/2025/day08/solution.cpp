#include "solution.hpp"
#include "common/registry.hpp"
#include "common/utils.hpp"
#include <vector>
#include <queue>
#include <algorithm>
#include <array>

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
    
    bool unite(int x, int y) {
        int px = find(x);
        int py = find(y);
        if (px == py) return false;  // already same circuit
        if (rank[px] < rank[py]) std::swap(px, py);
        parent[py] = px;
        if (rank[px] == rank[py]) rank[px]++;
        return true;
    }
};

struct edge {
    int a, b;           
    int64_t weight;
    
    bool operator>(const edge& other) const {
        return weight > other.weight;
    }
};

using Point = std::array<int, 3>;

// K-d tree node
struct KDNode {
    int point_idx;
    int split_dim;
    KDNode* left = nullptr;
    KDNode* right = nullptr;
};

class KDTree {
    std::vector<Point>& points;
    KDNode* root = nullptr;
    std::vector<KDNode> node_pool;
    int pool_idx = 0;
    
    KDNode* build(std::vector<int>& indices, int start, int end, int depth) {
        if (start >= end) return nullptr;
        
        int dim = depth % 3;
        int mid = (start + end) / 2;
        
        // (?) partial (?) sort to find median
        std::nth_element(indices.begin() + start, indices.begin() + mid, indices.begin() + end,
            [&](int a, int b) { return points[a][dim] < points[b][dim]; });
        
        KDNode* node = &node_pool[pool_idx++];
        node->point_idx = indices[mid];
        node->split_dim = dim;
        node->left = build(indices, start, mid, depth + 1);
        node->right = build(indices, mid + 1, end, depth + 1);
        return node;
    }
    
    inline int64_t dist_sq(int i, int j) const {
        int64_t dx = points[i][0] - points[j][0];
        int64_t dy = points[i][1] - points[j][1];
        int64_t dz = points[i][2] - points[j][2];
        return dx*dx + dy*dy + dz*dz;
    }
    
    void knn_search(KDNode* node, int target, int k,
                    std::priority_queue<std::pair<int64_t, int>>& best) const {
        if (!node) return;
        
        int idx = node->point_idx;
        if (idx != target) {
            int64_t d = dist_sq(target, idx);
            if (best.size() < (size_t)k) {
                best.push({d, idx});
            } else if (d < best.top().first) {
                best.pop();
                best.push({d, idx});
            }
        }
        
        int dim = node->split_dim;
        int64_t diff = points[target][dim] - points[idx][dim];
        int64_t diff_sq = diff * diff;
        
        KDNode* first = diff < 0 ? node->left : node->right;
        KDNode* second = diff < 0 ? node->right : node->left;
        
        knn_search(first, target, k, best);
        
        // Only search other branch if closer points
        if (best.size() < (size_t)k || diff_sq < best.top().first) {
            knn_search(second, target, k, best);
        }
    }
    
public:
    KDTree(std::vector<Point>& pts) : points(pts) {
        int n = pts.size();
        node_pool.resize(n);
        std::vector<int> indices(n);
        for (int i = 0; i < n; i++) indices[i] = i;
        root = build(indices, 0, n, 0);
    }
    
    // knn at index
    std::vector<std::pair<int64_t, int>> knn(int target, int k) const {
        std::priority_queue<std::pair<int64_t, int>> best;  // max-heap
        knn_search(root, target, k, best);
        
        std::vector<std::pair<int64_t, int>> result;
        while (!best.empty()) {
            result.push_back(best.top());
            best.pop();
        }
        return result;
    }
};

std::pair<int64_t, edge> solve(const std::string& input, int stop_after_connections) {
    auto lines = split(input);
    
    std::vector<Point> nodes;
    for (const auto& line : lines) {
        if (line.empty()) continue;
        auto coords = split(line, ',');
        nodes.push_back({stoi(coords[0]), stoi(coords[1]), stoi(coords[2])});
    }
    
    int n = nodes.size();
    if (n <= 1) return {0, {-1, -1, 0}};
    
    KDTree tree(nodes);
    const int k = 7;
    
    std::vector<edge> edges;
    edges.reserve(n * k / 2);
    
    for (int i = 0; i < n; i++) {
        auto neighbors = tree.knn(i, k);
        for (auto& [dist, j] : neighbors) {
            if (i < j) {  // avoid duplicates
                edges.push_back({i, j, dist});
            }
        }
    }
    
    std::sort(edges.begin(), edges.end(), [](const edge& a, const edge& b) {
        return a.weight < b.weight;
    });
    
    UnionFind uf(n);
    int connections_made = 0;
    int mst_edges = 0;
    edge last_mst_edge{-1, -1, 0};
    
    for (const auto& e : edges) {
        if (stop_after_connections > 0 && connections_made >= stop_after_connections) break;
        if (stop_after_connections <= 0 && mst_edges >= n - 1) break;
        
        if (stop_after_connections > 0) {
            // d1
            uf.unite(e.a, e.b);
            connections_made++;
        } else {
            // d2
            if (uf.unite(e.a, e.b)) {
                mst_edges++;
                last_mst_edge = e;
            }
        }
    }
    
    if (stop_after_connections > 0) {
        // d1
        std::unordered_map<int, int> circuit_sizes;
        for (int i = 0; i < n; i++) {
            circuit_sizes[uf.find(i)]++;
        }
        
        std::vector<int> sizes;
        for (auto& p : circuit_sizes) {
            sizes.push_back(p.second);
        }
        std::sort(sizes.rbegin(), sizes.rend());
        
        int64_t result = (int64_t)sizes[0] * sizes[1] * sizes[2];
        return {result, {-1, -1, 0}};
    } else {
        // d2
        int64_t result = (int64_t)nodes[last_mst_edge.a][0] * nodes[last_mst_edge.b][0];
        return {result, last_mst_edge};
    }
}

std::string Day08::part1(const std::string& input) {
    auto [result, _] = solve(input, 1000);
    return std::to_string(result);
}

std::string Day08::part2(const std::string& input) {
    auto [result, _] = solve(input, -1);
    return std::to_string(result);
}

REGISTER_SOLUTION(Day08)

} // namespace aoc::y2025