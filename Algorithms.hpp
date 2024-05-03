// Id: 207199282
// Mail: Roniharpaz1@gmail.com

#ifndef ALGORITHMS_HPP
#define ALGORITHMS_HPP
#include "Graph.hpp"
#include <vector>
#include <string>
#include <iostream>
#include <optional>
#include <sstream>

namespace ariel {

    class Algorithms {
    public:
        // Main function of this assignment:
        static bool isConnected(const Graph& graph);
        
        static std::string shortestPath(const Graph& graph, size_t start, size_t end);
        
        static std::string isContainsCycle(const Graph& graph);
        
        static std::string isBipartite(const Graph& graph);
        
        static std::string negativeCycle(const Graph& graph);

        //Helper function- Algorithms on graphs:
        static bool bfs(const Graph& graph, size_t startVertex, std::vector<bool>& visited);
        
        static std::string bellmanFord(const std::vector<std::vector<int>>& graph, size_t start, size_t end);
        
        static std::string dijkstra(const std::vector<std::vector<int>>& graph, size_t start, size_t end);
        
        static std::string reconstructPath(const std::vector<int>& predecessor, size_t start, size_t end);
        
        static bool dfsUndirected(const Graph &graph, size_t vertex, size_t parent, std::vector<bool> &visited, std::vector<int> &path);
        
        static bool dfsDirected(const Graph &graph, size_t vertex, std::vector<bool> &visited, std::vector<bool> &recStack, std::vector<int> &path);

        static bool colorCheck(const Graph& graph, std::vector<int>& color, size_t src);

        static std::vector<int> bellmanFord2(const Graph &graph, int start, std::string &cycle);
        
        static std::string makePath(std::string s);

    };

}
#endif // ALGORITHMS_HPP
