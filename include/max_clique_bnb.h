#ifndef MAX_CLIQUE_BNB_H
#define MAX_CLIQUE_BNB_H

#include "max_clique_tabu_search.h"

class MaxCliqueBnB {
    public:
        void ReadGraphFile(std::string filename);
        void RunBnB();
        bool Check();
        void ClearClique();
        std::string PrintClique();

        inline const std::unordered_set<int>& GetClique() { return best_clique; }
        inline const std::unordered_set<int>& GetHeuristicClique() { return heuristic_clique; }
        inline double GetHeuristicTime() { return heuristic_time; }
        inline double GetBnBTime() { return bnb_time; }

    private:
        std::vector<int> PardalosOrdering();   
        void BnBRecursion(const std::vector<int>& candidates, bool first_iteration = false);
        void SortCandidates(
            const std::vector<int>& candidates, std::vector<int>& sorted_candidates,
            const std::vector<int>& colors, std::vector<int>& sorted_colors,
            int maxcolor
        );     
        void ReverseGreedyColoring(
            const std::vector<int>& candidates,
            std::vector<int>& colors, int& maxcolor
        );
        void FindColor(int vertex, std::vector<bool>& used, std::vector<int>& colors, int& maxcolor);

    private:
        std::vector<std::unordered_set<int>> neighbors;
        std::unordered_set<int> best_clique;
        std::unordered_set<int> heuristic_clique;
        std::unordered_set<int> clique;
        std::string file;
        double heuristic_time;
        double bnb_time;
};

#endif // MAX_CLIQUE_BNB_H