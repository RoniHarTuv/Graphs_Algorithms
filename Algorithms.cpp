// Id: 207199282
// Mail: Roniharpaz1@gmail.com

#include "Algorithms.hpp"
#include <queue>
#include <iostream>
#include <vector>
#include <string>
#include <limits>
#include <sstream>
#include <algorithm>
#include <climits>
using namespace std;
using namespace ariel;

/*
This class presents 5 algorithms on graphs:
1. Is the graph connected (that is, is it possible to reach from every vertex to every vertex in the graph.)
2. Finding the shortest path from a start point to any end point on the graph.
3. Is there a circle in the graph.
4. Is the graph a biapartite graph.
5. Is there a negative circle in the graph

In addition, there are several auxiliary functions that appear under the department -
these functions are algorithms that operate on graphs as learned in the "Algorithms 1" course.
*/

/**
 * Determines if the graph is connected. For undirected graphs, it checks if there is a path
 * between every pair of vertices. For directed graphs, it checks if the graph is strongly connected,
 * meaning there is a directed path from any vertex to every other vertex.
 * @param graph The Graph object
 * @return true if the graph is connected (or strongly connected if directed); false otherwise.
 */
bool Algorithms::isConnected(const Graph &graph)
{
    bool is_directed = graph.getIsDirected();
    size_t size = graph.getAdjacencyMatrix().size();
    vector<vector<int>> matrix = graph.getAdjacencyMatrix();
    if (size == 0)
    {
        return true; // An empty graph is trivially connected
    }
    vector<bool> visited(size, false);// Initialize visited array

    // Run BFS from the first vertex for undirected graph
    if (!is_directed)
    {
        if (!bfs(graph, 0, visited))
        { // Start BFS from vertex 0
            return false;
        }
        // Check if all vertices were visited
        for (bool v : visited)
        {
            if (!v)
            {
                return false; // If any vertex was not visited, graph is not connected
            }
        }
        return true;
    }
    if (is_directed)
    {
        for (size_t i = 0; i < size; i++)
        {
            vector<bool> visited(size, false);
            bfs(graph, i, visited);
            for (bool v : visited)
            {
                if (!v)
                {
                    return false;
                }
            }
        }
        return true;
    }
    return false;
}
/**
 * Finds the shortest path between two vertices in a graph using appropriate algorithms
 * based on the graph's properties.The graph can be either directed or undirected and may contain
 * negative weight edges. It handles different scenarios using Dijkstra's or Bellman-Ford's
 * algorithm accordingly.
 * @param graph The graph 
 * @param start The index of the start vertex in the graph.
 * @param end The index of the end vertex in the graph.
 * @return A string that either describes the shortest path or reports an error (e.g., invalid vertices, negative cycles).
 */
string Algorithms::shortestPath(const Graph &graph, size_t start, size_t end)
{
    // Check for out-of-bound indices for start and end vertices
    if (start >= graph.getAdjacencyMatrix().size() || end >= graph.getAdjacencyMatrix().size())
    {
        throw invalid_argument("Invalid end / start vertex- not in the graph");
    }

    // Determine path finding strategy based on graph's directed/undirected nature
    if (graph.getHasNegativeVertices())
    {
        return bellmanFord(graph.getAdjacencyMatrix(), start, end);// Use Bellman-Ford for graphs with negative weight edges
    }
    else
    {
        return dijkstra(graph.getAdjacencyMatrix(), start, end);// Use Dijkstra's algorithm for non-negative edge weight graphs
    }
}

/**
 * Determines if the graph contains any cycles and provides a description of the cycle if it exists.
 * For directed graphs, it uses depth-first search (DFS) with a recursion stack to detect back edges which
 * indicate cycles.
 * For undirected graphs, it uses a modified DFS that also keeps track of the parent node
 * to differentiate between back edges and undirected edges.
 * @param graph The graph 
 * @return A string that either describes the cycle with the vertices involved
 */
static const size_t NO_PARENT = numeric_limits<size_t>::max(); // Use the maximum value of size_t as a sentinel value for no parent
string Algorithms::isContainsCycle(const Graph &graph)
{
    const size_t V = graph.getAdjacencyMatrix().size();
    vector<bool> visited(V, false);
    vector<int> path;

    // Different handling based on whether the graph is directed or not
    if (graph.getIsDirected())
    {
        vector<bool> recStack(V, false);
        // Iterate through each vertex that has not been visited
        for (size_t i = 0; i < V; ++i)
        {
            if (!visited[i])
            {
                // Perform DFS for directed graphs
                if (dfsDirected(graph, i, visited, recStack, path))
                {
                    // Construct the result string with cycle path
                    stringstream ss;
                    ss << "Is contain cycle: Yes: ";
                    for (size_t j = 0; j < path.size(); ++j)
                    {
                        ss << path[j];
                        if (j < path.size() - 1)
                            ss << "->";
                    }
                    // Close the cycle in the description if needed
                    if (!path.empty() && path.back() != path.front())
                    {
                        ss << "->" << path.front(); // Append the start to close the cycle only if needed
                    }
                    return ss.str();
                }
            }
        }
    }
    else
    {
        // Iterate through each vertex for undirected graphs
        for (size_t i = 0; i < V; ++i)
        {
            if (!visited[i])
            {
                // Perform DFS for undirected graphs, using NO_PARENT as initial parent
                if (dfsUndirected(graph, i, NO_PARENT, visited, path))
                {
                    // Construct the result string with cycle path
                    stringstream ss;
                    ss << "Is contain cycle: Yes: ";
                    for (size_t j = 0; j < path.size(); ++j)
                    {
                        ss << path[j];
                        if (j < path.size() - 1)
                            ss << "->";
                    }
                    // Close the cycle in the description if needed
                    if (!path.empty() && path.back() != path.front())
                    {
                        ss << "->" << path.front();
                    }
                    return ss.str();
                }
            }
        }
    }
    return "Is contain cycle: No"; // Return a negative result if no cycles are found
}
/**
 * Determines if the graph is bipartite and returns a formatted string indicating
 * @param graph The Graph object containing the adjacency matrix used for checking.
 * @return A string indicating whether the graph is bipartite and listing the vertices
 *         in each of the two sets if it is.
 */
string Algorithms::isBipartite(const Graph &graph)
{
    // Retrieve the number of vertices from the graph's adjacency matrix
    size_t n = graph.getAdjacencyMatrix().size();
    // Handle the case of an empty graph
    if (n == 0)
        return "Is bipartite: Yes, {}{}"; // An empty graph is trivially bipartite

    vector<int> color(n, -1); // Initialize color assignments (-1 indicates uncolored)

    // Attempt to color the graph using two colors starting from each unvisited vertex
    for (size_t i = 0; i < n; ++i)
    {
        if (color[i] == -1) // Check if the vertex is unvisited
        {
            if (!colorCheck(graph, color, i)) // Use BFS from this vertex to check bipartiteness
            {
                return "Is bipartite: No"; // Return immediately if a conflict is found
            }
        }
    }
    // Construct two sets based on the coloring result
    vector<size_t> setA, setB;
    for (size_t i = 0; i < n; ++i)
    {
        if (color[i] == 0)
        {
            setA.push_back(i); // Add to set A if colored 0
        }
        else if (color[i] == 1)
        {
            setB.push_back(i); // Add to set B if colored 1
        }
    }

    string result = "Is bipartite: Yes, {";
    for (size_t i = 0; i < setA.size(); ++i)
    {
        result += to_string(setA[i]);
        if (i < setA.size() - 1)
            result += ","; // Separate vertices with commas
    }
    result += "}{";
    for (size_t i = 0; i < setB.size(); ++i)
    {
        result += to_string(setB[i]);
        if (i < setB.size() - 1)
            result += ",";
    }
    result += "}";
    return result; 
}

/**
 * Checks for the presence of a negative weight cycle in the graph.
 * This function utilizes the Bellman-Ford algorithm for each vertex in the graph
 * to detect negative cycles. If a negative cycle is found, it formats the cycle
 * using the makePath function and returns a descriptive string.
 * @param graph 
 * @return A string indicating whether the graph contains a negative cycle.
 */
string Algorithms::negativeCycle(const Graph &graph)
{
    if (!graph.getHasNegativeVertices())
    {
        return "Is contain negative cycle: No";
    }
    if (isContainsCycle(graph) == "Is contain cycle: No")
    {
        return "Is contain negative cycle: No";
    }
    else
    {
        string s = "";
        size_t size = graph.getAdjacencyMatrix().size();
        for (size_t i = 0; i < size; ++i)
        {
            vector<int> parents = bellmanFord2(graph, i, s);
            if (parents.empty())
            {
                return "Is contain negative cycle: Yes: " + makePath(s);
            }
        }
        return "Is contain negative cycle: No";
    }
}
/**
 * Reverses a given string and formats it by inserting '->' between characters.
 */
string Algorithms::makePath(string s)
{
    reverse(s.begin(), s.end());
    string result;
    if (!s.empty())
    {
        for (size_t i = 0; i < s.length() - 1; ++i)
        {                   // Notice loop goes until second to last character
            result += s[i]; // Append current character
            result += "->"; // Append arrow after each character except the last
        }
        result += s[s.length() - 1]; // Append the last character without an arrow
    }
    return result;
}

/**
 * Performs a Breadth-First Search (BFS) to determine if all vertices in the graph are reachable
 * from a given start vertex. This function is used to assess the connectivity of the graph.
 *
 * The BFS algorithm starts at the 'startVertex' and explores all its neighbors at the present depth
 * prior to moving on to nodes at the next depth level. It uses a queue to keep track of vertices to be explored
 * and a vector 'visited' to keep track of which vertices have been visited.
 *
 * This implementation not only checks for reachability but also counts the number of vertices visited
 * during the traversal to determine if the graph is connected. If the count of visited vertices equals
 * the total number of vertices in the graph, then all vertices are reachable from the starting vertex,
 * indicating that the graph is connected.
 *
 * @param graph The graph 
 * @param startVertex The starting vertex for BFS.
 * @param visited A reference to a vector of boolean values indicating whether each vertex has been visited.
 * @return True if all vertices in the graph are reachable from 'startVertex', otherwise False.
 */
bool Algorithms::bfs(const Graph &graph, size_t startVertex, vector<bool> &visited)
{
    const vector<vector<int>> &adjMatrix = graph.getAdjacencyMatrix();
    queue<size_t> queue;
    visited[startVertex] = true;
    queue.push(startVertex);
    size_t visitCount = 1; // Starts with the start vertex already visited

    while (!queue.empty())
    {
        size_t current = queue.front();
        queue.pop();

        for (size_t i = 0; i < adjMatrix[current].size(); ++i)
        {
            if (adjMatrix[current][i] != 0 && !visited[i])
            {
                visited[i] = true;
                queue.push(i);
                visitCount++; // Increment visit count for each newly visited vertex
            }
        }
    }
    return visitCount == adjMatrix.size();// Check if all vertices were visited
}
/**
 * BELLMAN-FORD
 * #this function was taken from "Geeks For Geeks".
 */
string Algorithms::bellmanFord(const vector<vector<int>> &graph, size_t start, size_t end)
{
    size_t V = graph.size();
    vector<int> distance(V, numeric_limits<int>::max());
    vector<int> predecessor(V, -1); // To reconstruct the path

    distance[start] = 0;

    // Relax all edges |V| - 1 times
    for (size_t i = 1; i < V; ++i)
    {
        for (size_t u = 0; u < V; ++u)
        {
            for (size_t v = 0; v < V; ++v)
            {
                if (graph[u][v] != 0 && distance[u] != numeric_limits<int>::max() &&
                    distance[u] + graph[u][v] < distance[v])
                {
                    distance[v] = distance[u] + graph[u][v];
                    predecessor[v] = u;
                }
            }
        }
    }
    // Check for negative-weight cycles
    for (size_t u = 0; u < V; ++u)
    {
        for (size_t v = 0; v < V; ++v)
        {
            if (graph[u][v] != 0 && distance[u] != numeric_limits<int>::max() &&
                distance[u] + graph[u][v] < distance[v])
            {
                return "No Shortest Path- Graph contains a negative cycle";
            }
        }
    }
    return reconstructPath(predecessor, start, end);
}
/**
 * DIJKSTRA Algorithm
 * #this function was taken from "Geeks For Geeks".
 */
string Algorithms::dijkstra(const vector<vector<int>> &graph, size_t start, size_t end)
{
    size_t V = graph.size();
    vector<int> distance(V, numeric_limits<int>::max());
    vector<bool> visited(V, false);
    vector<int> predecessor(V, -1); // To reconstruct the path

    distance[start] = 0;
    priority_queue<pair<int, size_t>, vector<pair<int, size_t>>, greater<pair<int, size_t>>> pq;
    pq.push({0, start});

    while (!pq.empty())
    {
        size_t u = pq.top().second;
        pq.pop();

        if (visited[u])
            continue;
        visited[u] = true;

        for (size_t v = 0; v < V; ++v)
        {
            if (graph[u][v] != 0 && !visited[v])
            {
                int newDist = distance[u] + graph[u][v];
                if (newDist < distance[v])
                {
                    distance[v] = newDist;
                    pq.push({newDist, v});
                    predecessor[v] = u;
                }
            }
        }
    }
    return reconstructPath(predecessor, start, end);
}
/**
 * Reconstructs the path from a start vertex to an end vertex based on information stored in a predecessor array.
 * This helper function is typically used after running a shortest path algorithm which fills the predecessor array
 * with information about the path of each vertex back to the source vertex.
 *
 * The function traces the path from the end vertex back to the start vertex using the 'predecessor' array where each
 * index represents a vertex and the value at each index represents the predecessor of that vertex in the path.
 * It constructs a path by backtracking from the 'end' vertex to the 'start' vertex using this array.
 *
 * If no path exists (indicated by a '-1' in the predecessor array for the 'end' vertex), it returns an appropriate
 * message stating that no path is available.
 *
 * @param predecessor A vector containing the index of the preceding vertex in the path for each vertex in the graph.
 *                    The 'predecessor' vector is filled by the shortest path algorithm.
 * @param start The starting vertex of the path.
 * @param end The ending vertex of the path to which the path needs to be reconstructed.
 * @return A string representing the path from 'start' to 'end'. If no path exists, it returns a message indicating this.
 */
string Algorithms::reconstructPath(const vector<int> &predecessor, size_t start, size_t end)
{
    // Check if there is no predecessor for the end vertex indicating no path exists.
    if (predecessor[end] == -1)
    {
        return "Shortest Path: No way between " + to_string(start) + " to " + to_string(end);
    }
    vector<int> path;
    size_t at = end;
    // Backtrack from the end to the start using the predecessor array.
    while (at != static_cast<size_t>(-1))
    {
        path.push_back(at);
        if (predecessor[at] == -1)
        {
            break; // Stop if there is no predecessor.
        }
        at = static_cast<size_t>(predecessor[at]); // Convert safely as checked.
    }
    reverse(path.begin(), path.end()); // Reverse the path to start from the starting vertex.

    // Check if the reconstructed path is valid and non-empty.
    if (path.empty() || path.front() != start)
    {
        return "Shortest Path: No Path between " + to_string(start) + " to " + to_string(end);
    }

    // Construct a string that represents the path from start to end.
    stringstream ss;
    ss << "Shortest Path from " << start << " to " << end << " is: ";
    for (size_t i = 0; i < path.size(); i++)
    {
        ss << path[i];
        if (i < path.size() - 1)
            ss << "->";
    }
    return ss.str();
}
/**
 * Performs a Depth-First Search (DFS) to detect cycles in a directed graph.
 * This function checks for cycles by maintaining a recursion stack (recStack) along with
 * a visited list. If a vertex is revisited and is found on the recursion stack, a cycle is detected.
 * @param graph The graph 
 * @param vertex The current vertex being visited by the DFS.
 * @param visited A vector indicating whether each vertex has been visited.
 * @param recStack A vector indicating whether each vertex is in the current recursion stack.
 * @param path A vector storing the current path of vertices visited by the DFS.
 * @return True if a cycle is detected, otherwise False.
 * #this function was taken from "Geeks For Geeks".
 */
bool Algorithms::dfsDirected(const Graph &graph, size_t vertex, vector<bool> &visited, vector<bool> &recStack, vector<int> &path)
{
    if (!visited[vertex])
    {
        visited[vertex] = true;
        recStack[vertex] = true;
        path.push_back(vertex);

        const vector<vector<int>> &adjMatrix = graph.getAdjacencyMatrix();
        for (size_t i = 0; i < adjMatrix[vertex].size(); ++i)
        {
            if (adjMatrix[vertex][i] != 0)
            {
                if (!visited[i] && dfsDirected(graph, i, visited, recStack, path))
                {
                    return true;
                }
                else if (recStack[i])
                {
                    path.push_back(i);
                    return true;
                }
            }
        }
    }
    recStack[vertex] = false; // Remove the vertex from recursion stack
    path.pop_back();
    return false;
}
/**
 * Performs a Depth-First Search (DFS) to detect cycles in an undirected graph.
 * This function uses a slightly modified version of DFS tailored for undirected graphs,
 * where it must ignore the edge coming from the parent node to prevent false cycle detection.
 * @param graph The graph in which to detect cycles, represented as an adjacency matrix.
 * @param vertex The current vertex being visited by the DFS.
 * @param parent The parent vertex of the current vertex in the DFS tree, used to prevent
 *               considering the immediate back edge as a cycle.
 * @param visited A vector indicating whether each vertex has been visited.
 * @param path A vector storing the current path of vertices visited by the DFS.
 * @return True if a cycle is detected, otherwise False.
 * #this function was taken from "Geeks For Geeks".
 */
bool Algorithms::dfsUndirected(const Graph &graph, size_t vertex, size_t parent, vector<bool> &visited, vector<int> &path)
{
    visited[vertex] = true;
    path.push_back(vertex);

    const vector<vector<int>> &adjMatrix = graph.getAdjacencyMatrix();
    for (size_t i = 0; i < adjMatrix[vertex].size(); ++i)
    {
        if (adjMatrix[vertex][i] != 0)
        {
            if (!visited[i])
            {
                if (dfsUndirected(graph, i, vertex, visited, path))
                {
                    return true;
                }
            }
            else if (i != parent)
            { // Check if the node is not the parent
                path.push_back(i);
                return true;
            }
        }
    }
    path.pop_back();
    return false;
}

/**
 * Checks if a graph can be colored using two colors such that no two adjacent vertices
 * have the same color. This method is used to determine if the graph is bipartite.
 * @param graph The graph object
 * @param color A vector where the color of each vertex will be stored. Initially,
 *              all elements should be set to -1, indicating that no vertex has been colored.
 * @param src The starting vertex for the BFS-based coloring process.
 * @return true if the graph is bipartite (can be colored with two colors without conflicts),
 * #this function was taken from Algorithms 1 course(tirgul 4)
 */
bool Algorithms::colorCheck(const Graph &graph, vector<int> &color, size_t src)
{
    queue<size_t> q;
    q.push(src);
    color[src] = 0; // Start coloring the source vertex with color 0.

    const auto &adjacencyMatrix = graph.getAdjacencyMatrix();
    size_t n = adjacencyMatrix.size(); // Number of vertices

    while (!q.empty())
    {
        size_t u = q.front();
        q.pop();

        // Explore all adjacent vertices
        for (size_t v = 0; v < n; ++v)
        {
            if (adjacencyMatrix[u][v] != 0)
            { // There is an edge from u to v
                if (color[v] == -1)
                {                            // If vertex v is uncolored
                    color[v] = 1 - color[u]; // Color it with the opposite color of u
                    q.push(v);
                }
                else if (color[v] == color[u])
                {                 // If the adjacent vertex has the same color
                    return false; // The graph is not bipartite
                }
            }
        }
    }
    return true; // The graph can be colored with two colors, hence it's bipartite
}
/**
 * Implements the Bellman-Ford algorithm to detect negative weight cycles from a given start vertex.
 * The function attempts to relax the edges repeatedly and checks for any decrease in shortest path
 * estimate that indicates a negative cycle. If a negative cycle is detected, the function constructs
 * the cycle path which is then formatted using the makePath function.
 *
 * @param graph The Graph object
 * @param start Representing the starting vertex index for the Bellman-Ford algorithm.
 * @param cycle A reference to a string which will be used to store the cycle path if a negative cycle is detected.
 * @return A vector of integers representing the shortest path tree (or parent vector). If a negative cycle is found,
 *         returns an empty vector to indicate the detection.
 * @throws std::out_of_range if the starting vertex index is out of the bounds of the graph's adjacency matrix.
 */
vector<int> Algorithms::bellmanFord2(const Graph &graph, int start, string &cycle)
{
    const vector<vector<int>> &adjMatrix = graph.getAdjacencyMatrix();
    size_t size = adjMatrix[0].size();
    vector<int> distances(size, numeric_limits<int>::max());
    vector<int> parents(size, -1);
    // Ensure the start index is within bounds and convert it to size_t for safety
    if (start < 0 || static_cast<size_t>(start) >= size)
    {
        throw out_of_range("Start vertex is out of bounds.");
    }
    size_t start_index = static_cast<size_t>(start);
    distances[start_index] = 0;

    // Relax edges repeatedly
    for (size_t i = 0; i < size - 1; ++i)
    {
        for (size_t u = 0; u < size; ++u)
        {
            for (size_t v = 0; v < size; ++v)
            {
                if (adjMatrix[u][v] != 0 && distances[u] != numeric_limits<int>::max() && distances[u] + adjMatrix[u][v] < distances[v])
                {
                    if ((graph.getIsDirected()) || (parents[u] != v))
                    {
                        distances[v] = distances[u] + adjMatrix[u][v];
                        parents[v] = u;
                    }
                }
            }
        }
    }
    // Check for negative weight cycle
    for (size_t u = 0; u < size; ++u)
    {
        for (size_t v = 0; v < size; ++v)
        {
            if (adjMatrix[u][v] != 0 && distances[u] != numeric_limits<int>::max() && distances[u] + adjMatrix[u][v] < distances[v])
            {
                // cout << "negative cycle strat from vertex " << u << endl;
                size_t temp = u;
                // Detect cycle
                while (parents[u] != temp)
                {
                    cycle += to_string(u); //+ "->";
                    u = (size_t)parents[u];
                }
                cycle += to_string(u);
                cycle += to_string(parents[u]);
                return {}; // Return empty vector indicating a negative cycle
            }
        }
    }
    return parents; // Return parent vector if no negative cycles
}
