#include<iostream>
#include<cassert>

#include "vector.h"

using namespace std;

template<typename T>
class Stack{
    private:
    //It will have a vector of type T
    Vector<T> data;

    public:
    //Default constructor
    Stack() {}

    // Stack methods

    //Method to add an element to the stack
    void push(const T& element) { data.push_back(element); }
    //Method to remove an element from the stack
    void pop() { assert(!data.empty()); data.pop_back(); }
    //Method to get the element at the top of the stack without modifying it (top or peek are the same)
    const T& top() const { assert(!data.empty()); return data.at(data.size()-1); }
    //Method to get the element at the top of the stack and modify it
    T& top() { assert(!data.empty()); return data.at(data.size()-1); }
    //Method to check if the stack is empty
    bool empty() const { return data.empty(); }
    //Method to get the size of the stack
    unsigned int size() const { return data.size(); }
    //Method to print the stack data
    void print() { 
        assert(!data.empty());
        for(int i = data.size()-1; i>=0; i--){
            cout << data.at(i) << " ";
        }
        cout << endl;
    }
};

int main(){
    Stack<int> stack;
    stack.push(1);
    stack.push(2);
    stack.push(3);
    stack.push(4);
    stack.push(5);
    stack.print(); // Expected: 5 4 3 2 1
    stack.pop();
    stack.print(); // Expected: 4 3 2 1
    cout << stack.top() << endl; // Expected: 4
    stack.top() = 10;
    stack.print(); // Expected: 10 3 2 1
    cout << stack.size() << endl; // Expected: 4
    cout << stack.empty() << endl; // Expected: 0
    return 0;
}