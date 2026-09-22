#include <iostream>
#include <vector>
#include <cassert>
#include <stdexcept>

using namespace std;

// MaxHeap implementation over a vector (the heap is stored in an array).
class MaxHeap {
    private:
        vector<int> data; // The vector that holds the heap elements

        // Return the index of the parent node
        int parent(int i) { return (i - 1) / 2; }
        // Return the index of the left child
        int left(int i)   { return 2 * i + 1; }
        // Return the index of the right child
        int right(int i)  { return 2 * i + 2; }
    
        // Restore the heap property moving a node up until its parent is bigger
        void heapifyUp(int i) {
            while (i > 0 && data[i] > data[parent(i)]) {
                swap(data[i], data[parent(i)]); // swap with the parent
                i = parent(i); // move up
            }
        }
    
        // Restore the heap property moving a node down until both children are smaller
        void heapifyDown(int i) {
            int n = data.size();
            int largest = i; // assume the current node is the largest

            int l = left(i);
            int r = right(i);

            // Find the largest among the node and its children
            if (l < n && data[l] > data[largest]) largest = l;
            if (r < n && data[r] > data[largest]) largest = r;

            // If the largest is not the node, swap and keep going down
            if (largest != i) {
                swap(data[i], data[largest]);
                heapifyDown(largest);
            }
        }
    
    public:
        // Insert a value at the end and restore the heap property
        void insert(int value) {
            data.push_back(value); // add at the end
            heapifyUp(data.size() - 1); // fix the heap from that position
        }
    
        // Extract (and remove) the maximum element of the heap
        int extractMax() {
            assert(!data.empty()); // can't extract from an empty heap
            int root = data[0]; // the maximum is always the root
            data[0] = data.back(); // move the last element to the root
            data.pop_back(); // remove the last element
            if (!data.empty()) heapifyDown(0); // restore the heap property
            return root;
        }
    
        // Return the maximum element without removing it
        int getMax() const {
            assert(!data.empty()); // error if the heap is empty
            return data[0];
        }
    
        // Print the heap elements
        void print() const {
            for (int val : data)
                cout << val << " ";
            cout << endl;
        }
    
        bool empty() const { return data.empty(); } // Return true if the heap is empty
        int size() const { return data.size(); } // Return the number of elements
    };

    // MinHeap implementation over a vector (the heap is stored in an array).
    class MinHeap {
        private:
            vector<int> data; // The vector that holds the heap elements
        
            int parent(int i) { return (i - 1) / 2; } // Return the index of the parent node
            int left(int i)   { return 2 * i + 1; } // Return the index of the left child
            int right(int i)  { return 2 * i + 2; } // Return the index of the right child
        
            // Restore the heap property moving a node up until its parent is smaller
            void heapifyUp(int i) {
                while (i > 0 && data[i] < data[parent(i)]) {
                    swap(data[i], data[parent(i)]); // swap with the parent
                    i = parent(i); // move up
                }
            }
        
            // Restore the heap property moving a node down until both children are bigger
            void heapifyDown(int i) {
                int n = data.size();
                int smallest = i; // assume the current node is the smallest

                int l = left(i);
                int r = right(i);

                // Find the smallest among the node and its children
                if (l < n && data[l] < data[smallest]) smallest = l;
                if (r < n && data[r] < data[smallest]) smallest = r;

                // If the smallest is not the node, swap and keep going down
                if (smallest != i) {
                    swap(data[i], data[smallest]);
                    heapifyDown(smallest);
                }
            }
        
        public:
            // Insert a value at the end and restore the heap property
            void insert(int value) {
                data.push_back(value); // add at the end
                heapifyUp(data.size() - 1); // fix the heap from that position
            }
        
            // Extract (and remove) the minimum element of the heap
            int extractMin() {
                assert(!data.empty()); // can't extract from an empty heap
                int root = data[0]; // the minimum is always the root
                data[0] = data.back(); // move the last element to the root
                data.pop_back(); // remove the last element
                if (!data.empty()) heapifyDown(0); // restore the heap property
                return root;
            }
        
            // Return the minimum element without removing it
            int getMin() const {
                assert(!data.empty()); // error if the heap is empty
                return data[0];
            }
        
            // Print the heap elements
            void print() const {
                for (int val : data)
                    cout << val << " ";
                cout << endl;
            }
        
            bool empty() const { return data.empty(); } // Return true if the heap is empty
            int size() const { return data.size(); } // Return the number of elements
        };

// Test the MaxHeap implementation
void testMaxHeap() {
    MaxHeap maxHeap;
    maxHeap.insert(10);
    maxHeap.insert(20);
    maxHeap.insert(5);
    maxHeap.insert(30);
    maxHeap.insert(15);
    maxHeap.insert(25);

    cout << "MaxHeap: ";
    maxHeap.print(); 

    cout << "Max: " << maxHeap.getMax() << endl; 
    cout << "Extract Max: " << maxHeap.extractMax() << endl; 
    cout << "Max after extraction: " << maxHeap.getMax() << endl; 
}

// Test the MinHeap implementation
void testMinHeap() {
    MinHeap minHeap;
    minHeap.insert(10);
    minHeap.insert(20);
    minHeap.insert(5);
    minHeap.insert(30);
    minHeap.insert(15);
    minHeap.insert(7);

    cout << "MinHeap: ";
    minHeap.print(); 

    cout << "Min: " << minHeap.getMin() << endl; 
    cout << "Extract Min: " << minHeap.extractMin() << endl; 
    cout << "Min after extraction: " << minHeap.getMin() << endl; 
}

int main() {
    cout << "Testing MaxHeap:" << endl;
    testMaxHeap();
    cout << endl;

    cout << "Testing MinHeap:" << endl;
    testMinHeap();
    cout << endl;

    return 0;
}