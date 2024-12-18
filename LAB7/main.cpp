#include <iostream>
#include <list>
#include "../profiler/Profiler.h"
Profiler p;

/*
 *IORDACHE ALEXANDRU GRUPA 30228
 *Pentru aceasta tema am avut de implementat tree walk in mod recursiv si iterativ, si transformarile de la
 *arbore multicai cu vector de parinti la arbore multicai cu noduri si apoi la arbore binar.
 *Pentru tree walk recursiv am folosit implementarile standard de la cursul de SDA, iar pentru varianta iterativa am
 *folosit implementarea din curs. Toate variantele au exact aceeasi complexitate, O(n), cum se vede pe grafic.
 *Pentru a face prima transformare in O(n) am parcurs vectorul de parinti o singura data. Pe masura ce inaintam in
 *vector, daca nodul la care eram nu exista, il cream in memorie si, de asemenea, daca parintele acestuia nu exista,
 *il cream si pe el, apoi faceam legatura intre parinte si copil la pozitia corecta. In structura de node2 avem un array
 *de fii, in mod obisnuit as fi folosit o lista din c++ dar aveam probleme de implementare. As putea adauga un index in
 *plus in structura pentru pozitia ultimului fiu inserat, ceea ce ar face link ul dintre parinte si copil in o(1), dar
 *implementarea actuala este liniara (parcurge liniar vectorul de fii) si duce tot la implementare liniara in final.
 *Pentru cea de-a doua transformare, cream noul tip de nod adaptat pentru arborele binar. Daca ultimul frate exista, il
 *legam pe acesta la nodul curent. Apoi parcurgem liniar lista de fii ai nodului curent, pe primul fiu legandu-l la
 *stanga nodului curent.
 **/


using namespace std;

int parent[] = {-100, 2, 7, 5, 2, 7, 7, -1, 5, 2};

struct node2
{
    int key;
    //list<node2*> sons;
    node2 *sons[50];    //  max 50 fii
};

node2 *r2Const[500]; //  Pentru transformarea 1

struct node3
{
    int key;
    node3 *left, *right, *parent;
};

node3* r3 = (node3*)calloc(1, sizeof(node3)); //  pentru transformarea 2

node3* createNode3(int key)
{
    node3* newNode = (node3*)malloc(sizeof(node3));
    newNode->key = key;
    newNode->left = nullptr;
    newNode->right = nullptr;
    newNode->parent = nullptr;

    return newNode;
}

void prettyPrintR1(int parent[], int n, int tabs, int indexParent)
{
    for (int i = 0; i < tabs; i++)
    {
        cout << '\t';
    }

    cout << indexParent << '\n'; //  index parent is the child

    for (int i = 1; i <= n; i++) //  daca vreau cu 0 schimb aici index de la 0
    {
        if (parent[i] == indexParent)
        {
            prettyPrintR1(parent, n, tabs + 1, i);
        }
    }
}

void prettyPrintR2(node2* r2, int tabs)
{
    for (int i = 0; i < tabs; i++)
    {
        cout << '\t';
    }

    cout << r2->key << '\n';

    for (auto it : r2->sons)
    {
        if(it != nullptr)
            prettyPrintR2(it, tabs + 1);
    }
}

void prettyPrintR3(node3* r3, int tabs)
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
    prettyPrintR3(r3->right, tabs);
}

node2* transformToMultiway(int parent[], int n)
{
    node2* root = nullptr;

    // Create nodes and link children
    for (int i = 1; i < n; ++i)    //  Index from 1 to n because we don't use key 0
    {
        // Create the node for the current index if it doesn't exist
        if (r2Const[i] == nullptr)
        {
            r2Const[i] = static_cast<node2*>(calloc(1, sizeof(node2)));
            r2Const[i]->key = i;
        }

        // If the node has a parent
        if (parent[i] != -1)
        {
            // Create the parent node if it doesn't exist
            if (r2Const[parent[i]] == nullptr)
            {
                r2Const[parent[i]] = static_cast<node2*>(calloc(1, sizeof(node2)));
                r2Const[parent[i]]->key = parent[i];
            }

            // Add current node as a son of its parent
            int k = 0;
            while(r2Const[parent[i]]->sons[k] != nullptr)
            {
                k++;
            }
            r2Const[parent[i]]->sons[k] = r2Const[i];
            //sons.push_back(r2Const[i]);
        }
        else
        {
            // Else this is the root
            root = r2Const[i];
        }
    }
    return root;
}


node3* transformToBinaryTree(node2* root2, node3* leftBrother)
{
    node3* root3 = static_cast<node3*> (calloc(1, sizeof(node3)));
    root3->key = root2->key;
    if(leftBrother != nullptr)
    {
        leftBrother->right = root3;
    }
    node3* lastChild = nullptr;

    for(int i = 0; i < 50 && root2->sons[i] != nullptr; i++)
    {

        lastChild = transformToBinaryTree(root2->sons[i], lastChild);
        if(i == 0) //  If first child
        {
            root3->left = lastChild;
        }
    }
    return root3;
}

void recursivePostOrder(node3* root, Operation& op, bool print)
{
    if (root == nullptr)
        return;

    recursivePostOrder(root->left, op, print);
    recursivePostOrder(root->right, op, print);
    op.count();
    if (print) //  For performance reasons I will not always print
    {
        cout << " " << root->key;
    }
}

void recursiveInOrder(node3* root, Operation& op, bool print)
{
    if (root == nullptr)
        return;

    recursiveInOrder(root->left, op, print);
    op.count();
    if (print) //  For performance reasons I will not always print
    {
        cout << " " << root->key;
    }
    recursiveInOrder(root->right, op, print);
}

void recursivePreOrder(node3* root, Operation& op, bool print)
{
    if (root == nullptr)
        return;

    op.count();
    if (print) //  For performance reasons I will not always print
    {
        cout << " " << root->key;
    }
    recursivePreOrder(root->left, op, print);
    recursivePreOrder(root->right, op, print);
}

void walkIterative(node3* root, Operation& operation, bool print)
{
    // Pseudo code lecture 6
    if (root == nullptr) return; // Edge case: empty tree

    node3* node = root;
    int d = 1;

    do
    {
        if (d == 1)
        {
            //  Preorder
            //  Print here for pre-order

            if (node->left != nullptr)
            {
                node = node->left; // Advance to the left
            }
            else
            {
                d = 2;
            }
        }
        if (d == 2)
        {
            // In-order
            //  Print here for in-order
            operation.count();
            if (print) //  For performance reasons I will not always print
            {
                cout << " " << node->key;
            }
            if (node->right != nullptr)
            {
                node = node->right;
                d = 1;
            }
            else
            {
                d = 3;
            }
        }
        if (d == 3)
        {
            // Post-order
            //  Print here for post-order
            if (node->parent != nullptr)
            {
                if (node == node->parent->left)
                {
                    d = 2;
                }
                node = node->parent;
            }
        }
    }
    while (!(node == root && d == 3)); // Stop when back at root in post-order
}

void insertNode(node3** root, int key)
{
    node3* node = createNode3(key);
    if (*root == nullptr)
    {
        *root = node;
        node->parent = nullptr;
    }
    else
    {
        node3* p = nullptr;
        node3* q = *root;
        while (q != nullptr)
        {
            p = q;
            if (key < q->key)
            {
                q = q->left;
            }
            else
            {
                q = q->right;
            }
        }
        if (key < p->key)
        {
            p->left = node;
        }
        else
        {
            p->right = node;
        }
        node->parent = p;
    }
}

void demo()
{
    int aux[] = {25, 10, 32, 17, 7, 61, 11, 20, 36, 5};
    int n = sizeof(aux) / sizeof(int);
    Operation dummy = p.createOperation("dummy", 0);

    node3* tree = nullptr;
    for (int i = 0; i < n; ++i)
    {
        insertNode(&tree, aux[i]);
    }
    cout << "preorder recursive:\n";
    recursivePreOrder(tree, dummy, true);
    cout << endl << "inorder recursive:\n";
    recursiveInOrder(tree, dummy, true);
    cout << endl << "postorder recursive:\n";
    recursivePostOrder(tree, dummy, true);
    cout << endl << "iterative walk:\n";
    walkIterative(tree, dummy, true);
}

void perf()
{
    int aux[10001];
    FillRandomArray(aux, 10001, 1, 50000, true, UNSORTED);
    int auxIndex = 0;


    node3* tree = nullptr;
    for (int i = 100; i < 10000; i += 100)
    {
        //  Add 100 more nodes
        for (int index = 0; index < 100; index++)
        {
            insertNode(&tree, auxIndex);
            auxIndex++;
        }
        Operation op1 = p.createOperation("RecPreOrder", i);
        Operation op2 = p.createOperation("RecInOrder", i);
        Operation op3 = p.createOperation("RecPostOrder", i);
        Operation op4 = p.createOperation("IterWalk", i);

        //  Test functions
        recursivePreOrder(tree, op1, false);
        recursiveInOrder(tree, op2, false);
        recursivePostOrder(tree, op3, false);
        walkIterative(tree, op4, false);
    }
    p.createGroup("Comparision",
                  "RecPreOrder",
                  "RecInOrder",
                  "RecPostOrder",
                  "IterWalk");

    p.showReport();
}

void transforms()
{
    cout << "Reprezentare1:\n";
    prettyPrintR1(parent, 10, 0, 7);
    node2* tree2 = transformToMultiway(parent, sizeof(parent) / sizeof(int));
    cout << "Reprezentare2:\n";
    prettyPrintR2(tree2, 0);
    node3* tree3 = transformToBinaryTree(tree2, nullptr);
    cout << "Reprezentare3:\n";
    prettyPrintR3(tree3, 0);
}

int main()
{
    //demo();
    //perf();
    transforms();
    return 0;
}
