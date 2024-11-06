#include <iostream>
#include "../profiler/Profiler.h"
/*
 *IORDACHE ALEXANDRU 30228
 * Pentru acest laborator am avut de implementat QuickSort si de testat eficienta acestuia vs HeapSort.
 * Quicksort functioneaza pe baza principiului de divide & conquer: se alege un pivot, trecandu-se toate elementele mai
 * mici decat acesta in stanga lui, cele mai mari in dreapta lui, iar apoi se apeleaza recursiv quicksort pe cele doua
 * partitii generate ale sirului.
 * O mica nota: am refacut heapsort cu indexare de la 0 pentru ca imi era mai simplu de vazut greselile.
 * In cazul mediu statistic, quicksort este considerabil mai rapid decat heapsort, insa ambele urmeaza o curba de tipul
 * nlogn.
 *In best case complexitatea ramane nlogn, si ar fi in cazul cand pivotul se alege elementul median si sirul este sortat.
 *Worst case-ul pentru pivotul ales de noi la laborator (elementul cel mai din dreapta) este cand sirul este deja sortat
 *ceea ce duce la o complexitate de n^2.
 *Am implementat si hibridizarea quicksortului prin folosirea insertion sort iterativ sub un anumit prag. Pragul difera
 *in functie de task urile de fundal si un factor seemingly random, dar este in jur de 20-30 pentru testele facute.
 *Hybrid quicksort ofera un improvement notabil atat in numar de operatii, cat si secunde.
 *Pentru a vedea graficul ,trebuie decomentata functia din perf_all().
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

void insertionSort(int* a, int low, int n, Operation& op)
{
    for (int i = low + 1; i < n; i++)
    {
        op.count();
        int el = a[i];
        int j = i - 1;
        op.count();
        while (low <= j && a[j] > el)
        {
            op.count();
            a[j + 1] = a[j];
            j--;
        }
        op.count();
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
    if (right <= left) //operatii pe indici, nu numaram
    {
        return;
    }
    int k;
    if (posPivot == 0)
    {
        //default
        k = partition(a, left, right, right, n);
    }
    else if (posPivot == 1)
    {
        k = partition(a, left, right, (left + right) / 2, n);
    }
    else
    {
        k = partition(a, left, right, left, n);
    }

    quickSort(a, n, left, k - 1, posPivot);
    quickSort(a, n, k + 1, right, posPivot);
}

int hybridPartition(int a[], int left, int right, int pivot, int n, Operation& op)
{
    //profiler.countOperation("hybridQuickSort", n, 3);
    op.count(3);
    swap(a[right], a[pivot]);
    int j = left - 1;
    for (int i = left; i <= right; i++)
    {
        // profiler.countOperation("hybridQuickSort", n);
        op.count();
        if (a[i] <= a[right])
        {
            ++j;
            //profiler.countOperation("hybridQuickSort", n, 3);
            op.count(3);
            swap(a[i], a[j]);
        }
    }

    return j;
}

void hybridQuickSort(int a[], int n, int left, int right, int prag, Operation& op)
{
    //profiler.countOperation("hybridQuickSort", n, 1);
    op.count();
    if (right - left < prag)
    {
        insertionSort(a, left, right, op);
    }
    else
    {
        //profiler.countOperation("hybridQuickSort", n, 1);
        op.count();
        if (left < right)
        {
            //int pivot = rand() % (right - left + 1) + left;
            int pivot = right;
            int q = hybridPartition(a, left, right, pivot , n, op);
            hybridQuickSort(a, n, left, q - 1, prag, op);
            hybridQuickSort(a, n, q + 1, right, prag, op);
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
//nu numar nimic la quickselect asa ca am rescris functia fara count uri
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
    swap(a[j + 1], a[right]);
    return j + 1;
}

int quickSelect(int a[], int p, int r, int i)
{
    if (p == r)
    {
        return a[p];
    }
    int q = hoarePartition(a, p, r);
    int k = q - p + 1;
    if (i == k)
    {
        return a[q];
    }
    else if (i < q)
    {
        return quickSelect(a, p, q - 1, i);
    }
    else
        return quickSelect(a, q + 1, r, i - k);
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

    int c[10] = {1, 15, 0, -1, 16, -8, 5, 13, 9, 2};
    cout << "quickselect test: " << quickSelect(c, 0, 9, 3) << '\n';
    //i este indexat de la 1 la n, adica cel mai mic, al doilea c m mic etc

    Operation dummy = profiler.createOperation("dummy", 20);
    int d[20];
    int e[20];
    FillRandomArray(e, 20, 0, 500, false, UNSORTED);
    cout << "Testing insertion sort. Before sorting:\n";
    print_array(e, 20);
    cout << "After sorting:\n";
    insertionSort(e, 0, 20, dummy);
    print_array(e, 20);

    cout << "Testing hybrid quicksort. Before sorting:\n";
    FillRandomArray(d, 20, 0, 500, false, UNSORTED);
    print_array(d, 20);
    cout << "After sorting:\n";
    hybridQuickSort(d, 100, 0, 20, 15, dummy);
    print_array(d, 20);
}

void findPrag()
{
    int arr[MAX_SIZE];
    int aux[MAX_SIZE];
    FillRandomArray(aux, MAX_SIZE, -5000, 5000, false, UNSORTED);
    for (int prag = 5; prag <= 50; prag += 5)
    {
        Operation op = profiler.createOperation("prag", prag);
        CopyArray(arr, aux, MAX_SIZE);
        hybridQuickSort(arr, MAX_SIZE, 0, MAX_SIZE, prag, op);
    }

    for (int prag = 5; prag <= 50; prag += 5)
    {
        profiler.startTimer("timerHybrid", prag);
        for (int i = 0; i < 100; ++i)
        {
            Operation op = profiler.createOperation("dummy", prag); //numai pt a pasa ca parametru
            CopyArray(arr, aux, MAX_SIZE);
            hybridQuickSort(arr, MAX_SIZE - 1, 0, MAX_SIZE, prag, op);
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

void perfNormalVsHybrid(int sortingOrder)
{
    for (int n = STEP_SIZE; n <= 10000; n += STEP_SIZE)
    {
        int a[n];
        int b[n];
        for (int test = 0; test < TEST_SIZE; ++test)
        {
            Operation hybrid = profiler.createOperation("HybridQuickSort", n);
            FillRandomArray(a, n, -5000, 5000, false, sortingOrder);
            CopyArray(b, a, n);
            quickSort(a, n, 1, n - 1, 0);
            hybridQuickSort(b, n, 0, n, 31, hybrid);
        }

        int aux[n];
        FillRandomArray(aux, n, -5000, 5000, false, sortingOrder);
        profiler.startTimer("TimerNormal", n);
        for (int j = 0; j < 100; ++j)
        {
            CopyArray(a, aux, n);
            quickSort(a,n, 0, n - 1, 0);
        }
        profiler.stopTimer("TimerNormal", n);

        profiler.startTimer("TimerHybrid", n);
        Operation dummy = profiler.createOperation("dummy", 1);
        for(int j = 0; j < 100; ++j)
        {
            CopyArray(b, aux, n);
            hybridQuickSort(b, n, 0, n, 31, dummy);
        }
        profiler.stopTimer("TimerHybrid", n);
    }

    profiler.divideValues("QuickSort", TEST_SIZE);
    profiler.divideValues("HybridQuickSort", TEST_SIZE);
    profiler.createGroup("HybridVsNormal", "HybridQuickSort", "QuickSort");
    profiler.createGroup("Timers", "TimerHybrid", "TimerNormal");
}

void perf_all()
{
    perf(UNSORTED); //quick vs heap
    profiler.reset("Worst");
    perfQuickSortOnly(ASCENDING, 0); //ascending cu pivot pe dreapta duce la O(n^2)
    profiler.reset("Best");
    perfQuickSortOnly(ASCENDING, 1); //pt best case, sirul este ascending cu pivot median
   // perfNormalVsHybrid(UNSORTED);

    profiler.showReport();
}

int main()
{
    //demo();
    //perf_all();
    findPrag();
    return 0;
}
