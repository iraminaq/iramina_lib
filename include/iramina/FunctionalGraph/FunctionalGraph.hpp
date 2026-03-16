#pragma once

#include <vector>
#include <queue>

namespace iramina {

struct FunctionalGraph {
    int n;
    std::vector<int> to;
    std::vector<std::vector<int>> radj;

    std::vector<int> indeg;

    std::vector<int> cycle_id;
    std::vector<int> cycle_pos;
    std::vector<int> dist_to_cycle;
    std::vector<int> root_cycle_vertex;

    std::vector<std::vector<int>> cycles;

    FunctionalGraph() : n(0) {}
    explicit FunctionalGraph(const std::vector<int>& _to) {
        init(_to);
    }

    void init(const std::vector<int>& _to) {
        n = _to.size();
        to = _to;
        radj.assign(n, std::vector<int>());
        indeg.assign(n, 0);

        // 1. 基本情報の構築
        for (int i = 0; i < n; ++i) {
            radj[to[i]].push_back(i);
            indeg[to[i]]++;
        }

        // 2. 葉落としでサイクル頂点を特定
        std::vector<int> alive(n, 1);
        std::queue<int> q;
        for (int i = 0; i < n; ++i) {
            if (indeg[i] == 0) {
                q.push(i);
            }
        }

        while (!q.empty()) {
            int u = q.front();
            q.pop();
            alive[u] = 0;
            int v = to[u];
            indeg[v]--;
            if (indeg[v] == 0) {
                q.push(v);
            }
        }

        // 初期化
        cycle_id.assign(n, -1);
        cycle_pos.assign(n, -1);
        dist_to_cycle.assign(n, -1);
        root_cycle_vertex.assign(n, -1);
        cycles.clear();

        // 3. サイクル列を列挙
        for (int i = 0; i < n; ++i) {
            // サイクルの開始頂点を探す
            if (alive[i] && cycle_id[i] == -1) {
                int cid = cycles.size();
                std::vector<int> cycle;
                int curr = i;
                // 未訪問のサイクル頂点をたどる
                while (cycle_id[curr] == -1) {
                    cycle_id[curr] = cid;
                    cycle_pos[curr] = cycle.size();
                    dist_to_cycle[curr] = 0;
                    root_cycle_vertex[curr] = curr;
                    cycle.push_back(curr);
                    curr = to[curr];
                }
                cycles.push_back(cycle);
            }
        }

        // 4. 逆辺 BFS で木部分を埋める
        for (int cid = 0; cid < (int)cycles.size(); ++cid) {
            const auto& cycle = cycles[cid];
            for (int root : cycle) {
                q.push(root);
            }
        }

        while (!q.empty()) {
            int u = q.front();
            q.pop();

            for (int p : radj[u]) {
                // p がまだサイクル/木に属していない場合 (= cycle_id が -1、または dist_to_cycle が -1)
                // 今回は dist_to_cycle[p] == -1 で判定する
                if (dist_to_cycle[p] == -1) {
                    cycle_id[p] = cycle_id[u];
                    root_cycle_vertex[p] = root_cycle_vertex[u];
                    dist_to_cycle[p] = dist_to_cycle[u] + 1;
                    q.push(p);
                }
            }
        }
    }

    bool is_on_cycle(int v) const {
        return cycle_pos[v] != -1;
    }

    int dist_to_cycle_of(int v) const {
        return dist_to_cycle[v];
    }

    int cycle_length_of(int v) const {
        return cycles[cycle_id[v]].size();
    }

    int root_cycle_vertex_of(int v) const {
        return root_cycle_vertex[v];
    }

    int cycle_count() const {
        return cycles.size();
    }

    int cycle_size(int cid) const {
        return cycles[cid].size();
    }
};

} // namespace iramina
