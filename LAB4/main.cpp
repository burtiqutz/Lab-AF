#include <iostream>
#include "../profiler/Profiler.h"
/*
 *IORDACHE ALEXANDRU 30228
 */
using namespace std;
#define MAX_SIZE 10000
#define STEP_SIZE 100
#define TEST_SIZE 5

Profiler profiler("Average");

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

int partition(int a[], int left, int right, int pivot, int n)
{
    profiler.countOperation("QuickSort", n, 3);
    swap(a[right], a[pivot]);
    int j = left - 1;
    for (int i = left; i <= right; i++)
    {
        profiler.countOperation("QuickSort", n);
        if (a[i] <= a[right])
        {
            ++j;
            profiler.countOperation("QuickSort", n, 3);
            swap(a[i], a[j]);
        }
    }

    return j;
}

void quickSort(int a[], int n, int left, int right)
{

    //de ce nu luam left == right?
    //pentru ca
    if (right <= left)      //operatii pe indici, nu numaram
    {
        return;
    }
    int k = partition(a, left, right, right, n);
    quickSort(a, n, left, k - 1);
    quickSort(a, n, k + 1, right);
}

///copied from previous homework
void sink(int a[], int n, int index, int sizeForChart) //heapify
{
    while (2 * index <= n) //check only internal nodes
    {
        profiler.countOperation("HeapSort", sizeForChart, 1); //comaparision
        int indChild = 2 * index;
        //find smallest child
        profiler.countOperation("HeapSort", sizeForChart, 2); //if statement
        if (indChild + 1 < n && a[indChild + 1] < a[indChild])
        {
            indChild++;
        }
        //swap if there is a smaller one
        profiler.countOperation("HeapSort", sizeForChart, 1);
        if (a[indChild] < a[index])
        {
            profiler.countOperation("HeapSort", sizeForChart, 3);; //swap
            swap(a[indChild], a[index]);
            index = indChild;
        }
        else
        {
            break;
        }
    }
}

void makeHeapSink(int a[], int n, int chartSize) //buildheap
{
    for (int i = n / 2; i > 0; --i)
    {
        sink(a, n, i, chartSize);
    }
}

void heapSort(int a[], int n, int chartSize)
{
    makeHeapSink(a, n, chartSize);
    while (n > 1)
    {
        profiler.countOperation("HeapSort", n, 4);
       // cout << " " << a[1];
        swap(a[1], a[n]);
        n--;
        sink(a, n, 1, chartSize);
    }
    //cout << " " << a[1];
}

void hybridQuickSort(int a[], int n, int left, int right)
{
    if (right - left <= n) //if el count < certain amount
    {
        insertionSort(a, left - right);
    }
}

void demo()
{
    int a[15];
    FillRandomArray(a, 15, -5000, 5000, false, UNSORTED);
    int b[15];
    CopyArray(b, a, 15);

    cout << "Before sorting:\n";
    for (int i = 1; i < 15; i++)
    {
        cout << " " << a[i];
    }

    quickSort(a, 15, 1, 14);
    cout << "\nAfter quickSort:\n";
    for (int i = 1; i < 15; i++)
    {
        cout << " " << a[i];
    }

    cout << "\nAfter heapSort:\n";
    heapSort(b, 14, 14); //trebuie apelat cu n - 1 pt ca merge inclusiv si iau element out of bounds @sink

}

void perf(int sortingOrder)
{
    int a[MAX_SIZE];
    int b[MAX_SIZE];

    for(int test = 0; test < TEST_SIZE; ++test)
    {
        for(int n = STEP_SIZE; n <= 10000; n += STEP_SIZE)
        {
            FillRandomArray(a, n, -5000, 5000, false, sortingOrder);
            CopyArray(b, a, n);
            quickSort(a, n, 1, n-1);
            heapSort(b, n - 1, n);
        }
    }

    profiler.divideValues("QuickSort", TEST_SIZE);
    profiler.divideValues("HeapSort", TEST_SIZE);
    profiler.createGroup("HeapVSQuick", "HeapSort", "QuickSort");
}

void perf_all()
{
    perf(UNSORTED);
    //profiler.reset("Best");
    //perf(ASCENDING);
   // profiler.reset("Worst");
    //perf(DESCENDING);
    profiler.showReport();
}

int main()
{
    //demo();
    perf_all();
    return 0;
}
