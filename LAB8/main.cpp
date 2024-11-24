#include <iostream>
#include "../profiler/Profiler.h"
Profiler profiler;
using std::cout, std::cin;

#define TEST_SIZE 5
#define MAX_SIZE 10000

// la build aleg mediana mereu (mijocul) si apelez recursiv pe stanga si dreapta
// la select rank = size.left + 1, si cautam apoi pe stanga sau pe dreapta cu size schimbat
// la delete apelez select, dar am nevoie si de parintele celui selectat pt a repara nodul;
// il inlocuiesc cu succesorul,
// aceeasi problema si acolo. pt size scad cate 1 pe path ul catre nodul sters


struct bst
{
    int key, size;
    bst *left, *right;
};

bst* createNode(int key, int size, Operation &op)
{
    op.count(3);
    bst* node = new bst;
    node->key = key;
    node->size = size;

    return node;
}

void prettyPrintR3(bst* r3, int tabs)
{
    if (r3 == nullptr)
    {
        return;
    }

    for (int i = 0; i < tabs; i++)
    {
        cout << '\t';
    }

    cout << r3->key << '\n';
    prettyPrintR3(r3->left, tabs + 1);
    prettyPrintR3(r3->right, tabs + 1);
}

bst* buildTree(int left, int right, int arr[], Operation &op)
{
    op.count();
    if (right < left)
    {
        return nullptr;
    }
    op.count();
    int mid = (left + right) / 2;

    op.count(3);
    auto node = createNode(arr[mid], right - left + 1, op);
    node->left = buildTree(left, mid - 1, arr, op);
    node->right = buildTree(mid + 1, right, arr, op);

    return node;
}

bst* OSSelect(bst* root, int rank, bst*& parent, Operation &op)
{
    if (root == nullptr) //  Empty tree
        return nullptr;

    int curRank = 1;
    op.count();
    if (root->left != nullptr)
    {
        curRank = root->left->size + 1;
    }
    op.count();
    if (rank == curRank)
    {
        return root;
    }
    else if (rank < curRank)
    {
        op.count(2);    //  1 count for 1 for parent = root
        parent = root;      //  For deletion
        return OSSelect(root->left, rank,parent, op);
    }
    else
    {
        op.count(2);
        parent = root;      //  For deletion
        return OSSelect(root->right, rank - curRank,parent, op);
    }
}

void repairSize(bst* root, bst* target, Operation &op) {
    //  All nodes on path besides target will have the sized decremented
    op.count();
    bst* current = root;
    while (current != nullptr) {
        op.count();
        current->size--;
        op.count();
        if (current->left == target || current->right == target)
            break;
        op.count();
        if (target->key < current->key)
        {
            op.count();
            current = current->left;
        }
        else
        {
            op.count();
            current = current->right;
        }

    }
}

void OSDelete(bst* &root, int rank, Operation &op)
{
    //  Root is passed as refference in case we need to delete it

    bst* parent = nullptr;
    bst* z = OSSelect(root, rank, parent, op); // Find the node to delete

    op.count();
    if (z == nullptr)
    {
        //  cout << "Node doesn't exist\n";         //  Greyed out for performance reasons
        return;
    }

    repairSize(root, z, op);

    // Case 1: No children
    op.count();
    if (z->left == nullptr && z->right == nullptr)
    {
        op.count();
        if (parent == nullptr) // Deleting the root with no children
        {
            op.count();
            root = nullptr;
        }
        else if (parent->left == z)
        {
            op.count(2);
            parent->left = nullptr;
        }
        else
        {
            op.count(2);
            parent->right = nullptr;
        }
        delete z;       //  Func createNode uses new, so we use delete, not free
    }
    // Case 2: One child
    else if (z->left == nullptr || z->right == nullptr)
    {
        op.count(2);
        bst* child = (z->left != nullptr) ? z->left : z->right;     //  Instead of having another if else
        op.count();
        if (parent == nullptr)  // Root
        {
            root = child;       // Becomes child
        }
        else if (parent->left == z)
        {
            op.count(2);
            parent->left = child;
        }
        else
        {
            op.count(2);
            parent->right = child;
        }
        delete z;
    }
    // Case 3: Two children
    else
    {
        op.count(2);
        bst* successorParent = z;       //  Needed because we need to delete the successor
        bst* successor = z->right;      //  Could've swapped bst* structure but this approach seems simpler to me

        // Find the successor (smallest node in the right subtree) & its parent
        while (successor->left != nullptr)
        {
            op.count(3);
            successorParent = successor;
            successor = successor->left;
        }

        // Replace z's key and size with successor's key and size
        op.count();
        z->key = successor->key;

        // Delete the successor
        // Successor can only have a right child, as it is the left-most node (in that subtree)
        if (successorParent->left == successor)
        {
            op.count(2);
            successorParent->left = successor->right;
        }
        else
        {
            op.count(2);
            successorParent->right = successor->right;
        }

        delete successor;
    }
}

void demo()
{
    int numbers[] = {3, 4, 7, 9, 10, 11, 13, 15, 20, 23, 24};
    Operation OP = profiler.createOperation("a", 0);        //  Dummy
    bst* root = buildTree(0, 10, numbers, OP);

    prettyPrintR3(root, 0);
    int rank = 5;
    bst* dummy = nullptr;

    bst* searched = OSSelect(root, rank, dummy, OP);
    cout << rank << "th element is " << searched->key << '\n';
    rank = 3;
    searched = OSSelect(root, rank, dummy, OP);
    cout << rank << "th element is " << searched->key << '\n';
    rank = 8;
    searched = OSSelect(root, rank, dummy, OP);
    cout << rank << "th element is " << searched->key << '\n';
    srand(time(0));

    rank = rand()%10 + 1;
    cout << "First delete\n";
    searched = OSSelect(root, rank, dummy, OP);
    cout <<"deleted element: " << searched->key << '\n';
    OSDelete(root, rank, OP);
    prettyPrintR3(root, 0);

    rank = rand()%9 + 1;
    cout << "Second delete\n";
    searched = OSSelect(root, rank, dummy, OP);
    cout <<"deleted element: " << searched->key << '\n';
    OSDelete(root, rank, OP);
    prettyPrintR3(root, 0);

    rank = rand()%8 + 1;
    cout << "Third delete\n";
    searched = OSSelect(root, rank, dummy, OP);
    cout <<"deleted element: " << searched->key << '\n';
    OSDelete(root, rank, OP);
    prettyPrintR3(root, 0);
}

void perf()
{
    for(int i = 0; i < TEST_SIZE; i++)
    {
        for (int n = 100; n <= MAX_SIZE; n+= 100)
        {
            Operation op1 = profiler.createOperation("build", n);
            Operation op2 = profiler.createOperation("select", n);
            Operation op3 = profiler.createOperation("delete", n);
            int aux[n];
            FillRandomArray(aux, n, 1, n, true, ASCENDING);
            bst* tree = buildTree(0, n - 1, aux, op1);
            for(int j = 0; j < n; j++)
            {
                bst* dummy;
                OSSelect(tree, rand() % n - j, dummy, op2);
                OSDelete(tree, rand() % n - j, op3);
            }
        }
    }

    profiler.divideValues("build", 5);
    profiler.divideValues("select", 5);
    profiler.divideValues("delete", 5);
    profiler.createGroup("comp", "build", "select", "delete");
    profiler.showReport();
}

int main()
{
    //demo();
    perf();
    return 0;
}
