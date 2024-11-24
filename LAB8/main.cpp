#include <iostream>
#include <pstl/execution_defs.h>

using std::cout, std::cin;

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

bst* createNode(int key, int size)
{
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

bst* buildTree(int left, int right, int arr[])
{
    if (right < left)
    {
        return nullptr;
    }
    int mid = (left + right) / 2;

    auto node = createNode(arr[mid], right - left + 1);
    node->left = buildTree(left, mid - 1, arr);
    node->right = buildTree(mid + 1, right, arr);

    return node;
}

bst* OSSelect(bst* root, int rank, bst*& parent)
{
    if (root == nullptr) //  Empty tree
        return nullptr;

    int curRank = 1;
    if (root->left != nullptr)
    {
        curRank = root->left->size + 1;
    }

    if (rank == curRank)
    {
        return root;
    }
    else if (rank < curRank)
    {
        parent = root;      //  For deletion
        return OSSelect(root->left, rank,parent);
    }
    else
    {
        parent = root;      //  For deletion
        return OSSelect(root->right, rank - curRank,parent);
    }
}

void repairSize(bst* root, bst* target) {
    //  All nodes on path besides target will have the sized decremented (asta e cuvantul?:)))
    bst* current = root;
    while (current != nullptr) {
        current->size--;
        if (current->left == target || current->right == target)
            break;
        if (target->key < current->key)
            current = current->left;
        else
            current = current->right;
    }
}

void OSDelete(bst* &root, int rank)
{
    //  Root is passed as refference in case we need to delete it

    bst* parent = nullptr;
    bst* z = OSSelect(root, rank, parent); // Find the node to delete

    if (z == nullptr)
    {
        cout << "Node doesn't exist\n";
        return;
    }

    repairSize(root, z);

    // Case 1: No children
    if (z->left == nullptr && z->right == nullptr)
    {
        if (parent == nullptr) // Deleting the root with no children
        {
            root = nullptr;
        }
        else if (parent->left == z)
        {
            parent->left = nullptr;
        }
        else
        {
            parent->right = nullptr;
        }
        delete z;       //  Func createNode uses new, so we use delete, not free
    }
    // Case 2: One child
    else if (z->left == nullptr || z->right == nullptr)
    {
        bst* child = (z->left != nullptr) ? z->left : z->right;     //  Instead of having another if else
        if (parent == nullptr) // Deleting the root with one child
        {
            root = child;
        }
        else if (parent->left == z)
        {
            parent->left = child;
        }
        else
        {
            parent->right = child;
        }
        delete z;
    }
    // Case 3: Two children
    else
    {
        bst* successorParent = z;       //  Needed because we need to delete the successor
        bst* successor = z->right;      //  Could've swapped bst* structure but this approach seems simpler to me

        // Find the successor (smallest node in the right subtree) & its parent
        while (successor->left != nullptr)
        {
            successorParent = successor;
            successor = successor->left;
        }

        // Replace z's key and size with successor's key and size
        z->key = successor->key;

        // Delete the successor
        // Successor can only have a right child, as it is the left-most node (in that subtree)
        if (successorParent->left == successor)
        {
            successorParent->left = successor->right;
        }
        else
        {
            successorParent->right = successor->right;
        }

        delete successor;
    }
}

void demo()
{
    int numbers[] = {3, 4, 7, 9, 10, 11, 13, 15, 20, 23, 24};
    bst* root = buildTree(0, 10, numbers);
    prettyPrintR3(root, 0);
    int rank = 5;
    bst* dummy = nullptr;
    bst* searched = OSSelect(root, rank, dummy);
    cout << rank << "th element is " << searched->key << '\n';
    rank = 3;
    searched = OSSelect(root, rank, dummy);
    cout << rank << "th element is " << searched->key << '\n';
    rank = 8;
    searched = OSSelect(root, rank, dummy);
    cout << rank << "th element is " << searched->key << '\n';

    OSDelete(root, 6);  //  11 (ROOT)
    cout << "First delete\n";
    prettyPrintR3(root, 0);

    OSDelete(root, 2);  //  4 (for new tree)
    cout << "Second delete\n";
    prettyPrintR3(root, 0);

    OSDelete(root, 8);  //  23 (for new tree)
    cout << "Third delete\n";
    prettyPrintR3(root, 0);
}

int main()
{
    demo();
    return 0;
}
