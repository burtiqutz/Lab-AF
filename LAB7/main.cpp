#include <iostream>
#include <list>

using namespace std;

int parent[] = {-100, 2, 7, 5, 2, 7, 7, -1, 5, 2};

struct node2
{
    int key;
    list<node2 *> sons;
};

node2 *r2, *r2Const[500];

struct node3
{
    int key;
    node3 *left, *right;
};

node3 *r3;

void prettyPrintR1(int parent[], int n, int tabs, int indexParent)
{
    for(int i = 0 ; i < tabs; i++)
    {
        cout << '\t';
    }

    cout << indexParent << '\n';    //  index parent is the child

    for(int i = 1; i <= n; i++) //  daca vreau cu 0 schimb aici index de la 0
    {
        if(parent[i] == indexParent)
        {
            prettyPrintR1(parent, n, tabs + 1, i);
        }
    }
}

void prettyPrintR2(node2 *r2, int tabs)
{
    for(int i = 0 ; i < tabs; i++)
    {
        cout << '\t';
    }

    cout << r2->key << '\n';

    for(auto it : r2->sons)
    {
        prettyPrintR2(it, tabs + 1);
    }
}

void prettyPrintR3(node3 *r3, int tabs)
{
    if (r3 == nullptr)
    {
        return;
    }

    for(int i = 0 ; i < tabs; i++)
    {
        cout << '\t';
    }

    cout << r3->key << '\n';
    prettyPrintR3(r3->left, tabs + 1);
    prettyPrintR3(r3->right, tabs);
}

void transformToMultiway(node2 *r2[], int n)
{
    //  create all nodes (r2_const global),
}

void transformToBinaryTree()
{
    //  preorder multiway tree, create new nodes, first node i insert to the left, rest to the right
}

int main()
{
    prettyPrintR1(parent, 11, 0, 7);
    return 0;
}
