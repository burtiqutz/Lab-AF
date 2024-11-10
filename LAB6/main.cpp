#include <iomanip>
#include <iostream>
#include "../profiler/Profiler.h"
using namespace std;
Profiler profiler;

/*Iordache Alexandru grupa 30228
 *Pentru acest laborator am avut de implementat un hash table cu open addressing si
 *quadratic probing.
 */

#define HASH_TABLE_SIZE 9973
#define TEST_SIZE 5
#define SEARCH_COUNT 3000

enum
{
    UNOCCUPIED,
    OCCUPIED
};

typedef struct
{
    int id;
    int state; //occupied or not
} Entry;

void printHashTable(Entry* T, int tableSize)
{
    for (int i = 0; i < tableSize; ++i)
    {
        cout << "id: ";
        if(T[i].state == OCCUPIED)
            cout  << T[i].id << '\n';
        else
            cout  << '-' << '\n';
    }
}

int hashFunc(int key, int index, int tableSize)
{
    return (key % tableSize + 3 * index + 7 * index * index) % tableSize;
}

int hashInsert(Entry* T, int tableSize, Entry toInsert)
{
    int i = 0;
    while (i != tableSize)
    {
        int j = hashFunc(toInsert.id, i, tableSize);
        if (T[j].state == UNOCCUPIED)
        {
            T[j].id = toInsert.id;
            T[j].state = OCCUPIED;
            return j;
        }
        else
        {
            i++;
        }
    }
    return -1;
}

int hashSearch(Entry* T, int tableSize, Entry searched, int& foundAfter)
{
    int i = 0;
    while (i != tableSize)
    {
        int j = hashFunc(searched.id, i, tableSize);
        if (T[j].state == UNOCCUPIED)
        {
            //reached a spot where searched should be
            foundAfter = i;
            return -1;
        }
        else if (T[j].id == searched.id)
        {
            //found searched
            //remember how many acceses we needed
            foundAfter = i;
            return j;
        }
        i++;
    }

    //got to end of table and didn't find it
    foundAfter = i;
    return -1;
}

void hashDelete(Entry* T, int tableSize, Entry toDelete)
{
    int indexDeleted = -1;
    int dummy = 0;
    indexDeleted = hashSearch(T, tableSize, toDelete, dummy);
    if(indexDeleted != -1)
    {
        T[indexDeleted].state = UNOCCUPIED;
    }
}

void insertToAlpha(Entry* T, int tableSize, double alpha)
{
    int noElements = (int)(alpha * tableSize); //no of elements to insert in the table to acheive ~alpha
    int count = 0;
    srand(time(nullptr));

    //generating all elements up to 2 * tablesize
    int aux[2 * tableSize] = {0};
    FillRandomArray(aux, 2 * tableSize, 0, 2 * tableSize, true, UNSORTED);
    int x = 0;
    while (count < noElements)
    {
        do
        {
            //choose a random number until we find one that hasn't been inserted yet
            Entry toInsert;
            toInsert.id = aux[rand() % (2 * tableSize)];
            toInsert.state = UNOCCUPIED;
            x = hashInsert(T, tableSize, toInsert);
        }
        while (x != -1);
        count++;
    }
}

void testSearch(Entry* T, int tableSize, int &maxAcces, int &minAcces, float &avgFound, float &avgNotFound)
{
    int foundCount = 0;
    int notFoundCount = 0;
    int sumNotFound = 0;
    int sumFound = 0;

    for (int i = 0; i < SEARCH_COUNT; i++)
    {
        int value = rand() % (2 * tableSize);
        Entry dummy;
        dummy.id = value;
        dummy.state = UNOCCUPIED;
        int index = -1;

        int found = hashSearch(T, tableSize, dummy, index);
        if(found == -1)
        {
            notFoundCount++;
        } else
        {
            foundCount++;
        }

        if (index != -1)
        {
            if (index > maxAcces)
            {
                maxAcces = index;
            }
            sumFound += index;
        } else
        {
            if (index > minAcces)
            {
                minAcces = index;
            }
            sumNotFound += index;
        }
    }
    avgFound = (float)( sumFound /  foundCount);
    avgNotFound = (float) (sumNotFound / notFoundCount);
}

void perf()
{
    auto* T = (Entry*)calloc(HASH_TABLE_SIZE, sizeof(Entry));
    if (T == nullptr)
    {
        perror("couldnt alloc T");
        exit(-1);
    }
    double loadFactors[] = {0.80, 0.85, 0.90, 0.95, 0.99};

    cout << "Factor de umplere" << " || " << "Elemente gasite " << " || " << "Elemente negasite" <<'\n'
        <<  "                 " << " || " << "avg   || max    " << " || " << "avg    || max    " << '\n'
        <<"-------------------------------------------------------------------------------------------\n";

    //for (int i = 0; i < TEST_SIZE; i++)
    //{
        for (auto alpha : loadFactors)
        {
            insertToAlpha(T, HASH_TABLE_SIZE, alpha);
            int maxAcces = 0;
            int minAcces = 0;
            float avg1 = 0.f;
            float avg2 = 0.f;

            testSearch(T, HASH_TABLE_SIZE, maxAcces, minAcces, avg1, avg2);
            cout << alpha << "             " << " || " << avg1 << "      " << maxAcces << "   " << avg2 << "      " << minAcces << "   " << '\n';
        }
    //}

    free(T);
}

void demo()
{
    auto* T = (Entry*)calloc(11, sizeof(Entry));
    if (T == nullptr)
    {
        perror("couldnt alloc T");
        exit(-1);
    }
    Entry toInsert;
    toInsert.state = UNOCCUPIED;
    toInsert.id = 5;
    hashInsert(T, 11, toInsert);
    toInsert.id = 1;
    hashInsert(T, 11, toInsert);
    toInsert.id = 20;
    hashInsert(T, 11, toInsert);
    toInsert.id = 17;
    hashInsert(T, 11, toInsert);
    toInsert.id = 15;
    hashInsert(T, 11, toInsert);
    toInsert.id = 31;
    hashInsert(T, 11, toInsert);
    toInsert.id = 11;
    hashInsert(T, 11, toInsert);
    toInsert.id = 112;
    hashInsert(T, 11, toInsert);
    //fill factor 8 / 11 = 0.72
    printHashTable(T, 11);

    Entry toSearch;
    toSearch.state = UNOCCUPIED;
    toSearch.id = 11;
    int dummy;
    cout << "searching for 11, pos: " <<
        hashSearch(T, 11, toSearch, dummy) << '\n';
    toSearch.id = 30;
    cout << "searching for 30, pos: " <<
        hashSearch(T, 11, toSearch, dummy) << '\n';
    toSearch.id = 112;
    cout << "searching for 112, pos: " <<
        hashSearch(T, 11, toSearch, dummy) << '\n';
    toSearch.id = 500;
    cout << "searching for 500, pos: " <<
        hashSearch(T, 11, toSearch, dummy) << '\n';

    Entry toDelete;
    toDelete.state = UNOCCUPIED;

    toDelete.id = 15;
    hashDelete(T, 11, toDelete);
    toDelete.id = 6;
    hashDelete(T, 11, toDelete);
    toDelete.id = 112;
    hashDelete(T, 11, toDelete);
    printHashTable(T, 11);


    free(T);
}

int main()
{
    //demo();
    perf();
    return 0;
}
