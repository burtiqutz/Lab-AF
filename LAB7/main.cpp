#include <iostream>
#include <list>
#include "../profiler/Profiler.h"
Profiler p;

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

node3* createNode3(int key)
{
    node3 *newNode = (node3*) malloc(sizeof(node3));
    newNode->key = key;
    newNode->left = nullptr;
    newNode->right = nullptr;

    return newNode;
}

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

void recursivePostOrder(node3 *root, Operation &op)
{
    if(root == nullptr)
        return;

    recursivePostOrder(root->left, op);
    recursivePostOrder(root->right, op);
    op.count();
    cout << ' ' << root->key;
}

void recursiveInOrder(node3 *root, Operation &op)
{
    if(root == nullptr)
        return;

    recursiveInOrder(root->left, op);
    op.count();
    cout << ' ' << root->key;
    recursiveInOrder(root->right, op);
}

void recursivePreOrder(node3 *root, Operation &op)
{
    if(root == nullptr)
        return;

    op.count();
    cout << ' ' << root->key;
    recursivePreOrder(root->left, op);
    recursivePreOrder(root->right, op);
}

void iterativePrint(int d, node3 *root){
    node3 *node = root;
    do
    {
        if(d == 1)
        {
            if(node->left != nullptr)
            {
                node = node->left;
            } else
            {
                d = 2;
            }
        }
        if(d == 2)
        {
            if(node->right != nullptr)
            {
                node = node->right;
            }else
            {
                d = 3;
            }
        }
        if( d == 3)
        {

        }
    }while(node == root && d == 3);
}

void insertNode(node3 **root, int key)
{
    node3 *node = createNode3(key);
    if(*root == nullptr)
    {
        *root = node;
    } else
    {
        node3 *p = nullptr;
        node3 *q = *root;
        while(q != nullptr)
        {
            p = q;
            if(key < q->key)
            {
                q = q->left;
            } else
            {
                q = q->right;
            }
        }
        if(key < p->key)
        {
            p->left = node;
        } else
        {
            p->right = node;
        }

    }
}

void demo()
{
    int aux[] = {25, 10, 32, 17, 7, 61, 11, 20, 36, 5};
    int n = sizeof(aux) / sizeof(int);
    Operation dummy = p.createOperation("dummy", 0);

    node3 *tree = nullptr;
    for(int i = 0; i < n; ++i)
    {
        insertNode(&tree, aux[i]);
    }

    recursivePreOrder(tree, dummy);
    cout << endl;
    recursiveInOrder(tree, dummy);
    cout << endl;
    recursivePostOrder(tree, dummy);
    cout << endl;
    iterativePrint(1, tree);
}

void perf()
{

}

int main()
{
    demo();
    return 0;
}
