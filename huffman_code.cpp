// huffman_practice.cpp: Usman Ayobami
// Description:  Huffman Coding in C++

/*
Huffman Coding Algorithm
create a priority queue Q consisting of each unique character.
for all the unique characters:
    create a newNode
    extract minimum value from Q and assign it to leftChild of newNode
    extract minimum value from Q and assign it to rightChild of newNode
    calculate the sum of these two minimum values and assign it to the value of newNode
    insert this newNode into the tree
return rootNode
*/

#include <iostream>
#include <vector>
#include <queue>
#include <unordered_map>
#include <iomanip>
#include <numeric>
#include "huffman_data.h"

using namespace std;
#define k_table_data_width 15
#define letters_size 26

// Function to build Huffman Tree
HuffmanNode *buildHuffmanTree(vector<char> &letters, vector<int> &frequencies)
{
    priority_queue<HuffmanNode *, vector<HuffmanNode *>, CompareNodes> minHeap;

    // Create nodes for each letter and add to minHeap
    for (size_t i = 0; i < letters.size(); i++)
    {
        // create a newNode
        HuffmanNode *node = new HuffmanNode(letters[i], frequencies[i]);
        minHeap.push(node);
    }

    // Constructing the Huffman Tree
    // We will remove all the items in the queue and add them to the left or right.
    // Eventually, we'll have one item (i.e. Heap Head) in the queue.
    while (minHeap.size() > 1)
    {
        // extract minimum value from Q and assign it to leftChild of newNode
        HuffmanNode *left = minHeap.top();
        minHeap.pop();
        // extract minimum value from Q and assign it to rightChild of newNode
        HuffmanNode *right = minHeap.top();
        minHeap.pop();

        // calculate the sum of these two minimum values and assign it to the value of newNode
        HuffmanNode *newNode = new HuffmanNode('$', left->frequency + right->frequency);
        newNode->left = left;
        newNode->right = right;
        // insert this newNode into the tree
        minHeap.push(newNode);
    }
    // return rootNode
    return minHeap.top();
}

// Recursive function to generate Huffman codes
void generateHuffmanCodes(HuffmanNode *root, string code, unordered_map<char, string> &huffmanCodes)
{
    if (root == nullptr)
        return;

    // If it is a leaf node i.e. left and right children are empty nodes.
    if (root->left == nullptr && root->right == nullptr)
    {
        huffmanCodes[root->data] = code;
        return;
    }
    // Assign '0' to left childrean
    generateHuffmanCodes(root->left, code + "0", huffmanCodes);

    // Assign '1' to right childrean
    generateHuffmanCodes(root->right, code + "1", huffmanCodes);
}

// Prints out the result in a table format.
template <typename T>
void FormatDataInTabularForm(T t)
{
    cout << left << setw(k_table_data_width) << setfill(' ') << t;
}

void printResult(vector<char> data, vector<int> frequency, unordered_map<char, string> &huffmanCodes)
{
    // Header
    cout << endl
         << endl;
    FormatDataInTabularForm("Letter");
    FormatDataInTabularForm("Frequency");
    FormatDataInTabularForm("Code");
    FormatDataInTabularForm("Length");
    FormatDataInTabularForm("Freq X Len");
    cout << endl;

    for (int i = 0; i < 5; i++)
    {
        FormatDataInTabularForm("----------");
    }

    cout << endl
         << endl;

    int sum_of_letters_frequencies = 0;
    int sum_of_letters_frequencyxlenght = 0;
    // Body
    for (int i = 0; i < letters_size; i++)
    {
        FormatDataInTabularForm(data.at(i));
        FormatDataInTabularForm(frequency.at(i));
        FormatDataInTabularForm(huffmanCodes[data.at(i)]);
        FormatDataInTabularForm(huffmanCodes[data.at(i)].length());
        FormatDataInTabularForm(huffmanCodes[data.at(i)].length() * frequency.at(i));
        cout << endl;
        sum_of_letters_frequencies += frequency.at(i);
        sum_of_letters_frequencyxlenght += huffmanCodes[data.at(i)].length() * frequency.at(i);
    }
    cout << endl
         << endl;
    cout << "The weighted minimum path length is: " << sum_of_letters_frequencyxlenght << " bits" << endl
         << endl;
};

int main()
{
    vector<char> letters = {'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z'};
    vector<int> frequencies = {77, 17, 32, 42, 120, 24, 17, 50, 76, 4, 7, 42, 24, 67, 67, 20, 5, 59, 67, 85, 37, 12, 22, 4, 22, 2};

    HuffmanNode *root = buildHuffmanTree(letters, frequencies);

    unordered_map<char, string> huffmanCodes;
    generateHuffmanCodes(root, "", huffmanCodes);
    printResult(letters, frequencies, huffmanCodes);

    return 0;
}
