// Id: 207199282
// Mail: Roniharpaz1@gmail.com

#ifndef GRAPH_HPP
#define GRAPH_HPP
#include <vector>

namespace ariel {

    class Graph {
    private:
        std::vector<std::vector<int>> adjacencyMatrix; //the matrix that represent the graph
        bool is_directed;                              //is this matrix symetric? if yes- the graph is not directed                        
        bool HasNegativeVertices;                      // is this graph has negative vertices

    public:
        Graph(); // Constructor

        void loadGraph(const std::vector<std::vector<int>>& graph); // Method to load a graph-+ecided is this graph directed\not
        
        void printGraph() const; // Method to print the graph and the settings of the graph

        const bool getIsDirected() const ;

        bool getHasNegativeVertices() const;
        
        const std::vector<std::vector<int>>& getAdjacencyMatrix() const; // Method to get the adjacency matrix
    };

} 

#endif // GRAPH_HPP
