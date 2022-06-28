#include <mpi.h>
#include <iostream>
#include <vector>
#include <bits/stdc++.h>

using namespace std;

int main(int argc, char *argv[])
{

    int rank, no_process;

    // *** MPI INIT *** //
    MPI_Init(&argc, &argv);

    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &no_process);

    int graph[][3] = {
        {0, 1, -1},
        {0, 2, 4},
        {1, 2, 3},
        {1, 3, 2},
        {1, 4, 2},
        {3, 2, 5},
        {3, 1, 1},
        {4, 3, -3}};

    int V = 5, E = 8;
    int dist[V];
    int pedges[(E / no_process) * 3];
    int d1Graph[E * 3];

    if (rank == 0) // Master
    {
        // Init Distance Array with infinity
        for (int i = 0; i < V; i++)
            dist[i] = INT_MAX;

        dist[0] = 0; // 0 is the Source

        // Coverting Graph to 1D
        int k = 0;
        for (int i = 0; i < E; ++i)
        {
            for (int j = 0; j < 3; ++j)
            {
                d1Graph[k++] = graph[i][j];
            }
        }

        // Round 1 Relaxation done by master. 
        for (int i = 0; i < E; ++i)
        {
            if (dist[graph[i][1]] > dist[graph[i][0]] + graph[i][2] && dist[graph[i][0]] != INT_MAX)
                dist[graph[i][1]] = dist[graph[i][0]] + graph[i][2];
        }
    }

    MPI_Bcast(dist, V, MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Scatter(d1Graph, (E / no_process) * 3, MPI_INT, pedges, (E / no_process) * 3, MPI_INT, 0, MPI_COMM_WORLD);

    int k = 0;
    int localgraph[E / no_process][((E / no_process) * 3) / (E / no_process)];

    // Creating Local Graphs for processes to simplify the procedure.
    for (int i = 0; i < (E / no_process); ++i)
    {
        for (int j = 0; j < ((E / no_process) * 3) / (E / no_process); ++j)
            localgraph[i][j] = pedges[k++];
    }

    // Relaxation
    for (int outer = 0; outer < V - 1; ++outer)
    {
        for (int i = 0; i < (E / no_process); ++i)
        {
            for (int j = 0; j < E; ++j)
            {
                if (dist[localgraph[i][1]] > dist[localgraph[i][0]] + localgraph[i][2] && dist[localgraph[i][0]] != INT_MAX)
                    dist[localgraph[i][1]] = dist[localgraph[i][0]] + graph[i][2];
            }
        }
    }

    if (rank == 0)
    {
        std::cout << "\nVertex Distance from Source\n";
        for (int i = 0; i < V; ++i)
            std::cout << dist[i] << " ";
    }

    MPI_Finalize();
    // *** MPI END ***//
}
