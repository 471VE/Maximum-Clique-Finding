#ifndef MAX_CLIQUE_TABU_SEARCH_H
#define MAX_CLIQUE_TABU_SEARCH_H

#include <unordered_set>
#include <vector>
#include <deque>

class MaxCliqueTabuSearch {
    public:
        static int GetRandom(int a, int b);

        void ReadGraphFile(std::string filename);
        void RunSearch(int starts, int randomization);

        bool Check();
        void ClearClique();
        std::string PrintClique();
        inline const std::unordered_set<int>& GetClique() { return best_clique; }

    private:
        int ComputeTightness(int vertex);
        void SwapVertices(int vertex, int border);
        void InsertToClique(int i);
        void RemoveFromClique(int k);

        bool VertexInTabuList(const int& vertex, std::deque<int>& tabu_list);
        void AddToTabuList(int vertex, std::deque<int>& tabu_list, const int& tabu_list_max);

        bool Swap1To1();
        bool Swap1to2();
        bool Move();

        void RunInitialHeuristic(int randomization);

    private:
        std::vector<std::unordered_set<int>> neighbor_sets;
        std::vector<std::unordered_set<int>> non_neighbors;
        std::unordered_set<int> best_clique;
        std::vector<int> qco;
        std::vector<int> index;
        std::vector<int> tightness;
        int q_border = 0;
        int c_border = 0;

        std::vector<int> degrees;
        std::vector<std::vector<int>> adjacency_matrix;
        std::deque<int> added_tabu_list;
        std::deque<int> removed_tabu_list;
        const int ADDED_TABU_LIST_MAX = 20;
        const int REMOVED_TABU_LIST_MAX = 40;
};

#endif // MAX_CLIQUE_TABU_SEARCH_H