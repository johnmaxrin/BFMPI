#include <iostream>
#include <bits/stdc++.h>

using namespace std;

int bellford(int V, int E, int graph[][3],int src);

int main()
{
    int graph[][3] = {{0, 1, -1}, {0, 2, 4}, {1, 2, 3}, {1, 3, 2}, {1, 4, 2}, {3, 2, 5}, {3, 1, 1}, {4, 3, -3}};

    int V = 5, E = 8;

    bellford(V, E, graph, 0);

    return 0;
}

int bellford(int V, int E, int graph[][3], int src)
{
    int dist[V];
    // int parent[V];

    // Init Distance Array
    for (int i = 0; i < V; i++)
        dist[i] = INT_MAX;

    //parent[src] = -1;
    dist[src] = 0;

    for (int i = 0; i < V; ++i)
    {
        for (int j = 0; j < E; ++j)
        {
            if (dist[graph[j][1]] > dist[graph[j][0]] + graph[j][2] && dist[graph[j][0]] != INT_MAX)
                dist[graph[j][1]] = dist[graph[j][0]] + graph[j][2];
        }
    }

    cout << "Vertex Distance from Source" << endl;
    for (int i = 0; i < V; i++)
        cout << i << "\t\t" << dist[i] << endl;


return 0;
}