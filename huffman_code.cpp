// huffman_practice.cpp: Usman Ayobami
// Description:  Huffman Coding in C++
// Reference Code: https://www.programiz.com/dsa/huffman-coding#:~:text=Huffman%20Coding%20Complexity,is%20O(nlog%20n)%20.
// Compile with "g++ -std=c++11 -o huff huffman_code.cpp"

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
#include <iomanip>
#include <algorithm>
#include <vector>
#include "huffman_data.h"
#include <stdlib.h>
using namespace std;

#define k_max_tree_height 26
#define k_table_data_width 15
#define letters_size 26

/* Prints out the result in a table format. */
template <typename T>
void FormatDataInTabularForm(T t)
{
    cout << left << setw(k_table_data_width) << setfill(' ') << t;
}

// Creating Heap node
HeapNode *CreateHeapNode(char item, unsigned freq)
{
    HeapNode *temp = new HeapNode;
    temp->left = temp->right = NULL;
    temp->letter = item;
    temp->frequency = freq;
    return temp;
}

// Create min heap using given capacity
// Returns a pointer to the min heap
MinHeap *CreateMinHeap(unsigned capacity)
{
    MinHeap *min_heap = new MinHeap;
    min_heap->size = 0;
    min_heap->capacity = capacity;
    min_heap->array = new HeapNode *[capacity];
    return min_heap;
}

string ConvertArrayOfIntegerToString(const vector<int> &arr)
{
    string output_string = "";

    for (size_t i = 0; i < arr.size(); i++)
    {
        output_string += to_string(arr[i]);
    }

    return output_string;
}

// Swap function
void SwapHuffmanTreeNode(HeapNode **a, HeapNode **b)
{
    HeapNode *temp = *a;
    *a = *b;
    *b = temp;
}

// Re-arrange heap to make a min-heap.
// If left is smaller than parent, swap parent and left
// If right is smaller than parent, swap parent and right
void MinHeapify(MinHeap *minHeap, int idx)
{
    int parent = idx;
    int left = 2 * idx + 1;
    int right = 2 * idx + 2;

    if (left < minHeap->size && minHeap->array[left]->frequency < minHeap->array[parent]->frequency)
        parent = left;

    if (right < minHeap->size && minHeap->array[right]->frequency < minHeap->array[parent]->frequency)
        parent = right;

    if (parent != idx)
    {
        SwapHuffmanTreeNode(&minHeap->array[parent], &minHeap->array[idx]);
        MinHeapify(minHeap, parent);
    }
}

// Extract the min
// The min is the first node
// Remove the first node, replace it with the last node
// Resize the heap i.e minus one
// Re-arrange heap to make a min-heap.
HeapNode *ExtractMinimumNode(MinHeap *min_heap)
{
    HeapNode *temp = min_heap->array[0];
    min_heap->array[0] = min_heap->array[min_heap->size - 1];

    --min_heap->size;
    MinHeapify(min_heap, 0);

    return temp;
}

// Insertion
void InsertMinHeap(MinHeap *min_heap, HeapNode *min_heap_node)
{
    ++min_heap->size;
    int i = min_heap->size - 1;

    while (i && min_heap_node->frequency < min_heap->array[(i - 1) / 2]->frequency)
    {
        min_heap->array[i] = min_heap->array[(i - 1) / 2];
        i = (i - 1) / 2;
    }

    min_heap->array[i] = min_heap_node;
}

MinHeap *CreateMinHeapAndHeapify(char item[], int freq[], int size)
{
    MinHeap *new_min_heap = CreateMinHeap(size);

    for (int i = 0; i < size; i++)
        new_min_heap->array[i] = CreateHeapNode(item[i], freq[i]);

    new_min_heap->size = size;

    int n = new_min_heap->size - 1;
    for (int i = (n - 1) / 2; i >= 0; --i)
        MinHeapify(new_min_heap, i);

    return new_min_heap;
}

HeapNode *BuildHuffmanTree(char item[], int freq[], int size)
{
    HeapNode *left, *right, *new_node;
    MinHeap *min_heap = CreateMinHeapAndHeapify(item, freq, size);

    while (min_heap->size != 1)
    {
        left = ExtractMinimumNode(min_heap);
        right = ExtractMinimumNode(min_heap);

        new_node = CreateHeapNode('$', left->frequency + right->frequency);

        new_node->left = left;
        new_node->right = right;

        // insert this new_node into the tree
        InsertMinHeap(min_heap, new_node);
    }
    return ExtractMinimumNode(min_heap);
}

// For each non-leaf node, assign 0 to the left edge and 1 to the right edge.
// When you encounter a leaf node, print all the already assigned numbers
void AssignNumbersToEdges(HeapNode *root, vector<int> &code_array, int top, vector<TableData *> &table_data)
{
    if (root->left)
    {
        // for left tree, assign 0 to the array and process next node.
        code_array.push_back(0);
        AssignNumbersToEdges(root->left, code_array, top + 1, table_data);
        code_array.pop_back();
    }

    if (root->right)
    {
        // for right tree, assign 1 to the array and process next node.
        code_array.push_back(1);
        AssignNumbersToEdges(root->right, code_array, top + 1, table_data);
        code_array.pop_back();
    }
    // For a leaf node
    if (!root->left && !root->right)
    {
        table_data.push_back(new TableData{
            root,
            ConvertArrayOfIntegerToString(code_array),
            top});
    }
}

void GenerateHuffmanCodes(char item[], int freq[], int size, vector<TableData *> &table_data)
{
    HeapNode *root = BuildHuffmanTree(item, freq, size);

    vector<int> code_array;
    AssignNumbersToEdges(root, code_array, 0, table_data);
}

void PrintTabularResult(vector<TableData *> &table_data)
{
    // Header
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

    cout << endl;

    // Body
    for (size_t i = 0; i < table_data.size(); i++)
    {
        FormatDataInTabularForm(table_data[i]->root->letter);
        FormatDataInTabularForm(table_data[i]->root->frequency);
        FormatDataInTabularForm(table_data[i]->code_array);
        FormatDataInTabularForm(table_data[i]->code_array.length());
        FormatDataInTabularForm(table_data[i]->code_array.length() * table_data[i]->root->frequency);
        cout << endl;
    }
}

bool CompareTableData(TableData *a, TableData *b)
{
    return a->root->letter < b->root->letter;
}

int main()
{
    char letters[] = {'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z'};
    int letters_frequencies[] = {77, 17, 32, 42, 120, 24, 17, 50, 76, 4, 7, 42, 24, 67, 67, 20, 5, 59, 67, 85, 37, 12, 22, 4, 22, 2};

    vector<TableData *> table_data;
    GenerateHuffmanCodes(letters, letters_frequencies, letters_size, table_data);
    // Sort data in alphabetical order.
    sort(table_data.begin(), table_data.end(), CompareTableData);

    // Print
    PrintTabularResult(table_data);

    int sum_of_letters_frequencyxlenght = 0;
    for (size_t i = 0; i < table_data.size(); i++)
    {
        sum_of_letters_frequencyxlenght += (table_data[i]->code_array.length() * table_data[i]->root->frequency);
    }

    cout << endl;
    cout << "The weighted minimum path length is: " << sum_of_letters_frequencyxlenght << " bits" << endl;

    // memory clean up
    for (size_t i = 0; i < table_data.size(); i++)
    {
        delete table_data[i];
    }

    return 0;
}
