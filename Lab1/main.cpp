#include <iostream>
#include "../profiler/Profiler.h"
using namespace std;
Profiler profiler;

void func(int n)
{
    Operation o = profiler.createOperation("O(N)", n);
        for(int i = 0; i < n; i++)
        {
            o.count();
            int a = 1 + 2;
        }
}

void func10n(int n)
{
    Operation o = profiler.createOperation("10*O(N)", n);
    for(int i = 0; i < n; i++)
    {
        o.count(10);
        int a = 1 + 2;
    }
}

void funcLog(int n)
{
    Operation o = profiler.createOperation("100 O(logN)", n);
    int a = n;
    for(int i = 0; i < 100; i++)
    {
        //a = n;
        while(a > 0)
        {
            a /= 2;
            o.count();
        }
    }
}

void funcNLog(int n)
{
    Operation o = profiler.createOperation("n*O(logN)", n);
    int a = n;
    for(int i = 0; i < n; i++)
    {
        //a = n;
        while(a > 0)
        {
            a /= 2;
            o.count();
        }
    }
}

void funcTenthSquared(int n)
{
    Operation o = profiler.createOperation("0.1 O(N^2)", n);
    for(int i = 0; i < n; i++)
    {
        for(int j = 0; j < n; j++)
        {
            if(i % 10 == 0)//sau j
            {
                o.count();
            }
        }
    }
}

void funcHundrethCubic(int n)
{
    Operation o = profiler.createOperation("0.01 O(N^3)", n);
    for(long long int i = 0; i < n; i++)
    {
        for(long long int j = 0; j < n; j++)
        {
            for(long long int k = 0; k < n; k++)
            {
                if(i % 100 == 0)//sau j, k
                {
                    o.count();
                }
            }
        }
    }
}
int main()
{
    for(int i = 100; i <= 10000; i+= 100)
    {
        func(i);
        func10n(i);
        funcLog(i);
        funcNLog(i);
        funcTenthSquared(i);
        funcHundrethCubic(i);
    }
    profiler.showReport();

    return 0;
}
