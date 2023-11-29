#include <cstring>
#include <string>
using namespace std;




// Node structure for Huffman Tree
struct HuffmanNode
{
    char data;
    int frequency;
    HuffmanNode *left;
    HuffmanNode *right;

    HuffmanNode(char d, int f) : data(d), frequency(f), left(nullptr), right(nullptr) {}
};

// Comparison struct for priority_queue
struct CompareNodes
{
    bool operator()(HuffmanNode *a, HuffmanNode *b)
    {
        return a->frequency > b->frequency;
    }
};
