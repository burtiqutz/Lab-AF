#include <iomanip>
#include <iostream>
#include "../profiler/Profiler.h"
using namespace std;

/*Iordache Alexandru grupa 30228
 *Pentru acest laborator am avut de implementat un hash table cu open addressing si
 *quadratic probing.
 *Fiecare element poate avea unul din trei statusuri, neocupat, ocupat, sters.
 *Motivarea este pentru functia de search: cand stergi un element, daca a avut loc o coliziune pe acel element,
 *mai pot fi elemente dupa acesta pe acelasi "i". Daca nu am avea status de deleted, ne am opri la acel element, desi
 *elementul cautat ar putea fi dupa acesta, la un i urmator. Astfel, la cautare doar sarim peste elementele deleted.
 *Am testat load factors de la 0.8 la 0.99 si se poate observa cum efortul creste semnificativ pentru un load factor
 *mare, in special in cazurile de max accesari.
 *In cazul in care stergem elemente, performanta tabelei de dispersie scade, intrucat fata de tabela echivalenta in
 *care inseram aceleasi elemente din prima, unele elemente nu vor fi pe prima lor pozitie, ci la iteratii ulterioare
 *de i pt search.
 */

#define HASH_TABLE_SIZE 9973
#define TEST_SIZE 5
#define SEARCH_COUNT 3000

enum
{
    UNOCCUPIED,
    OCCUPIED,
    DELETED
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
        if (T[j].state == UNOCCUPIED || T[j].state == DELETED)
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
            //  reached a spot where searched should be
            foundAfter = i;
            return -1;
        }
        else if (T[j].id == searched.id && T[j].state != DELETED)
        {
            //  occupied and corresponding id
            //  found searched
            //  remember how many acceses we needed
            foundAfter = i;
            return j;
        }
        i++;
    }

    //  got to end of table and didn't find it
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
        T[indexDeleted].state = DELETED;

    }

}

void insertToAlpha(Entry* T, int tableSize, double alpha)
{
    int noElements = static_cast<int>(alpha * tableSize); //no of elements to insert in the table to acheive ~alpha
    srand(time(0));

    // generating all elements up to tablesize
    // basically filling aux with elements from 0 to table size, in a random order


    for (int i = 0; i < noElements; i++)
    {
        Entry toInsert = {rand(), UNOCCUPIED};
        hashInsert(T, tableSize, toInsert);
    }
}

void testSearch(Entry* T, int tableSize, int &maxAccessFound, int &maxAccessNotFound, float &avgFound, float &avgNotFound)
{
    int foundCount = 0;
    int notFoundCount = 0;
    int sumFound = 0;
    int sumNotFound = 0;

    maxAccessFound = -1;       // max accesses for found entries
    maxAccessNotFound = -1;    // max accesses for not-found entries

    for (int i = 0; i < SEARCH_COUNT; i++)
    {
        int value = -1 ;
        if(i < SEARCH_COUNT / 2)
        {
            value = T[rand() % tableSize].id;   //  from table
        } else
        {
            value = T[1].id;
            Entry search = {value, UNOCCUPIED};
            int test = 0;   //  not using this
            while(hashSearch(T, tableSize, search, test) != -1)
            {
                //  generating an element until it is not part of the array
                value = rand();
                search.id = value;
            }
        }
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

void lowerLoadFactor(Entry* T, int tableSize, double newLoadFactor, double oldLoadFactor) {
    int newNoElem = static_cast<int>(newLoadFactor * tableSize);
    int oldNoElem = static_cast<int>(oldLoadFactor * tableSize);

    int countRemoved = 0;
    while (countRemoved < (oldNoElem - newNoElem)) {
        int index = rand() % tableSize;
        Entry deleted = {T[index].id, UNOCCUPIED};
        if(T[index].state == OCCUPIED)
        {
            hashDelete(T, tableSize, deleted);
            countRemoved++;
        }
    }
}

void perf()
{
    auto* T = (Entry*)calloc(HASH_TABLE_SIZE, sizeof(Entry));
    if (T == nullptr)
    {
        perror("couldn't allocate T");
        exit(-1);
    }

    double loadFactors[] = {0.80, 0.85, 0.90, 0.95, 0.99};

    cout << "Factor de umplere" << " || " << "Elemente gasite " << " || " << "Elemente negasite" << '\n'
         << "                 " << " || " << "avg   || max    " << " || " << "avg    || max    " << '\n'
         << "------------------------------------------------------------------\n";

    for (auto alpha : loadFactors)
    {
        int totalMaxAccessFound = 0;
        int totalMaxAccessNotFound = 0;
        float totalAvgFound = 0.f;
        float totalAvgNotFound = 0.f;

        for (int i = 0; i < TEST_SIZE; i++)
        {
            memset(T, 0, HASH_TABLE_SIZE * sizeof(Entry));  // Resetting hash table
            insertToAlpha(T, HASH_TABLE_SIZE, alpha);       // Fill up to alpha load factor

            int maxAccessFound = 0, maxAccessNotFound = 0;
            float avgFound = 0.f, avgNotFound = 0.f;

            testSearch(T, HASH_TABLE_SIZE, maxAccessFound, maxAccessNotFound, avgFound, avgNotFound);

            totalMaxAccessFound += maxAccessFound;
            totalMaxAccessNotFound += maxAccessNotFound;
            totalAvgFound += avgFound;
            totalAvgNotFound += avgNotFound;
        }

        printf("%-18.2f || %-8.2f || %-8d || %-8.2f || %-8d\n",
               alpha,
               totalAvgFound / TEST_SIZE,
               totalMaxAccessFound / TEST_SIZE,
               totalAvgNotFound / TEST_SIZE,
               totalMaxAccessNotFound / TEST_SIZE);
    }

    //  Repeat steps for deletion
    int totalMaxAccessFound = 0;
    int totalMaxAccessNotFound = 0;
    float totalAvgFound = 0.f;
    float totalAvgNotFound = 0.f;
    // Test lowering from 0.99 to 0.80
    for (int i = 0; i < TEST_SIZE; i++)
    {
        memset(T, 0, HASH_TABLE_SIZE * sizeof(Entry));  // Resetting hash table

        insertToAlpha(T, HASH_TABLE_SIZE, 0.99);

        lowerLoadFactor(T, HASH_TABLE_SIZE, 0.80, 0.99);

        int maxAccessFound = 0, maxAccessNotFound = 0;
        float avgFound = 0.f, avgNotFound = 0.f;

        testSearch(T, HASH_TABLE_SIZE, maxAccessFound, maxAccessNotFound, avgFound, avgNotFound);

        totalMaxAccessFound += maxAccessFound;
        totalMaxAccessNotFound += maxAccessNotFound;
        totalAvgFound += avgFound;
        totalAvgNotFound += avgNotFound;
    }

    printf("deleted            || %-8.2f || %-8d || %-8.2f || %-8d\n",
           totalAvgFound / TEST_SIZE,
           totalMaxAccessFound / TEST_SIZE,
           totalAvgNotFound / TEST_SIZE,
           totalMaxAccessNotFound / TEST_SIZE);

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
