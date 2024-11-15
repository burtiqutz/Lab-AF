/*
 * Iordache Alexandru 30228
 *Am implementat 3 variante pentru sortare: selection, insertion si bubble sort.
 *Selection sort imparte sirul in doua parti, una sortata si una nesortata, cautand in partea nesortata minimul,
 *pe care il insereaza la sfarsitul partii sortate.
 *Insertion sort imparte sirul in doua parti, una sortata si una nesortata, alegand primul element din partea nesortata
 *si inserandul in pozitia potrivita in partea sortata.
 *Bubble sort compara elementele adiacente si le interschimba, luand apoi de la capat parcurgerea.
 *Am observat ca in best case cel mai bun algoritm este bubblesort, deoarece trece o singura data prin sir (liniar), iar
 *selection cel mai prost(pentru ca mereu cauta minimul din partea nesortata).
 *Iar in worst case este tot bubble sort cel mai prost, pentru ca trebuie sa parcurga incontinuu sirul de la inceput
 *(cu tot cu optimizarea in care nu mergem pe elementele de la final), iar cel mai bun Selection.
 *In cazul mediu, cel mai bun este egalitate intre selection si insertion, cu bubble fiind semnificativ mai rau dpdv sta
 *tistic.
 */
#include <iostream>
#include "../profiler/Profiler.h"
Profiler profiler("Average");

#define MAX_SIZE 10000
#define STEP_SIZE 100
#define TEST_SIZE 5

using namespace std;

void swap(int* a, int* b)
{
    int temp = *a;
    *a = *b;
    *b = temp;
}

int binarySearch(int a[], int searched, int left, int right, Operation &opComp, Operation &opAsgn)
{

    while (left <= right)
    {
        opComp.count();
        opAsgn.count();
        int mid = (left + right) / 2;
        opComp.count();
        if(searched == a[mid])
            return ++mid;
        else
        {
            opComp.count();
            if (searched > a[mid])
            {
                left = ++mid;
            } else
            {
                right = --mid;
            }
            opAsgn.count();
        }
    }
    return left;

}

void selectionSort(int* a, int n)
{
    Operation opComp = profiler.createOperation("SelectionSortComp", n);
    Operation opAsgn = profiler.createOperation("SelectionSortAsgn", n);
    for (int i = 0; i < n - 1; i++)
    {
        int pos = i;
        for (int j = i + 1; j < n; j++)
        {
            opComp.count();
            if (a[j] < a[pos])
            {
                pos = j;
            }
        }
        if (i != pos)
        {
            opAsgn.count(3);
            swap(&a[i], &a[pos]);
        }
    }
}

void insertionSort(int* a, int n)
{
    Operation opCompIS = profiler.createOperation("InsertionSortComp", n);
    Operation opAsgn = profiler.createOperation("InsertionSortAsgn", n);
    for (int i = 1; i < n; i++)
    {
        int el = a[i];
        opAsgn.count();
        int j = i - 1;
        while (0 <= j && a[j] > el)
        {
            opCompIS.count();
            opAsgn.count();
            a[j + 1] = a[j];
            j--;
        }
        opAsgn.count();
        a[j + 1] = el;
    }
}
void binaryInsertionSort(int a[], int n)
{
    Operation opCompIS = profiler.createOperation("BinaryInsertionSortComp", n);
    Operation opAsgn = profiler.createOperation("BinaryInsertionSortAsgn", n);
    for (int i = 1; i < n; i++)
    {
        opAsgn.count();
        int el = a[i];
        int j = i - 1;
        opAsgn.count();
        int pos = binarySearch(a, el, 0, j, opCompIS, opAsgn);

        while (pos <= j)
        {
            opCompIS.count();
            opAsgn.count();
            a[j + 1] = a[j];
            j--;
        }
        opAsgn.count();
        a[j + 1] = el;
    }
}
void bubbleSort(int* a, int n)
{
    Operation opComp = profiler.createOperation("BubbleSortComp", n);
    Operation opAsgn = profiler.createOperation("BubbleSortAsgn", n);
    int sorted = 0;
    int elCount = 1;
    do
    {
        sorted = 1;
        for (int i = 0; i < n - elCount; i++)
        {
            opComp.count();
            if (a[i] > a[i + 1])
            {
                opAsgn.count(3);
                swap(&a[i], &a[i + 1]);
                sorted = 0;
            }
        }
        //la fiecare parcurgere a sirului, un element ajunge pe pozitia sa finala
        //deci nu mai trebuie sa parcurgem pana acolo
        elCount++;
    }
    while (!sorted);
}

void perf(int order)
{
    int a[MAX_SIZE];
    int b[MAX_SIZE];
    int c[MAX_SIZE];
    int d[MAX_SIZE];
    for (int test = 0; test < TEST_SIZE; test++)
    {
        for (int n = STEP_SIZE; n <= MAX_SIZE; n += STEP_SIZE)
        {
            FillRandomArray(a, n, -5000, 5000, false, order);
            CopyArray(b, a, n);
            CopyArray(c, a, n);
            CopyArray(d, a, n);

            selectionSort(a, n);
            insertionSort(b, n);
            bubbleSort(c, n);
            binaryInsertionSort(d, n);
        }
    }

    profiler.divideValues("SelectionSortComp", TEST_SIZE);
    profiler.divideValues("SelectionSortAsgn", TEST_SIZE);
    profiler.addSeries("SelectionSort", "SelectionSortAsgn", "SelectionSortComp");

    profiler.divideValues("InsertionSortComp", TEST_SIZE);
    profiler.divideValues("InsertionSortAsgn", TEST_SIZE);
    profiler.addSeries("InsertionSort", "InsertionSortComp", "InsertionSortAsgn");

    profiler.divideValues("BubbleSortAsgn", TEST_SIZE);
    profiler.divideValues("BubbleSortComp", TEST_SIZE);
    profiler.addSeries("BubbleSort", "BubbleSortAsgn", "BubbleSortComp");

    profiler.divideValues("BinaryInsertionSortAsgn", TEST_SIZE);
    profiler.divideValues("BinaryInsertionSortComp", TEST_SIZE);
    profiler.addSeries("BinaryInsertionSort", "BinaryInsertionSortAsgn", "BinaryInsertionSortComp");

    profiler.createGroup("Asgn", "SelectionSortAsgn", "InsertionSortAsgn", "BubbleSortAsgn", "BinaryInsertionSortAsgn");
    profiler.createGroup("Asgn Selection Sort (for avg case)", "SelectionSortAsgn");
    profiler.createGroup("Comp", "SelectionSortComp", "InsertionSortComp", "BubbleSortComp", "BinaryInsertionSortComp");
    profiler.createGroup("Comp BubbleSort (for best case)", "BubbleSortComp");
    profiler.createGroup("Overall", "SelectionSort", "InsertionSort", "BubbleSort", "BinaryInsertionSort");
    profiler.createGroup("OverallOnlyBubbleInsertionBinary", "BubbleSort", "InsertionSort", "BinaryInsertionSort");
}

void perf_all()
{
    perf(UNSORTED);
    profiler.reset("best");
    perf(ASCENDING);
    profiler.reset("worst");
    perf(DESCENDING);
    profiler.showReport();
}

void demo()
{
    // demo pt corectitudinea algoritmilor

    int a[20];
    FillRandomArray(a, 20, -5000, 5000, false, UNSORTED);
    int b[20];
    int c[20];
    int d[20];
    const int n = 20;
    CopyArray(b, a, n);
    CopyArray(c, a, n);
    CopyArray(d, a, n);
    bubbleSort(a, n);
    insertionSort(b, n);
    selectionSort(c, n);
    binaryInsertionSort(d, n);
    for (int i = 0; i < n; i++)
    {
        cout << a[i] << " ";
    }
    cout << '\n';
    for (int i = 0; i < n; i++)
    {
        cout << b[i] << " ";
    }
    cout << '\n';
    for (int i = 0; i < n; i++)
    {
        cout << c[i] << " ";
    }
    cout << '\n';
    for (int i = 0; i < n; i++)
    {
        cout << d[i] << " ";
    }
}

int main()
{
    //demo();
    perf_all();
    return 0;
}
