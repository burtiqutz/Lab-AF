#include <iostream>
#include <queue>

#include "../profiler/Profiler.h"
Profiler profiler("Average");

using namespace std;
/*
 *Iordache Alexandru grupa 30228
 *Am avut de implementat in acest laborator k-way merge.
 * Am folosit o lista de liste pentru a putea implementa un heap. Fiecare nod al heap ului reprezinta de fapt o lista
 * sortata crescator.
 * Algoritmul se foloseste de proprietatea structurii de heap pentru a putea gasi minimul dintre orice numar de liste.
 * Se extrage radacina heapului (primul element dintr-o lista) si se aplica heapify pentru a recrea heap ul. De fiecare
 * data cand extragem un numar dintr-o lista, il scoatem din acea lista si este inlocuit de urmatorul, daca exista, daca
 * nu exista scadem heapsize dupa extragere si practic nu mai exista lista(conceptual).
 * Pentru a genera liste aleatorii am folosit functia FillRandomArray (descending) si am facut insert first in liste.
 * Folosim min-heap implementat bottom up, adaptat conform structurii noi folosite.
 * Algoritmul variaza, confrom graficelor, liniar cu n si logaritmic cu k => O(nlogk)
 */

typedef struct node
{
    int key;
    node* next;
} Node;

Node* h[501] = {nullptr};
//no counting here, O(1)
Node* createNode(int key)
{
    Node* newNode = new Node(); //what ?? :))))
    //Node* newNode = (Node*)malloc(sizeof(Node));
    newNode->key = key;
    newNode->next = nullptr;
    return newNode;
}

void resetHeap(int k)
{
    //reset heap for reuse
    for (int i = 1; i <= k; i++)
    {
        h[i] = nullptr;
    }
}

void sink(int index, int k, Operation& op)
{
    while (2 * index <= k)
    {
        int smallest = 2 * index;
        op.count();
        if (smallest + 1 <= k && h[smallest + 1]->key < h[smallest]->key)
        {
            smallest++;
        }
        op.count();
        if (h[smallest]->key < h[index]->key)
        {
            op.count(3);
            swap(h[smallest], h[index]);
            index = smallest;
        }
        else
        {
            break;
        }
    }
}

void makeHeap(int k, Operation& op)
{
    for (int i = k / 2; i >= 1; --i)
    {
        sink(i, k, op);
    }
}

void generateKLists(bool showLists, int n, int k)
{
    int listSize = n / k;

    for (int i = 1; i < k; i++)
    {
        int aux[listSize];
        FillRandomArray(aux, listSize, 0, 10000, false, DESCENDING); //aux array for generating numbers
        for (int j = 0; j < listSize; j++)
        {
            Node* toInsert = createNode(aux[j]);
            if (h[i] == nullptr)
            {
                h[i] = toInsert;
            }
            else
            {
                //insert first
                toInsert->next = h[i];
                h[i] = toInsert;
            }
        }
    }
    //seperate for last list
    //we have to insert last few elements
    //ex n = 24, k = 3 => list of 8, list of 8, list of 9
    int aux[listSize + n % k] = {0};
    FillRandomArray(aux, listSize + n % k, 0, 10000, false, DESCENDING); //aux array for generating numbers
    for (int j = 0; j < listSize + n % k; j++)
    {
        Node* toInsert = createNode(aux[j]);
        if (h[k] == nullptr)
        {
            //never enters here after i used the heap once already
            h[k] = toInsert;
        }
        else
        {
            //insert first
            toInsert->next = h[k];
            h[k] = toInsert;
        }
    }
    if (showLists)
    {
        //for demo, we dont count here
        cout << "generated lists:\n";
        for (int i = 1; i <= k; ++i)
        {
            Node* auxNode = h[i];
            cout << "list " << i << '\n';
            while (auxNode != nullptr)
            {
                cout << " " << auxNode->key;
                auxNode = auxNode->next;
            }
            cout << '\n';
        }
    }
}

queue<int> sol; //solution queue
void mergeKOrderedLists(bool showLists, int k, int n, Operation& op)
{
    generateKLists(showLists, n, k);
    makeHeap(k, op);
    int heapSize = k; //fucks up charts if we dont
    while (heapSize > 0)
    {
        op.count(2); //push and comparision
        sol.push(h[1]->key);
        if (h[1]->next != nullptr)
        {
            op.count(); //idk if i should count 1 or 3 here, since in other languages we dont use pointers
            Node* aux = h[1];
            h[1] = h[1]->next;
            free(aux);
        }
        else
        {
            op.count();
            Node* aux = h[1];
            h[1] = h[heapSize];
            free(aux);
            heapSize--;
        }
        sink(1, heapSize, op);
    }
}


void demo()
{
    Operation op = profiler.createOperation("dummy", 1);
    mergeKOrderedLists(true, 4, 20, op); //pt ca am h global, nu pot sa afisez aici listele, pt ca dupa merge
    //heap ul e gol, asa ca il afisez in generate lists
    while (!sol.empty())
    {
        cout << " " << sol.front();
        sol.pop();
    }
}

void perf()
{
    for (int n = 100; n <= 10000; n += 100)
    {
        resetHeap(500);
        Operation op1 = profiler.createOperation("k1", n);
        mergeKOrderedLists(false, 5, n, op1);
        resetHeap(500);
        Operation op2 = profiler.createOperation("k2", n);
        mergeKOrderedLists(false, 10, n, op2);
        resetHeap(500);
        Operation op3 = profiler.createOperation("k3", n);
        mergeKOrderedLists(false, 100, n, op3);
    }
    profiler.createGroup("nVariable", "k1", "k2", "k3");

    int n = 10000;
    resetHeap(500);
    for (int k = 10; k <= 500; k += 10)
    {
        resetHeap(k);
        Operation op4 = profiler.createOperation("kVariable", k);
        mergeKOrderedLists(false, k, n, op4);
    }
    profiler.showReport();
}

int main()
{
    //demo();
    perf();
    return 0;
}
