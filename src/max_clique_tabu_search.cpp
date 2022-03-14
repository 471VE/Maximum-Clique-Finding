#include "max_clique_tabu_search.h"

#include <iostream>
#include <random>
#include <fstream>
#include <sstream>


int MaxCliqueTabuSearch::GetRandom(int a, int b) {
    static std::mt19937 generator;
    std::uniform_int_distribution<int> uniform(a, b);
    return uniform(generator);
}


void MaxCliqueTabuSearch::ReadGraphFile(std::string filename) {
    std::ifstream fin(filename);
    std::string line;
    int vertices = 0, edges = 0;
    while (getline(fin, line)) {
        if (line[0] == 'c')
            continue;

        std::stringstream line_input(line);
        char command;
        if (line[0] == 'p') {
            std::string type;
            line_input >> command >> type >> vertices >> edges;
            neighbor_sets.resize(vertices);
            qco.resize(vertices);
            index.resize(vertices, -1);
            tightness.resize(vertices, 0);
            non_neighbors.resize(vertices);
            adjacency_matrix.resize(vertices, std::vector<int>(vertices, 0));
            degrees.resize(vertices);
        } else {
            int start, finish;
            line_input >> command >> start >> finish;
            neighbor_sets[start - 1].insert(finish - 1);
            neighbor_sets[finish - 1].insert(start - 1);
            adjacency_matrix[start - 1][finish - 1]++;
            adjacency_matrix[finish - 1][start - 1]++;
        }
    }
    for (int i = 0; i < vertices; ++i)
        for (int j = 0; j < vertices; ++j)
            if (neighbor_sets[i].count(j) == 0 && i != j)
                non_neighbors[i].insert(j);
    for (int i = 0; i < neighbor_sets.size(); ++i) {
        degrees[i] = static_cast<int>(neighbor_sets[i].size());
    }
}


void MaxCliqueTabuSearch::RunSearch(int starts, int randomization) {
    for (int iter = 0; iter < starts; ++iter) {
        ClearClique();
        for (int i = 0; i < neighbor_sets.size(); ++i) {
            qco[i] = i;
            index[i] = i;
        }

        RunInitialHeuristic(randomization);

        for (int i = 0; i < neighbor_sets.size(); ++i) {
            tightness[i] = ComputeTightness(i);
        }

        c_border = q_border;
        int swaps = 0;
        while (swaps < 100) {
            if (!Move()) {
                if (!Swap1To1()) {
                    if (!Swap1to2()) {
                        break;
                    }
                    else {
                        ++swaps;
                    }
                } else {
                    ++swaps;
                }
            }
        }
        if (q_border > best_clique.size()) {
            best_clique.clear();
            for (int i = 0; i < q_border; ++i)
                best_clique.insert(qco[i]);
        }
    }
}


bool MaxCliqueTabuSearch::Check() {
    for (int i : best_clique)
        for (int j : best_clique)
            if (i != j && neighbor_sets[i].count(j) == 0) {
                std::cout << "Returned subgraph is not clique\n";
                return false;
            }
    return true;
}


void MaxCliqueTabuSearch::ClearClique() {
    q_border = 0;
    c_border = 0;
}


std::string MaxCliqueTabuSearch::PrintClique() {
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


int MaxCliqueTabuSearch::ComputeTightness(int vertex) {
    int tightness = 0;
    for (int i = 0; i < q_border; ++i)
        if (qco[i] != vertex)
            if (neighbor_sets[qco[i]].count(vertex) == 0)
                ++tightness;
    return tightness;
}


void MaxCliqueTabuSearch::SwapVertices(int vertex, int border) {
    int vertex_at_border = qco[border];
    std::swap(qco[index[vertex]], qco[border]);
    std::swap(index[vertex], index[vertex_at_border]);
}


void MaxCliqueTabuSearch::InsertToClique(int i) {
    SwapVertices(i, q_border);
    q_border++;

    for (int j : non_neighbors[i]) {
        if (tightness[j] == 0) {
            --c_border;
            SwapVertices(j, c_border);
        }
        ++tightness[j];
    }
}


void MaxCliqueTabuSearch::RemoveFromClique(int k) {    
    --q_border;
    SwapVertices(k, q_border);

    for (int j : non_neighbors[k]) {
        if (tightness[j] == 1) {
            SwapVertices(j, c_border);
            c_border++;
        }
        --tightness[j];
    }
}


bool MaxCliqueTabuSearch::VertexInTabuList(const int& vertex, std::deque<int>& tabu_list) {
    std::deque<int>::iterator it = std::find(tabu_list.begin(), tabu_list.end(), vertex);
    return (it != tabu_list.end());
}


void MaxCliqueTabuSearch::AddToTabuList(int vertex, std::deque<int>& tabu_list, const int& tabu_list_max) {
    tabu_list.push_back(vertex);
    if (tabu_list.size() > tabu_list_max)
        tabu_list.pop_front();
}


bool MaxCliqueTabuSearch::Swap1To1() {
    for (int counter = 0; counter < q_border; ++counter) {
        int vertex = qco[counter];
        if (VertexInTabuList(vertex, added_tabu_list))
            continue;

        std::vector<int> L;
        for (int i : non_neighbors[vertex])
            if (tightness[i] == 1)
                if (!VertexInTabuList(i, removed_tabu_list))
                    L.push_back(i);
        if (L.size() == 0)
            continue;

        RemoveFromClique(vertex);
        AddToTabuList(vertex, removed_tabu_list, REMOVED_TABU_LIST_MAX);

        int i = L[GetRandom(0, static_cast<int>(L.size()) - 1)];
        AddToTabuList(i, added_tabu_list, ADDED_TABU_LIST_MAX);
        InsertToClique(i);

        return true;
    }
    return false;
}


bool MaxCliqueTabuSearch::Swap1to2() {
    for (int counter = 0; counter < q_border; ++counter) {
        int vertex = qco[counter];
        std::vector<int> L;
        for (int i : non_neighbors[vertex])
            if (tightness[i] == 1)
                L.push_back(i);
        if (L.size() < 2)
            continue;
        int i, j;
        for (int l1 = 0; l1 < L.size(); l1++) {
            for (int l2 = l1 + 1; l2 < L.size(); l2++) {
                i = L[l1]; j = L[l2];
                if (adjacency_matrix[i][j]) {
                    RemoveFromClique(vertex);
                    InsertToClique(i);
                    InsertToClique(j);
                    return true;
                }
            }
        }
    }
    return false;
}


bool MaxCliqueTabuSearch::Move() {
    if (c_border == q_border)
        return false;
    int vertex = qco[GetRandom(q_border, c_border - 1)];
    InsertToClique(vertex);
    return true;
}


void MaxCliqueTabuSearch::RunInitialHeuristic(int randomization) {
    static std::mt19937 generator;
    std::vector<int> candidates(neighbor_sets.size());
    for (int i = 0; i < neighbor_sets.size(); ++i)
        candidates[i] = i;

    std::shuffle(candidates.begin(), candidates.end(), generator);
    while (! candidates.empty()) {
        int last = static_cast<int>(candidates.size()) - 1;
        int rnd = GetRandom(0, std::min(randomization - 1, last));
        int vertex = candidates[rnd];
        SwapVertices(vertex, q_border);
        ++q_border;
        for (int c = 0; c < candidates.size(); ++c) {
            int candidate = candidates[c];
            if (neighbor_sets[vertex].count(candidate) == 0) {
                std::swap(candidates[c], candidates[candidates.size() - 1]);
                candidates.pop_back();
                --c;
            }
        }
        std::shuffle(candidates.begin(), candidates.end(), generator);
    }
}