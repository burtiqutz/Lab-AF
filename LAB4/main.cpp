#include <iostream>
#include "../profiler/Profiler.h"
/*
 *IORDACHE ALEXANDRU 30228
 * Pentru acest laborator am avut de implementat QuickSort si de testat eficienta acestuia vs HeapSort.
 * Quicksort functioneaza pe baza principiului de divide & conquer: se alege un pivot, trecandu-se toate elementele mai
 * mici decat acesta in stanga lui, cele mai mari in dreapta lui, iar apoi se apeleaza recursiv quicksort pe cele doua
 * partitii generate ale sirului.
 * In cazul mediu statistic, quicksort este considerabil mai rapid decat heapsort, insa ambele urmeaza o curba de tipul
 * nlogn.
 *In best case complexitatea ramane nlogn, si ar fi in cazul cand pivotul se alege elementul median si sirul este sortat.
 *Worst case-ul pentru pivotul ales de noi la laborator (elementul cel mai din dreapta) este cand sirul este deja sortat
 *ceea ce duce la o complexitate de n^2.
 */
using namespace std;
#define MAX_SIZE 10000
#define STEP_SIZE 100
#define TEST_SIZE 5

Profiler profiler("Average");

void print_array(int arr[], int size)
{
    for (int i = 0; i < size; i++)
    {
        cout << " " << arr[i];
    }
    cout << '\n';
}

void insertionSort(int* a, int low, int n)
{
    for (int i = low + 1; i < n; i++)
    {
        int el = a[i];
        int j = i - 1;
        while (low <= j && a[j] > el)
        {
            a[j + 1] = a[j];
            j--;
        }
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

void quickSort(int a[], int n, int left, int right, int posPivot)
{
    //de ce nu luam left == right?
    //pentru ca
    if (right <= left) //operatii pe indici, nu numaram
    {
        return;
    }
    int k;
    if(posPivot == 0)
    {
        k = partition(a, left, right, right, n);
    } else
    {
        k = partition(a, left, right, (left + right)/2, n);
    }

    quickSort(a, n, left, k - 1, posPivot);
    quickSort(a, n, k + 1, right, posPivot);
}

//PURELY FOR COUNTING PURPOSES BECAUSE I DIDN'T WANT TO REWRITE THE CODE TO INCLUDE OPERATION REFFERENCE LOL
int hybridPartition(int a[], int left, int right, int pivot, int n)
{
    profiler.countOperation("hybridQuickSort", n, 3);
    swap(a[right], a[pivot]);
    int j = left - 1;
    for (int i = left; i <= right; i++)
    {
        profiler.countOperation("hybridQuickSort", n);
        if (a[i] <= a[right])
        {
            ++j;
            profiler.countOperation("hybridQuickSort", n, 3);
            swap(a[i], a[j]);
        }
    }

    return j;
}
void hybridQuickSort(int a[], int n, int left, int right, int prag)
{
    profiler.countOperation("hybridQuickSort", n, 1);
    if (left - right < prag)
    {
        insertionSort(a, left, right);
    }
    else
    {   profiler.countOperation("hybridQuickSort", n, 1);
        if (left < right)
        {
            int q = hybridPartition(a, left, right, right, n);
            hybridQuickSort(a, n, left, q - 1, prag);
            hybridQuickSort(a, n, q + 1, right, prag);
        }
    }
}

void minHeapify(int a[], int n, int i, int chartSize)
{
    int l = 2 * i + 1;
    int r = 2 * i + 2;
    int largest;
    profiler.countOperation("HeapSort", chartSize, 1);
    if (l < n && a[l] > a[i])
    {
        largest = l;
    }
    else
    {
        largest = i;
    }
    profiler.countOperation("HeapSort", chartSize, 1);
    if (r < n && a[r] > a[largest])
    {
        largest = r;
    }

    if (largest != i)
    {
        profiler.countOperation("HeapSort", chartSize, 3);
        swap(a[i], a[largest]);
        minHeapify(a, n, largest, chartSize);
    }
}

void buildHeap(int a[], int n, int chartSize)
{
    for (int i = n / 2 - 1; i >= 0; --i)
    {
        minHeapify(a, n, i, chartSize);
    }
}

void heapSort(int a[], int heapSize, int chartSize)
{
    buildHeap(a, heapSize, chartSize);
    for (int i = heapSize - 1; i > 0; --i)
    {
        profiler.countOperation("HeapSort", chartSize, 3);
        swap(a[0], a[i]);
        heapSize--;
        minHeapify(a, i, 0, chartSize);
        profiler.countOperation("HeapSort", chartSize, 1); //daca l am muta in alt array
    }
}

int hoarePartition(int a[], int left, int right)
{
    int pivot = a[right];
    int j = left - 1;
    for (int i = left; i < right; i++)
    {
        if (a[i] <= pivot)
        {
            ++j;
            swap(a[i], a[j]);
        }
    }

    return j;
}

int quickSelect(int a[], int p, int r, int i)
{
    if (p == r)
    {
        return a[p];
    }
    int q = hoarePartition(a, p, r);
    int k = q - p + 1;
    if (i <= k)
    {
        return quickSelect(a, p, q, i);
    }
    else
    {
        return quickSelect(a, q + 1, r, i - k);
    }
}

void demo()
{
    int a[15];
    FillRandomArray(a, 15, -5000, 5000, false, UNSORTED);
    int b[15];
    CopyArray(b, a, 15);

    cout << "Before sorting:\n";
    print_array(a, 15);

    quickSort(a, 15, 0, 14, 0);
    cout << "After quickSort:\n";
    print_array(a, 15);

    cout << "After heapSort:\n";
    heapSort(b, 15, 15);
    print_array(a, 15);

    //int c[10] = {1, 15, 0, -1, 16, -8, 5, 13, 9, 2};
    // cout << "quickselect test: " << quickSelect(c, 0, 9, 2)<< '\n';

    int d[20];
    FillRandomArray(d, 20, 0, 500, false, UNSORTED);
    cout << "Testing hybrid quicksort. Before sorting:\n";
    print_array(d, 20);
    cout<<"After sorting:\n";
    hybridQuickSort(d, 100, 0, 20, 15);
    print_array(d, 20);
}

void findPrag()
{
    int arr[MAX_SIZE];
    int aux[MAX_SIZE];
    FillRandomArray(aux, MAX_SIZE, -5000, 5000, false, UNSORTED);
    for(int prag = 5; prag <= 50; prag += 5)
    {
        CopyArray(arr, aux, MAX_SIZE);
        hybridQuickSort(arr, MAX_SIZE, 0, MAX_SIZE, prag);
    }
    for(int prag = 5; prag <= 50; prag += 5)
    {
        profiler.startTimer("timerHybrid", prag);
        for(int i = 0; i < 100; ++i)
        {
            CopyArray(arr, aux, MAX_SIZE);
            hybridQuickSort(arr, MAX_SIZE - 1, 0, MAX_SIZE, prag);
        }
        profiler.stopTimer("timerHybrid", prag);
    }

    profiler.showReport();
}

void perf(int sortingOrder)
{
    int a[MAX_SIZE];
    int b[MAX_SIZE];

    for (int test = 0; test < TEST_SIZE; ++test)
    {
        for (int n = STEP_SIZE; n <= 10000; n += STEP_SIZE)
        {
            FillRandomArray(a, n, -5000, 5000, false, sortingOrder);
            CopyArray(b, a, n);
            quickSort(a, n, 1, n - 1, 0);
            heapSort(b, n - 1, n);
        }
    }

    profiler.divideValues("QuickSort", TEST_SIZE);
    profiler.divideValues("HeapSort", TEST_SIZE);
    profiler.createGroup("HeapVSQuick", "HeapSort", "QuickSort");
}

void perfQuickSortOnly(int sortingOrder, int posPivot)
{
    ///for worst and best case
    int a[MAX_SIZE];
    for (int n = STEP_SIZE; n <= 10000; n += STEP_SIZE)
    {
        for (int test = 0; test < TEST_SIZE; ++test)
        {
            FillRandomArray(a, n, -5000, 5000, false, sortingOrder);
            quickSort(a, n, 1, n - 1, posPivot);
        }
    }
    profiler.divideValues("QuickSort", TEST_SIZE);
}

void perf_all()
{
    perf(UNSORTED);
    profiler.reset("Worst"); ///ascending cu pivot pe dreapta duce la O(n^2)
    perfQuickSortOnly(ASCENDING, 1);
    profiler.reset("Best"); //pt best case, sirul este ascending cu pivot median
    perfQuickSortOnly(ASCENDING, 1);

    profiler.showReport();
}

int main()
{
    //demo();
    //perf_all();
    findPrag();
    return 0;
}
