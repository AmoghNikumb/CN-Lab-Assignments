
#include <stdio.h>
#include <string.h>

#define MAX 20
#define INF 100000000

char name[MAX][20];
int cost[MAX][MAX];
int N;

/* Find router index from its name */
int findRouter(char s[])
{
    int i;

    for (i = 0; i < N; i++)
    {
        if (strcmp(name[i], s) == 0)
            return i;
    }

    return -1;
}

/* Print distance vector table */
void printDVTable(int dist[MAX][MAX], int iteration)
{
    int i, j;

    printf("\n========== ITERATION %d ==========\n", iteration);

    printf("%-12s", "Router");

    for (j = 0; j < N; j++)
        printf("%-10s", name[j]);

    printf("\n");

    for (i = 0; i < N; i++)
    {
        printf("%-12s", name[i]);

        for (j = 0; j < N; j++)
        {
            if (dist[i][j] >= INF)
                printf("%-10s", "INF");
            else
                printf("%-10d", dist[i][j]);
        }

        printf("\n");
    }
}

/* Print a path using predecessor information */
void printPath(int parent[], int destination)
{
    if (parent[destination] == -1)
    {
        printf("%s", name[destination]);
        return;
    }

    printPath(parent, parent[destination]);
    printf(" -> %s", name[destination]);
}

/* Read and validate a source/destination router */
int readRouter(char message[])
{
    char s[20];
    int index;

    printf("%s", message);
    scanf("%19s", s);

    index = findRouter(s);

    if (index == -1)
        printf("Router not found.\n");

    return index;
}

/* DISTANCE VECTOR ROUTING */
void distanceVector()
{
    int dist[MAX][MAX], nextDist[MAX][MAX];
    int nextHop[MAX][MAX], nextNextHop[MAX][MAX];

    int i, j, k, iteration = 0, changed;

    /* Initial distance vectors */
    for (i = 0; i < N; i++)
    {
        for (j = 0; j < N; j++)
        {
            dist[i][j] = cost[i][j];

            if (i != j && cost[i][j] < INF)
                nextHop[i][j] = j;
            else
                nextHop[i][j] = -1;
        }
    }

    printDVTable(dist, iteration);

    /* Exchange distance vectors until convergence */
    do
    {
        changed = 0;

        for (i = 0; i < N; i++)
        {
            for (j = 0; j < N; j++)
            {
                nextDist[i][j] = dist[i][j];
                nextNextHop[i][j] = nextHop[i][j];
            }
        }

        for (i = 0; i < N; i++)
        {
            for (j = 0; j < N; j++)
            {
                for (k = 0; k < N; k++)
                {
                    if (i != k &&
                        cost[i][k] < INF &&
                        dist[k][j] < INF)
                    {
                        int newCost = cost[i][k] + dist[k][j];

                        if (newCost < nextDist[i][j])
                        {
                            nextDist[i][j] = newCost;
                            nextNextHop[i][j] = k;
                            changed = 1;
                        }
                    }
                }
            }
        }

        for (i = 0; i < N; i++)
        {
            for (j = 0; j < N; j++)
            {
                dist[i][j] = nextDist[i][j];
                nextHop[i][j] = nextNextHop[i][j];
            }
        }

        if (changed)
        {
            iteration++;
            printDVTable(dist, iteration);
        }

    } while (changed);

    /* Final routing tables for all routers */
    printf("\n========== FINAL ROUTING TABLES ==========\n");

    for (i = 0; i < N; i++)
    {
        printf("\nRouting Table for Router %s\n", name[i]);
        printf("%-15s %-15s %-15s\n",
               "Destination", "Cost", "Next Hop");

        for (j = 0; j < N; j++)
        {
            printf("%-15s ", name[j]);

            if (dist[i][j] >= INF)
                printf("%-15s %-15s\n", "INF", "-");
            else if (i == j)
                printf("%-15d %-15s\n", dist[i][j], "-");
            else
                printf("%-15d %-15s\n",
                       dist[i][j], name[nextHop[i][j]]);
        }
    }

    /* Shortest path for selected source and destination */
    {
        int source, destination, current, steps = 0;

        source = readRouter("\nEnter Source Router: ");

        if (source == -1)
            return;

        destination = readRouter("Enter Destination Router: ");

        if (destination == -1)
            return;

        printf("\n========== DISTANCE VECTOR RESULT ==========\n");

        if (dist[source][destination] >= INF)
        {
            printf("No path exists between %s and %s.\n",
                   name[source], name[destination]);
            return;
        }

        printf("Shortest Path: %s", name[source]);

        current = source;

        while (current != destination && steps < N)
        {
            current = nextHop[current][destination];

            if (current == -1)
            {
                printf("\nNo path exists.\n");
                return;
            }

            printf(" -> %s", name[current]);
            steps++;
        }

        printf("\nMinimum Cost: %d\n", dist[source][destination]);
    }
}

/* LINK STATE ROUTING: DIJKSTRA */
void linkState()
{
    int dist[MAX], visited[MAX], parent[MAX];
    int i, j, min, u, iteration = 0;
    int source, destination;

    source = readRouter("\nEnter Source Router: ");

    if (source == -1)
        return;

    destination = readRouter("Enter Destination Router: ");

    if (destination == -1)
        return;

    for (i = 0; i < N; i++)
    {
        dist[i] = INF;
        visited[i] = 0;
        parent[i] = -1;
    }

    dist[source] = 0;

    printf("\n========== DIJKSTRA'S ALGORITHM ==========\n");
    printf("Source Router: %s\n\n", name[source]);

    printf("%-12s %-12s", "Iteration", "Selected");

    for (i = 0; i < N; i++)
        printf("%-10s", name[i]);

    printf("\n");

    for (iteration = 1; iteration <= N; iteration++)
    {
        min = INF;
        u = -1;

        /* Select the unvisited router with minimum cost */
        for (i = 0; i < N; i++)
        {
            if (!visited[i] && dist[i] < min)
            {
                min = dist[i];
                u = i;
            }
        }

        if (u == -1)
            break;

        visited[u] = 1;

        /* Update neighbouring routers */
        for (j = 0; j < N; j++)
        {
            if (!visited[j] &&
                cost[u][j] < INF &&
                dist[u] + cost[u][j] < dist[j])
            {
                dist[j] = dist[u] + cost[u][j];
                parent[j] = u;
            }
        }

        printf("%-12d %-12s", iteration, name[u]);

        for (j = 0; j < N; j++)
        {
            if (dist[j] >= INF)
                printf("%-10s", "INF");
            else
                printf("%-10d", dist[j]);
        }

        printf("\n");
    }

    printf("\n========== LINK STATE RESULT ==========\n");

    if (dist[destination] >= INF)
    {
        printf("No path exists between %s and %s.\n",
               name[source], name[destination]);
        return;
    }

    printf("Source: %s\n", name[source]);
    printf("Destination: %s\n", name[destination]);

    printf("Shortest Path: ");
    printPath(parent, destination);

    printf("\nMinimum Cost: %d\n", dist[destination]);

    printf("\nFinal Shortest Distances from %s:\n", name[source]);

    for (i = 0; i < N; i++)
    {
        if (dist[i] >= INF)
            printf("%s = INF\n", name[i]);
        else
            printf("%s = %d\n", name[i], dist[i]);
    }
}

int main()
{
    int choice, edges, i, u, v, weight;
    char first[20], second[20];

    printf("========================================\n");
    printf("     ROUTING ALGORITHMS SIMULATOR\n");
    printf("========================================\n");

    printf("Enter Number of Routers (maximum %d): ", MAX);
    scanf("%d", &N);

    if (N < 1 || N > MAX)
    {
        printf("Invalid number of routers.\n");
        return 1;
    }

    /* Enter router names */
    printf("\nEnter Router Names:\n");

    for (i = 0; i < N; i++)
    {
        int j;

        printf("Router %d: ", i + 1);
        scanf("%19s", name[i]);

        for (j = 0; j < i; j++)
        {
            if (strcmp(name[i], name[j]) == 0)
            {
                printf("Router names must be unique.\n");
                return 1;
            }
        }
    }

    /* Initialize graph */
    for (i = 0; i < N; i++)
    {
        int j;

        for (j = 0; j < N; j++)
            cost[i][j] = (i == j) ? 0 : INF;
    }

    printf("\nEnter Number of Edges (maximum %d): ",
           N * (N - 1) / 2);
    scanf("%d", &edges);

    if (edges < 0 || edges > N * (N - 1) / 2)
    {
        printf("Invalid number of edges.\n");
        return 1;
    }

    /* Enter every edge and its cost */
    printf("\nEnter each edge as: Router1 Router2 Cost\n");
    printf("Example: a b 4\n");
    printf("Edges are undirected; costs must be non-negative.\n");

    for (i = 0; i < edges; i++)
    {
        printf("Edge %d: ", i + 1);
        scanf("%19s %19s %d", first, second, &weight);

        u = findRouter(first);
        v = findRouter(second);

        if (u == -1 || v == -1 || u == v || weight < 0 ||
            weight >= INF || cost[u][v] != INF)
        {
            printf("Invalid edge. Enter valid, unique routers and cost.\n");
            return 1;
        }

        cost[u][v] = weight;
        cost[v][u] = weight;
    }

    printf("\n1. Distance Vector Routing\n");
    printf("2. Link State Routing (Dijkstra)\n");
    printf("Enter your choice: ");
    scanf("%d", &choice);

    switch (choice)
    {
        case 1:
            distanceVector();
            break;

        case 2:
            linkState();
            break;

        default:
            printf("Invalid choice. Select 1 or 2.\n");
    }

    return 0;
}
