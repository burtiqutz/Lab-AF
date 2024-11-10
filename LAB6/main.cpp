#include <iostream>
#include "../profiler/Profiler.h"
using namespace std;
Profiler profiler;

/*Iordache Alexandru grupa 30228
 *Pentru acest laborator am avut de implementat un hash table cu open addressing si
 *quadratic probing.
 */

enum
{
    UNOCCUPIED,
    OCCUPIED
};

typedef struct
{
    int id;
    int state;  //occupied or not
}Entry;

void printHashTable(Entry *T, int tableSize)
{
    for(int i = 0; i < tableSize; ++i)
    {
        cout << "id: " << T[i].id <<'\n';
    }
}

int hashFunc(int key, int index, int tableSize)
{
    return (key % tableSize + index + index * index) % tableSize;
}

int hashInsert(Entry *T, int tableSize , Entry toInsert)
{
    int i = 0;
    while(i != tableSize)
    {
        int j = hashFunc(toInsert.id, i, tableSize);
        if(T[j].state == UNOCCUPIED)
        {
            T[j].id = toInsert.id;
            T[j].state = OCCUPIED;
            return j;
        } else
        {
            i++;
        }
    }
    return -1;
}

int hashSearch(Entry *T, int tableSize, Entry searched)
{
    int i = 0;
    while(i != tableSize)
    {
        int j = hashFunc(searched.id, i, tableSize);
        if(T[j].state == UNOCCUPIED)
        {
            return -1;
        } else if(T[j].id == searched.id)
        {
            return j;
        }
    }
    return -1;
}

void demo()
{
    Entry T[10];
    int aux[10];

    for(int i = 0; i < 10; i++)
    {

    }
}

int main()
{

    return 0;
}
