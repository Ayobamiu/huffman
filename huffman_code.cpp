// huffman_practice.cpp: Usman Ayobami
// Description:  Huffman Coding in C++
// Reference Code: https://www.programiz.com/dsa/huffman-coding#:~:text=Huffman%20Coding%20Complexity,is%20O(nlog%20n)%20.

/*
Huffman Coding Algorithm
create a priority queue Q consisting of each unique character.
sort then in ascending order of their frequencies.
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
#include <numeric>

#include "huffman_data.h"
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

TableData **table_data;
int total_table_data;

int SortTableData(const void *a, const void *b)
{
    TableData *a_poniter = *((TableData **)a);
    TableData *b_pointer = *((TableData **)b);

    if (a_poniter == NULL || b_pointer == NULL)
    {
        return 0;
    }
    return strcmp(&a_poniter->root->letter, &b_pointer->root->letter);
}

// Creating Heap node
struct HeapNode *CreateHeapNode(char item, unsigned freq)
{
    struct HeapNode *temp = (struct HeapNode *)malloc(sizeof(struct HeapNode));

    temp->left = temp->right = NULL;
    temp->letter = item;
    temp->frequency = freq;

    return temp;
}

// Create min heap using given capacity
// Allocate space for the min heap
// Returns a pointer to the min heap
struct MinHeap *CreateMinHeap(unsigned capacity)
{
    struct MinHeap *min_heap = (struct MinHeap *)malloc(sizeof(struct MinHeap));
    min_heap->size = 0;
    min_heap->capacity = capacity;
    min_heap->array = (struct HeapNode **)malloc(min_heap->capacity * sizeof(struct HeapNode *));
    return min_heap;
}

string ConvertArrayOfIntegerToString(int arr[], int n)
{
    string output_string = "";

    for (int i = 0; i < n; ++i)
        output_string += to_string(arr[i]);

    return output_string;
}

// Swap function
void SwapHuffmanTreeNode(struct HeapNode **a, struct HeapNode **b)
{
    struct HeapNode *temp = *a;
    *a = *b;
    *b = temp;
}

// Re-arrange heap to make a min-heap.
// If left is smaller than parent, swap parent and left
// If right is smaller than parent, swap parent and right
void MinHeapify(struct MinHeap *minHeap, int idx)
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
        SwapHuffmanTreeNode(&minHeap->array[parent],
                            &minHeap->array[idx]);
        MinHeapify(minHeap, parent);
    }
}

// Extract the min
// The min is the first node
// Remove the first node, replace it with the last node
// Resize the heap i.e minus one
// Re-arrange heap to make a min-heap.
struct HeapNode *ExtractMinimumNode(struct MinHeap *min_heap)
{
    struct HeapNode *temp = min_heap->array[0];
    min_heap->array[0] = min_heap->array[min_heap->size - 1];

    --min_heap->size;
    MinHeapify(min_heap, 0);

    return temp;
}

// Insertion
void InsertMinHeap(struct MinHeap *min_heap, struct HeapNode *min_heap_node)
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

int IsLeaf(struct HeapNode *root)
{
    return !(root->left) && !(root->right);
}

struct MinHeap *CreateMinHeapAndHeapify(char item[], int freq[], int size)
{
    struct MinHeap *new_min_heap = (struct MinHeap *)malloc(sizeof(struct MinHeap));
    new_min_heap->size = 0;
    new_min_heap->capacity = size;
    new_min_heap->array = (struct HeapNode **)malloc(new_min_heap->capacity * sizeof(struct HeapNode *));

    for (int i = 0; i < size; ++i)
        new_min_heap->array[i] = CreateHeapNode(item[i], freq[i]);

    new_min_heap->size = size;

    int n = new_min_heap->size - 1;
    for (int i = (n - 1) / 2; i >= 0; --i)
        MinHeapify(new_min_heap, i);

    return new_min_heap;
}

struct HeapNode *BuildHuffmanTree(char item[], int freq[], int size)
{
    struct HeapNode *left, *right, *new_node;
    struct MinHeap *min_heap = CreateMinHeapAndHeapify(item, freq, size);

    while (min_heap->size != 1)
    {
        left = ExtractMinimumNode(min_heap);
        right = ExtractMinimumNode(min_heap);

        // calculate the sum of these two minimum values and assign it to the value of new_node
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
void AssignNumbersToEdges(struct HeapNode *root, int code_array[], int top)
{
    if (root->left)
    {
        code_array[top] = 0;
        // for left tree, assign 0 to the array and process next node.
        AssignNumbersToEdges(root->left, code_array, top + 1);
    }

    if (root->right)
    {
        // for right tree, assign 1 to the array and process next node.
        code_array[top] = 1;
        AssignNumbersToEdges(root->right, code_array, top + 1);
    }
    if (IsLeaf(root))
    {
        // Increment the size of total_table_data
        total_table_data++;
        table_data = (TableData **)realloc(table_data, sizeof(TableData *) * total_table_data);
        table_data[total_table_data - 1] = (TableData *)malloc(sizeof(TableData));
        table_data[total_table_data - 1]->code_array = ConvertArrayOfIntegerToString(code_array, top);
        table_data[total_table_data - 1]->root = root;
        table_data[total_table_data - 1]->top = top;
    }
}

void GenerateHuffmanCodes(char item[], int freq[], int size)
{
    struct HeapNode *root = BuildHuffmanTree(item, freq, size);

    int code_array[k_max_tree_height], top = 0;

    AssignNumbersToEdges(root, code_array, top);
}

void PrintTabularResult()
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
    for (int i = 0; i < total_table_data; i++)
    {
        FormatDataInTabularForm(table_data[i]->root->letter);
        FormatDataInTabularForm(table_data[i]->root->frequency);
        FormatDataInTabularForm(table_data[i]->code_array);
        FormatDataInTabularForm(table_data[i]->code_array.length());
        FormatDataInTabularForm(table_data[i]->code_array.length() * table_data[i]->root->frequency);
        cout << endl;
    }
};

int main()
{
    char letters[] = {'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z'};
    int letters_frequencies[] = {77, 17, 32, 42, 120, 24, 17, 50, 76, 4, 7, 42, 24, 67, 67, 20, 5, 59, 67, 85, 37, 12, 22, 4, 22, 2};

    total_table_data = 0;
    TableData **data_ts = (TableData **)malloc(sizeof(TableData *) * letters_size);

    GenerateHuffmanCodes(letters, letters_frequencies, letters_size);
    // Sort data in alphabetical order.
    qsort(table_data, total_table_data, sizeof(TableData *), SortTableData);
    // Print
    PrintTabularResult();

    // cout << "Characters 26 * 8 =" << 26 * 8 << " bits" << endl;
    // int sum_of_letters_frequencies = accumulate(letters_frequencies, letters_frequencies + letters_size, 0);
    int sum_of_letters_frequencyxlenght = 0;
    for (int i = 0; i < total_table_data; i++)
    {
        sum_of_letters_frequencyxlenght += (table_data[i]->code_array.length() * table_data[i]->root->frequency);
    }
    // cout << "Sum of Frequencies =" << sum_of_letters_frequencies << " bits" << endl;
    cout << endl;
    cout << "The weighted minimum path length is: " << sum_of_letters_frequencyxlenght << " bits" << endl;
}