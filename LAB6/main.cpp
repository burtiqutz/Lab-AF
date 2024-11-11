#include <iomanip>
#include <iostream>
#include "../profiler/Profiler.h"
using namespace std;

/*Iordache Alexandru grupa 30228
 *Pentru acest laborator am avut de implementat un hash table cu open addressing si
 *quadratic probing.
 */

#define HASH_TABLE_SIZE 9973
#define TEST_SIZE 5
#define SEARCH_COUNT 3000

int aux[30000];   // global value so i can pick values from it later on
int aux2[30000];
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
            //occupied and corresponding id
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
    int noElements = static_cast<int>(alpha * tableSize); //no of elements to insert in the table to acheive ~alpha
    srand(time(0));

    // generating all elements up to tablesize
    // basically filling aux with elements from 0 to table size, in a random order

    FillRandomArray(aux, tableSize, 0, 2 * tableSize, true, UNSORTED);
    for (int i = 0; i < noElements; i++)
    {
        Entry toInsert = {aux[i], UNOCCUPIED};
        hashInsert(T, tableSize, toInsert);
    }
}

void testSearch(Entry* T, int tableSize, int &maxAccessFound, int &maxAccessNotFound, float &avgFound, float &avgNotFound)
{
    int foundCount = 0;
    int notFoundCount = 0;
    int sumFound = 0;
    int sumNotFound = 0;

    maxAccessFound = 0;       // max accesses for found entries
    maxAccessNotFound = 0;    // max accesses for not-found entries
    FillRandomArray(aux, tableSize, 0, 2 * tableSize, true, UNSORTED);
    for (int i = 0; i < SEARCH_COUNT; i++)
    {
        int value = aux[i];

        Entry dummy = {value, UNOCCUPIED};

        int accessCount = 0;

        //  hashSearch returns the index where it found the entry, -1 if it didn't find it
        int found = hashSearch(T, tableSize, dummy, accessCount);
        if (found == -1)
        {
            // Not found case
            notFoundCount++;
            sumNotFound += accessCount; //  for calculating average accesses
            if (accessCount > maxAccessNotFound)
            {
                maxAccessNotFound = accessCount;    //  computing max access
            }
        }
        else
        {
            // Found case
            foundCount++;
            sumFound += accessCount;
            if (accessCount > maxAccessFound)
            {
                maxAccessFound = accessCount;
            }
        }
    }

    // avoid division by zero because cLion is crying
    avgFound = (foundCount > 0) ? static_cast<float>(sumFound) / foundCount : 0;
    avgNotFound = (notFoundCount > 0) ? static_cast<float>(sumNotFound) / notFoundCount : 0;
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
        <<"------------------------------------------------------------------\n";

    int avgMaxAccesFound[TEST_SIZE] = {0};
    int avgMaxAccesNotFound[TEST_SIZE] = {0};
    float avgNotFoundResults[TEST_SIZE] = {0.f};
    float avgFoundResults[TEST_SIZE] = {0.f};

    for (int i = 0; i < TEST_SIZE; i++)
    {
        for (auto alpha : loadFactors)
        {
            memset(T, 0, HASH_TABLE_SIZE * sizeof(Entry));  // reseting hash table
            insertToAlpha(T, HASH_TABLE_SIZE, alpha);   //  fill up to alpha load factor
            int maxAccess = 0;
            int minAccess = 0;
            float avg1 = 0.f;
            float avg2 = 0.f;
            testSearch(T, HASH_TABLE_SIZE, maxAccess, minAccess, avg1, avg2);

            avgMaxAccesFound[i] += maxAccess;    //  for load factor on pos i
            avgMaxAccesNotFound[i] += minAccess;
            avgNotFoundResults[i] += avg2;
            avgFoundResults[i] += avg1;

        }
    }

    for(int i = 0; i < TEST_SIZE; i++)
    {
        printf("%-18.2f", loadFactors[i]);
        cout << "||";
        printf("%-8.2f", avgFoundResults[i] / TEST_SIZE);
        cout<< "||";
        printf("%-8d", avgMaxAccesFound[i] / TEST_SIZE);
        cout << "||";
        printf("%-8.2f", avgNotFoundResults[i] / TEST_SIZE);
        cout<< "||";
        printf("%-8d", avgMaxAccesNotFound[i] / TEST_SIZE);
        cout << '\n';
    }
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
