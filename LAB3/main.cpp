#include <iostream>
#include "../profiler/Profiler.h"

#define MAX_SIZE 10000
#define STEP_SIZE 100
#define TEST_SIZE 5

using namespace std;

Profiler profiler("Average");

void swim(int a[], int n, int index)
{
    while (index > 1 && a[index] < a[index / 2])
    {
        profiler.countOperation("Swim", n, 5);//2 comparisions and a swap
        swap(a[index], a[index / 2]);
        index /= 2;
    }
}

void makeHeapSw(int a[], int n)
{
    Operation swimCount = profiler.createOperation("Swim", n);
    for (int i = 2; i <= n; ++i)
    {
        swim(a, n, i);
    }
}

void sink(int a[], int n, int index)
{
    while (2 * index <= n) //check only internal nodes
    {
        profiler.countOperation("Sink", n, 1);//comaparision
        int indChild = 2 * index;
        //find smallest child
        profiler.countOperation("Sink", n, 2);//if statement
        if (indChild + 1 <= n && a[indChild + 1] < a[indChild])
        {
            indChild++;
        }
        //swap if there is a smaller one
        profiler.countOperation("Sink", n, 1);
        if (a[indChild] < a[index])
        {
            profiler.countOperation("Sink", n, 3);;//swap
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
    cout <<" "<< a[1];
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
    cout<<"Swim method:";
    for (int i = 1; i <= n; i++)
    {
        cout << " " << a[i];
    }
    cout<<"\nSink method: ";
    for (int i = 1; i <= n; i++)
    {
        cout << " " << b[i];
    }
    cout << "\nsorting: \n";
    heapSort(a, n);
}

void perf(int order)
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
    profiler.showReport();
}

void perf_all()
{
    perf(UNSORTED);
    profiler.reset("Worst case");
    perf(ASCENDING);
}

int main()
{
    //demo();
    perf_all();
    return 0;
}
