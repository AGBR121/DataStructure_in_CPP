#include <iostream>
#include <vector>
#include <cassert>
#include <stdexcept>

using namespace std;

class MaxHeap {
    private:
        vector<int> data;
    
        int parent(int i) { return (i - 1) / 2; }
        int left(int i)   { return 2 * i + 1; }
        int right(int i)  { return 2 * i + 2; }
    
        void heapifyUp(int i) {
            while (i > 0 && data[i] > data[parent(i)]) {
                swap(data[i], data[parent(i)]);
                i = parent(i);
            }
        }
    
        void heapifyDown(int i) {
            int n = data.size();
            int largest = i;
    
            int l = left(i);
            int r = right(i);
    
            if (l < n && data[l] > data[largest]) largest = l;
            if (r < n && data[r] > data[largest]) largest = r;
    
            if (largest != i) {
                swap(data[i], data[largest]);
                heapifyDown(largest);
            }
        }
    
    public:
        void insert(int value) {
            data.push_back(value);
            heapifyUp(data.size() - 1);
        }
    
        int extractMax() {
            assert(!data.empty());
            int root = data[0];
            data[0] = data.back();
            data.pop_back();
            if (!data.empty()) heapifyDown(0);
            return root;
        }
    
        int getMax() const {
            assert(!data.empty());
            return data[0];
        }
    
        void print() const {
            for (int val : data)
                cout << val << " ";
            cout << endl;
        }
    
        bool empty() const { return data.empty(); }
        int size() const { return data.size(); }
    };

    class MinHeap {
        private:
            vector<int> data;
        
            int parent(int i) { return (i - 1) / 2; }
            int left(int i)   { return 2 * i + 1; }
            int right(int i)  { return 2 * i + 2; }
        
            void heapifyUp(int i) {
                while (i > 0 && data[i] < data[parent(i)]) {
                    swap(data[i], data[parent(i)]);
                    i = parent(i);
                }
            }
        
            void heapifyDown(int i) {
                int n = data.size();
                int smallest = i;
        
                int l = left(i);
                int r = right(i);
        
                if (l < n && data[l] < data[smallest]) smallest = l;
                if (r < n && data[r] < data[smallest]) smallest = r;
        
                if (smallest != i) {
                    swap(data[i], data[smallest]);
                    heapifyDown(smallest);
                }
            }
        
        public:
            void insert(int value) {
                data.push_back(value);
                heapifyUp(data.size() - 1);
            }
        
            int extractMin() {
                assert(!data.empty());
                int root = data[0];
                data[0] = data.back();
                data.pop_back();
                if (!data.empty()) heapifyDown(0);
                return root;
            }
        
            int getMin() const {
                assert(!data.empty());
                return data[0];
            }
        
            void print() const {
                for (int val : data)
                    cout << val << " ";
                cout << endl;
            }
        
            bool empty() const { return data.empty(); }
            int size() const { return data.size(); }
        };

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