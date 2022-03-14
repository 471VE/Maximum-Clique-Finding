#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <time.h>
#include <filesystem>

#include "max_clique_tabu_search.h"
#include "max_clique_bnb.h"


int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);

    std::vector<std::string> files;
    for(auto& graph: std::filesystem::directory_iterator("graphs")) {
        if (!graph.is_directory()) {
            files.push_back(graph.path().string());
        }      
    }

    std::ofstream fout("max_clique.csv");
    fout << "File;Heuristic Time (sec);BnB Time (sec);Heuristic clique size;Clique size;Clique vertices\n";
    for (std::string file : files) {
        MaxCliqueBnB problem;
        problem.ReadGraphFile(file);
        problem.ClearClique();
        clock_t start = clock();
        problem.RunBnB();
        if (!problem.Check()) {
            std::cout << "*** WARNING: incorrect clique ***\n";
            fout << "*** WARNING: incorrect clique ***\n";
        }
        double time = double(clock() - start) / 1000;

        fout << file << ';' << problem.GetHeuristicTime() << ';' << problem.GetBnBTime() << ';';
        fout << problem.GetHeuristicClique().size() << ';' << problem.GetClique().size() << ';' << problem.PrintClique() << '\n';

        std::cout << file << ", result - " << problem.GetClique().size() << ", heuristic result - ";
        std::cout << problem.GetHeuristicClique().size() << ", heuristic time - " << problem.GetHeuristicTime();
        std::cout << ", BnB time - " << problem.GetBnBTime() << "\n";
    }
    return 0;
}
