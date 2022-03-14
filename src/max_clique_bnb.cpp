#include "max_clique_bnb.h"

#include <fstream>
#include <sstream>
#include <iostream>
#include <algorithm>

void MaxCliqueBnB::ReadGraphFile(std::string filename) {
    std::ifstream fin(filename);
    std::string line;
    file = filename;
    int vert = 0, edges = 0;
    while (getline(fin, line)) {
        if (line[0] == 'c')
            continue;
        if (line[0] == 'p') {
            std::stringstream s(line);
            char c;
            std::string in;
            s >> c >> in >> vert >> edges;
            neighbors.resize(vert);
        } else {
            std::stringstream s(line);
            char c;
            int st, fn;
            s >> c >> st >> fn;
            neighbors[st - 1].insert(fn - 1);
            neighbors[fn - 1].insert(st - 1);
        }
    }
}


void MaxCliqueBnB::RunBnB() {
    clock_t heuristic_start = clock();
    MaxCliqueTabuSearch st;
    st.ReadGraphFile(file);
    st.RunSearch(10000, 7);
    heuristic_clique = st.GetClique();
    best_clique = heuristic_clique;
    
    heuristic_time = static_cast<double>(clock() - heuristic_start) / 1000;
    
    clock_t bnb_start = clock();
    auto candidates = PardalosOrdering();
    BnBRecursion(candidates, true);
    bnb_time = static_cast<double>(clock() - bnb_start) / 1000;
}


bool MaxCliqueBnB::Check() {
    for (int i : clique)
        for (int j : clique)
            if (i != j && neighbors[i].count(j) == 0) {
                std::cout << "Returned subgraph is not clique\n";
                return false;
            }
    return true;
}


void MaxCliqueBnB::ClearClique() {
    best_clique.clear();
    clique.clear();
}


std::string MaxCliqueBnB::PrintClique() {
    std::string result = "";
    bool started = false;
    for (const auto& vertex: best_clique) {
        if (started)
            result += ", ";
        else
            started = true;
        result += std::to_string(vertex);
    }
    return result;
}


void MaxCliqueBnB::BnBRecursion(const std::vector<int>& candidates, bool first_iteration) {
    if (candidates.empty()) {
        if (clique.size() > best_clique.size())
            best_clique = clique;
        return;
    }

    clock_t coloring_start = clock();
    std::vector<int> colors(neighbors.size(), 0);
    int maxcolor = 1;
    if (!first_iteration)
        ReverseGreedyColoring(candidates, colors, maxcolor);
    else {
        for (int i = static_cast<int>(candidates.size()) - 1; i >=0; --i)
            colors[candidates[i]] = static_cast<int>(candidates.size()) - i;
        maxcolor = static_cast<int>(candidates.size());
    }
    std::vector<int> sorted_candidates, sorted_colors;
    SortCandidates(candidates, sorted_candidates, colors, sorted_colors, maxcolor);

    for (int i = 0; i < sorted_colors.size(); ++i) {
        if (clique.size() + sorted_colors[i] <= best_clique.size()) {
            return;
        }
        std::vector<int> new_candidates;
        new_candidates.reserve(sorted_candidates.size());
        for (int c = i + 1; c < sorted_candidates.size(); ++c) {
            if (neighbors[sorted_candidates[i]].count(sorted_candidates[c]) != 0)
                new_candidates.push_back(sorted_candidates[c]);
        }
        if (clique.size() + new_candidates.size() + 1 <= best_clique.size()) {
            continue;
        }
        clique.insert(sorted_candidates[i]);
        BnBRecursion(new_candidates);
        clique.erase(sorted_candidates[i]);
    }
}

void MaxCliqueBnB::SortCandidates(
    const std::vector<int>& candidates, std::vector<int>& sorted_candidates,
    const std::vector<int>& colors, std::vector<int>& sorted_colors,
    int maxcolor
) {
    for (int color = maxcolor; color >= 1; --color)
        for (int c = 0; c < candidates.size(); ++c)
            if (colors[candidates[c]] == color) {
                sorted_candidates.push_back(candidates[c]);
                sorted_colors.push_back(color);
            }       
}


std::vector<int> MaxCliqueBnB::PardalosOrdering() {
    std::vector<int> degrees;
    degrees.reserve(neighbors.size());
    for (const auto& vertex: neighbors)
        degrees.push_back(static_cast<int>(vertex.size()));

    std::vector<int> vertices_sorted_by_degrees;
    vertices_sorted_by_degrees.reserve(neighbors.size());
    for (int i = 0; i < neighbors.size(); ++i)
        vertices_sorted_by_degrees.push_back(i);
    
    for (int i = 0; i < vertices_sorted_by_degrees.size() - 1; ++i) {
        std::sort(
            vertices_sorted_by_degrees.begin() + i, vertices_sorted_by_degrees.end(),
            [&degrees](const int& vertex_a, const int& vertex_b)
            { return degrees[vertex_a] < degrees[vertex_b]; });

        for (auto vertex: neighbors[vertices_sorted_by_degrees[i]])
            --degrees[vertex];
    }
    return vertices_sorted_by_degrees;
}


void MaxCliqueBnB::ReverseGreedyColoring(
    const std::vector<int>& candidates,
    std::vector<int>& colors, int& maxcolor
) {
    std::vector<bool> used(neighbors.size(), false);
    for (int i = static_cast<int>(candidates.size()) - 1; i >= 0; --i) {
        FindColor(candidates[i], used, colors, maxcolor);
    }
}

void MaxCliqueBnB::FindColor(
    int vertex,
    std::vector<bool>& used, std::vector<int>& colors, int& maxcolor
) {
    for (auto neighbor: neighbors[vertex])
        used[colors[neighbor]] = true;

    int color;
    for (color = 1; color < used.size(); color++)
        if (used[color] == false)
            break;

    used.assign(used.size(), false);

    colors[vertex] = color;
    if (color > maxcolor)
        maxcolor = color;
}