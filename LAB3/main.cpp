#include <iostream>
#include "../profiler/Profiler.h"

#define MAX_SIZE 10000
#define STEP_SIZE 100
#define TEST_SIZE 5

using namespace std;

/*
 *IORDACHE ALEXANDRU GRUPA 30228
 *Am implementat cele 2 variante de a crea un heap (min heap in cazul meu).
 *Prima varianta (swim, sau top down), incepe de la inceputul sirului in jos(top-down), si impinge elementele pe pozitia
 *lor in sus.
 *Cea de-a doua varianta (sink, bottom-up), incepe de la jumatatea sirului si merge in sus pe acesta, impingand elementele
 *in jos in heap pe pozitia lor.
 *Cum noi cream un minheap, worst case este atunci cand elementele sunt sortate descrescator, adica toate elementele nu
 *respecta relatia de ordine.
 *In average case, bottom up este putin mai eficient, la 10k elemente doar cateva mii de operatii diferenta, ambele sunt
 *nlogn, insa la worst case este un ordin de magnitudine mai eficient (cam de 8 ori mai rapid), nlogn vs n.
 *Pentru sortari, am folosit bubble sort. Statistic, in average, cele doua sunt more or less identice.
 */

Profiler profiler("Average");

void swim(int a[], int n, int index)
{
    while (index > 1 && a[index] < a[index / 2])
    {
        profiler.countOperation("Swim", n, 5); //2 comparisions and a swap
        swap(a[index], a[index / 2]);
        index /= 2;
    }
}

void makeHeapSw(int a[], int n)
{
    Operation swimCount = profiler.createOperation("Swim", n); //i dont think this is needed
    for (int i = 2; i <= n; ++i)
    {
        //swim(a, n, i, swimCount);
        swim(a, n, i);
    }
}

void sink(int a[], int n, int index)
{
    while (2 * index <= n) //check only internal nodes
    {
        profiler.countOperation("Sink", n, 1); //comaparision
        int indChild = 2 * index;
        //find smallest child
        profiler.countOperation("Sink", n, 2); //if statement
        if (indChild + 1 <= n && a[indChild + 1] < a[indChild])
        {
            indChild++;
        }
        //swap if there is a smaller one
        profiler.countOperation("Sink", n, 1);
        if (a[indChild] < a[index])
        {
            profiler.countOperation("Sink", n, 3);; //swap
            swap(a[indChild], a[index]);
            index = indChild;
        }
        else
        {
            break;
        }
    }
}

void makeHeapSink(int a[], int n)
{
    Operation sinkCount = profiler.createOperation("Sink", n);
    for (int i = n / 2; i > 0; --i)
    {
        sink(a, n, i);
    }
}

void heapSort(int a[], int n)
{
    //doar printam sirul, nu il mai salvam (dar putem si asa)
    while (n > 1)
    {
        cout << " " << a[1];
        swap(a[1], a[n]);
        n--;
        sink(a, n, 1);
    }
    cout << " " << a[1];
}

void bubbleSort(int a[], int n)
{
    //Operation op = profiler.createOperation("BubbleSort", n);
    int sorted = 0;
    int elCount = 1;
    do
    {
        sorted = 1;
        for (int i = 0; i < n - elCount; i++)
        {
            profiler.countOperation("BubbleSort", n, 1);
            //op.count();
            if (a[i] > a[i + 1])
            {
                profiler.countOperation("BubbleSort", n, 3);
                //op.count(3);
                swap(a[i], a[i + 1]);
                sorted = 0;
            }
        }
        elCount++;
        profiler.countOperation("BubbleSort", n, 1);//while check
        //op.count();
    }
    while (!sorted);
}

void recBubbleSort(int a[], int n, int size)
{
    if (n == 1)
    {
        return;
    }

    int ok = 1;
    profiler.countOperation("RecBubbleSort", size, 1);
    for (int i = 0; i < n - 1; i++)
    {
        if (a[i] > a[i + 1])
        {
            profiler.countOperation("RecBubbleSort", size, 4);//1 comp 3 asgn
            swap(a[i], a[i + 1]);
            profiler.countOperation("RecBubbleSort", size, 1);//1 asign
            ok = 0;
        }
    }
    profiler.countOperation("RecBubbleSort", size, 1);
    if (ok)
    {
        return; //daca nu am facut niciun swap am terminat
    }
    recBubbleSort(a, n - 1, size); //ultimul element este pe pozitia corecta
}

void demo()
{
    int a[MAX_SIZE];
    int b[MAX_SIZE];
    int n = 10;
    FillRandomArray(a, n + 1, 0, 100, false, UNSORTED);
    CopyArray(b, a, n + 1);
    cout << "Before heapify\n";
    for (int i = 1; i <= n; i++)
    {
        cout << " " << a[i];
    }

    cout << "\nAfter heapify\n";
    makeHeapSw(a, n);
    makeHeapSink(b, n);
    cout << "Swim method:";
    for (int i = 1; i <= n; i++)
    {
        cout << " " << a[i];
    }
    cout << "\nSink method: ";
    for (int i = 1; i <= n; i++)
    {
        cout << " " << b[i];
    }
    cout << "\nsorting: \n";
    heapSort(a, n);

    int c[20];
    int d[20];
    FillRandomArray(c, 20, -500, 500, false, UNSORTED);
    CopyArray(d, c, 20);
    cout << "\nbefore bubble sort\n";
    for (int i = 0; i < 20; i++)
    {
        cout << " " << c[i];
    }
    cout << '\n';
    for (int i = 0; i < 20; i++)
    {
        cout << " " << d[i];
    }
    cout << '\n';
    bubbleSort(c, 20);
    recBubbleSort(d, 20, 20);
    cout << "after bubble sort\n";
    for (int i = 0; i < 20; i++)
    {
        cout << " " << c[i];
    }
    cout << '\n';
    for (int i = 0; i < 20; i++)
    {
        cout << " " << d[i];
    }
}

void perf_heap(int order)
{
    int a[MAX_SIZE];
    int b[MAX_SIZE];

    for (int i = 0; i < TEST_SIZE; i++)
    {
        for (int n = STEP_SIZE; n <= MAX_SIZE; n += STEP_SIZE)
        {
            FillRandomArray(a, n, -5000, 5000, false, order);
            CopyArray(b, a, n);
            makeHeapSw(a, n);
            makeHeapSink(b, n);
        }
    }
    profiler.divideValues("Sink", TEST_SIZE);
    profiler.divideValues("Swim", TEST_SIZE);
    profiler.createGroup("SinkVsSwim", "Sink", "Swim");
}

void perf_all_heap()
{
    perf_heap(UNSORTED);
    //profiler.reset("Ascending");
    //perf(ASCENDING);
    profiler.reset("Descending-worst case");
    perf_heap(DESCENDING);
    profiler.showReport();
}

void perf_sort()
{
    int a[MAX_SIZE];
    int b[MAX_SIZE];

    for (int i = 0; i < TEST_SIZE; i++)
    {
        for (int n = STEP_SIZE; n <= 3000; n += STEP_SIZE)
        {
            FillRandomArray(a, n, -5000, 5000, false, UNSORTED);
            CopyArray(b, a, n);
            bubbleSort(a, n);
            recBubbleSort(b, n, n);
        }
    }
    profiler.divideValues("BubbleSort", TEST_SIZE);
    profiler.divideValues("RecBubbleSort", TEST_SIZE);
    profiler.createGroup("IterativeVsRecursive", "BubbleSort", "RecBubbleSort");

    profiler.showReport();
}

void perf_sort_timer()
{
    int a[MAX_SIZE];
    int b[MAX_SIZE];

    for (int i = 0; i < TEST_SIZE; i++)
    {
        for (int n = STEP_SIZE; n <= 3000; n += STEP_SIZE)
        {
            FillRandomArray(a, n, -5000, 5000, false, UNSORTED);
            CopyArray(b, a, n);
            profiler.startTimer("BubbleSort", n);
            for(int k = 0; k < 1000; k++)
                bubbleSort(a, n);
            profiler.stopTimer("BubbleSort", n);
            profiler.startTimer("RecBubbleSort", n);
            for(int k = 0; k < 1000; k++)
                recBubbleSort(b, n, n);
            profiler.stopTimer("RecBubbleSort", n);
        }
    }

    profiler.createGroup("IterativeVsRecursiveTimer", "BubbleSort", "RecBubbleSort");

    profiler.showReport();
}

int main()
{
    //demo();
    //perf_all_heap();
    //perf_sort();
    perf_sort_timer();
    return 0;
}
