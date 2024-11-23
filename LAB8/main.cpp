#include <iostream>

using std::cout, std::cin;

// la build aleg mediana mereu (mijocul) si apelez recursiv pe stanga si dreapta
// la select rank = size.left + 1, si cautam apoi pe stanga sau pe dreapta cu size schimbat
// la delete apelez select, dar am nevoie si de parintele celui selectat pt a repara nodul; il inlocuiesc cu succesorul,
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

bst* OSSelect(bst* root, int rank)
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
        return OSSelect(root->left, rank);
    }
    else
    {
        return OSSelect(root->right, rank - curRank);
    }
}



void demo()
{
    int numbers[] = {3, 4, 7, 9, 10, 11, 13, 15, 20, 23, 24};
    bst* root = buildTree(0, 10, numbers);
    prettyPrintR3(root, 0);
    int rank = 5;
    bst* searched = OSSelect(root, rank);
    cout << rank << "th element is " << searched->key << '\n';
    rank = 3;
    searched = OSSelect(root, rank);
    cout << rank << "th element is " << searched->key << '\n';
    rank = 8;
    searched = OSSelect(root, rank);
    cout << rank << "th element is " << searched->key << '\n';
}

int main()
{
    demo();
    return 0;
}
