// Id: 207199282
// Mail: Roniharpaz1@gmail.com

#include "Graph.hpp"
#include <iostream>
#include <stdexcept>
using namespace std;
using namespace ariel;

/**
 * this class represent a Graph.
 */
Graph::Graph() 
{
    // Initialize adjacencyMatrix as an empty vector
    adjacencyMatrix = {};
    is_directed = false;
    HasNegativeVertices = false;
}
/**
 * This function get a matrix, and check if the matrix is leagel.
 * If so, the graph will be this matrix.
 * Then, it checks whether the graph is directed or not.
 * The graph is considered directed if there exists at least one pair (i, j)
 * such that the weight from i to j (i.e., matrix[i][j]) is not equal to the weight from j to i (i.e., matrix[j][i]).
 * This loop checks all pairs and sets isDirected to true if any such pair is found.
 * After, it checks if there is some negative edges.
 * A Graph could be not weighted if all edges is 1.
 */

void Graph::loadGraph(const vector<vector<int>> &matrix)
{
    // Check if the graph is a square matrix
    size_t rows = matrix.size();
    for (size_t i = 0; i < rows; ++i)
    {
        if (matrix[i].size() != rows)
        {
            throw invalid_argument("Invalid graph: The graph is not a square matrix.");
            return;
        }
    }
    for (size_t i = 0; i < rows; ++i)
    {
        for (size_t j = 0; j < rows; ++j)
        {
            if (i == j)
            {
                if (matrix[i][j] != 0)
                {
                    throw invalid_argument("Invalid graph: not 0 in the diagonal.");
                    return;
                }
            }
        }
    }
    adjacencyMatrix = matrix;
    // check if the graph is directed\not directed
    //  The graph will be a directed graph when thare is two different weight between 2 vertices. else- the graph not directed.
    is_directed = false; // Start by assuming the graph is not directed
    for (size_t i = 0; i < rows; ++i)
    {
        for (size_t j = 0; j < rows; ++j)
        {
            if (matrix[i][j] != matrix[j][i])
            {
                is_directed = true;
                break;
            }
        }
        if (is_directed)
        {
            break;
        }
    }
    HasNegativeVertices = false;
    for (size_t i = 0; i < rows; ++i)
    {
        for (size_t j = 0; j < rows; ++j)
        {
            if (matrix[i][j] < 0)
            {
                HasNegativeVertices = true;
            }
        }
    }
}

/**
 * This class print a graph and a information about it- is it directed or not,
 * how many vertices and edges, is this graph have a negative edge and also print the matrix.
 */

void Graph::printGraph() const
{
    if (is_directed == false) // isDirected== false
    {
        cout << "A Not Directed Graph" << endl;
        if (HasNegativeVertices == true)
        {
            cout << "This graph has a negative edges." << endl;
        }
        if (HasNegativeVertices == false)
        {
            cout << "This graph without negative edges." << endl;
        }
        cout << "With " << adjacencyMatrix.size() << " vertices and "; // num of vertices is like the matrix size
        int edges = 0;
        for (size_t i = 0; i < adjacencyMatrix.size(); ++i)
        {
            for (size_t j = i + 1; j < adjacencyMatrix[i].size(); ++j)
            {
                if (adjacencyMatrix[i][j] != 0)
                {
                    ++edges;
                }
            }
        }
        cout << edges << " edges." << endl;
        // Print adjacency matrix
        for (size_t i = 0; i < adjacencyMatrix.size(); ++i)
        {
            for (size_t j = 0; j < adjacencyMatrix[i].size(); ++j)
            {
                cout << adjacencyMatrix[i][j] << " ";
            }
            cout << endl;
        }
    }
    else // isDirected == true
    {
        cout << "A Directed Graph" << endl;
        if (HasNegativeVertices == true)
        {
            cout << "This graph has a negative edges." << endl;
        }
        if (HasNegativeVertices == false)
        {
            cout << "This graph without negative edges." << endl;
        }
        cout << "With " << adjacencyMatrix.size() << " vertices and "; // num of vertices is like the matrix size
        int edges = 0;
        for (size_t i = 0; i < adjacencyMatrix.size(); ++i)
        {
            for (size_t j = 0; j < adjacencyMatrix[i].size(); ++j)
            {
                if (adjacencyMatrix[i][j] != 0)
                {
                    ++edges;
                }
            }
        }
        cout << edges << " edges:" << endl;
        // Print adjacency matrix
        for (size_t i = 0; i < adjacencyMatrix.size(); ++i)
        {
            for (size_t j = 0; j < adjacencyMatrix[i].size(); ++j)
            {
                cout << adjacencyMatrix[i][j] << " ";
            }
            cout << endl;
        }
    }
}
/**
 * This function return the Adjacency Matrix of the graph
 */
const vector<vector<int>> &Graph::getAdjacencyMatrix() const
{
    return adjacencyMatrix;
}

/**
 * This function return is this graph directed
 */
const bool Graph::getIsDirected() const
{
    return is_directed;
}

/**
 * This function return is this graph has negative edges
 */
bool Graph::getHasNegativeVertices() const
{
    return HasNegativeVertices;
}