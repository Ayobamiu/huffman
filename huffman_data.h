#include <cstring>
#include <string>
using namespace std;

struct HeapNode
{
    unsigned frequency;
    char letter;
    HeapNode *left, *right;
};

struct MinHeap
{
    unsigned size;
    unsigned capacity;
    HeapNode **array;
};

struct TableData
{
    struct HeapNode *root;
    string code_array;
    int top;
};