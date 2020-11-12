#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>
#include <map>

void getGraph(std::map<std::string, std::map<std::string, size_t>> &graph, std::vector<std::string> &cities, std::ifstream &file) {
    std::string str;
    std::string source;
    std::string target;
    size_t distance;
    std::map<std::string, int> cities_map;
    while(std::getline(file, str)) {
        std::stringstream ss(str);
        ss >> source;
        ss >> str;
        ss >> target;
        ss >> str;
        ss >> distance;
        graph[source][target] = distance;
        graph[target][source] = distance;
        cities_map[source] = 0;
        cities_map[target] = 0;
    }
    for(auto &entry : cities_map) {
        cities.push_back(entry.first);
    }
}

bool isThereUnvisited(std::vector<bool> &visited) {
    bool ret = false;
    for(auto x : visited) {
        ret |= !x;
    }
    return ret;
}

size_t findPath(size_t prev, std::map<std::string, std::map<std::string, size_t>> &graph,
                    std::vector<bool> &visited, std::vector<std::string> &cities,
                    std::function<bool(size_t, size_t)> cmpfunc) {
    if(!isThereUnvisited(visited))
        return 0;
    size_t path = -1;
    for(size_t i = 0; i < cities.size(); i++) {
        if(!visited[i] && graph.find(cities[prev]) != graph.end() && graph[cities[prev]].find(cities[i]) != graph[cities[prev]].end()) {
            visited[i] = true;
            auto tmp = findPath(i, graph, visited, cities, cmpfunc);
            if(tmp != (size_t)-1) {
                tmp = graph[cities[prev]][cities[i]] + tmp;
                if(cmpfunc(tmp, path) || path == (size_t)-1)
                    path = tmp;
            }
            visited[i] = false;
        }
    }
    return path;
}

size_t findPath(std::map<std::string, std::map<std::string, size_t>> &graph, std::vector<std::string> &cities, std::function<bool(size_t, size_t)> cmpfunc) {
    std::vector<bool> visited;
    visited.resize(cities.size(), false);
    size_t path = -1;
    for(size_t i = 0; i < cities.size(); i++) {
        visited[i] = true;
        auto tmp = findPath(i, graph, visited, cities, cmpfunc);
        if(cmpfunc(tmp, path) || path == (size_t)-1)
            path = tmp;
        visited[i] = false;
    }
    return path;
}

bool lowerThan(size_t a, size_t b) {
    return a < b;
}

bool higherThan(size_t a, size_t b) {
    return a > b;
}

int main() {
    std::ifstream input_file( "input" );
    std::map<std::string, std::map<std::string, size_t>> graph;
    std::vector<std::string> cities;
    getGraph(graph, cities, input_file);
    std::cout << "Shortest path is \033[93;1m" << findPath(graph, cities, lowerThan) << "\033[0m"
              << std::endl;
    std::cout << "Longest path is \033[93;1m" << findPath(graph, cities, higherThan) << "\033[0m"
              << std::endl;
}
