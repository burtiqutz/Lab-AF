#include <iostream>
#include "../profiler/Profiler.h"
Profiler profiler;

/**
 *  @author IORDACHE ALEXANDRU GRUPA 30228
 *  Pentru acest laborator am avut de implementat operatiile de baza pentru disjoint sets
 *  si algoritmul lui kruskal folosind ca structura de date disjoint sets.
 *  Functia de make-set creaza in O(1) singleton-uri.
 *  Functia de find-set merge liniar in sus pe vectorul de parinte.
 *  Functia de union leaga un arbore la celalalt arbore, legand un root de celalalt root.
 *  Algoritmul lui kruskal functioneaza astfel: sortam muchiile dupa cost, le parcurgem liniar si, daca cele
 *  doua noduri care construiesc muchia nu inchid un ciclu (nu fac parte din acelasi set), o adaugam la setul
 *  de muchii al arborelui minim de acoperire.
 */

#define MAX_SIZE 10000

struct node
{
    int rank;
    int key;
};

struct edge
{
    int x, y, cost;
};

node parent[10005];
std::vector<edge> edges;

bool compare(edge e1, edge e2)
{
    return e1.cost < e2.cost;
}

void makeSet(int const x, Operation &op)
{
    op.count(2);
    parent[x].key = x;
    parent[x].rank = 1;
}

int findSet(int const x, Operation &op)
{
    op.count();
    if(x == parent[x].key)
    {
        return x;
    }
    op.count();
    parent[x].key = findSet(parent[x].key, op);

    return parent[x].key;
}

void unionSet(int const x, int const y, Operation &op)
{
    // if(x != y)
    // {
    //     if(parent[x].rank < parent[y].rank)
    //     {
    //         parent[x].key = parent[y].key;
    //         parent[x].rank = parent[y].rank;
    //     } else if (parent[y].rank < parent[x].rank)
    //     {
    //         parent[y].key = parent[x].key;
    //         parent[y].rank = parent[x].rank;
    //     } else
    //     {
    //         parent[x].key = parent[y].key;
    //         parent[y].rank++;
    //         parent[x].rank = parent[y].rank;
    //     }
    // }
    op.count(2);
    int rootX = findSet(x, op);
    int rootY = findSet(y, op);
    if(rootX != rootY)
    {
        if (parent[rootX].rank < parent[rootY].rank) {
            op.count(2);
            parent[rootX].key = rootY;
        } else if (parent[rootX].rank > parent[rootY].rank) {
            op.count(2);
            parent[rootY].key = rootX;
        } else {
            op.count(3);
            parent[rootX].key = rootY;
            parent[rootY].rank++;
        }
    }
}

int mstKruskal(const std::vector<int>& vertex, Operation &opFind, Operation &opMake, Operation &opUnion, bool print)
{
    int set = 0;
    std::vector<edge> mstEdges;

     for(int const key : vertex)
     {
         makeSet(key, opMake);
     }

    std::sort(edges.begin(), edges.end(), compare);

    for (const auto& curEdge : edges) {
        if (findSet(curEdge.x, opFind) != findSet(curEdge.y, opFind)) {
            unionSet(curEdge.x, curEdge.y, opUnion);
            mstEdges.push_back(curEdge);
            Operation opAux = profiler.createOperation("dummy for remembering set", 1);
            set = findSet(curEdge.x, opAux);
        }
    }

    if(print)
    {
        std::cout << "MST edges:\n";
        for (const auto& e : mstEdges) {
            std::cout << "(" << e.x << ", " << e.y << ") cost: " << e.cost << "\n";
        }
    }
    return set;
}

void printSet(int key)
{
    Operation op = profiler.createOperation("dummy", 1);
    int root = findSet(key, op);
    std::cout << "Set with key " << key << " is: ";
    for(int i = 0; i < 10005; i++)
    {
        if(findSet(i, op) == root)
        {
            std::cout << " " << i;
        }
    }
    std::cout << '\n';
}

void demo()
{
    Operation op = profiler.createOperation("dummy", 1);
    for(int i = 1; i <= 10; i++)
    {
        makeSet(i, op);
    }
    for(int i = 1; i <= 10; i++)
    {
        printSet(parent[i].key);
    }
    unionSet(1, 3, op);
    unionSet(1, 6, op);
    unionSet(2, 8, op);
    unionSet(2, 10, op);
    unionSet(1, 2, op);
    std::cout << findSet(1, op) << '\n';
    std::cout << findSet(3, op) << '\n';
    std::cout << findSet(4, op) << '\n';
    std::cout << findSet(5, op) << '\n';
    std:: cout << findSet(10, op) << '\n';
    printSet(1);
    printSet(4);
    printSet(5);
    printSet(7);

    edges = {
        {1, 2, 5},
        {1, 4, 2},
        {2, 5, 3},
        {3, 5, 10},
        {1, 5, 15},
        {2, 4, 8},
        {2, 3, 1},
        {4, 5, 3},
        {3, 4, 7}
    };
    std::cout << "All edges in graph:\n";
    for (const auto& e : edges) {
        std::cout << "(" << e.x << ", " << e.y << ") cost: " << e.cost << "\n";
    }
    std::vector<int> vertexes = {1, 2, 3, 4, 5};

    int set = mstKruskal(vertexes, op, op, op, true);
    printSet(set);
}

void perf()
{
    srand(time(0));
    for(int n = 100; n <= MAX_SIZE; n += 100)
    {
        //  Reset memory
        memset(parent, 0, sizeof(parent));
        edges.clear();
        //  Create a connected graph
        for(int i = 1; i < n; i++)
        {
            edges.push_back({i, i + 1, rand()});
        }
        //  Generate the rest of the edges
        for(int i = 0; i < 3 * n; i++)
        {
            int x = 0, y = 0;
            while(x == y)
            {
                x = rand() % n + 1;
                y = rand() % n + 1;
            }
            edges.push_back({x, y, rand()});
        }
        Operation opMake = profiler.createOperation("Make-Set", n);
        Operation opFind = profiler.createOperation("Find-Set", n);
        Operation opUnion = profiler.createOperation("Union-Set", n);
        //  Create vertex list
        std::vector<int> vertexes;
        for(int i = 1; i <= n; i++)
        {
            vertexes.push_back(i);
        }
        mstKruskal(vertexes, opFind, opMake, opUnion, false);
    }

    profiler.createGroup("Comparasion", "Make-Set", "Find-Set", "Union-Set");
    profiler.showReport();
}

int main()
{
    //demo();
    perf();
    return 0;
}
