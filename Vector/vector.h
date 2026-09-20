#ifndef VECTOR_H
#define VECTOR_H

#include <iostream>
#include <cassert>
#include <initializer_list>

using namespace std;

// Define a class template for a dynamic vector
template<typename T>
class Vector {
    private:
    T* storage_; // Pointer to the dynamic memory where the elements are stored
    unsigned int size_; // Number of elements stored in the vector
    unsigned int capacity_; // Maximum capacity before needing to resize

    public:
    // Default constructor: initializes with a capacity of 5 and size 0
    Vector(){
        capacity_ = 5;
        storage_ = new T[capacity_];
        size_ = 0;
    }
    
    // Constructor with parameters: initializes with given capacity and values
    Vector(unsigned int c, T element){
        capacity_ = c;
        storage_ = new T[capacity_];
        size_ = c;
        for(int i = 0; i < c; i++){
            storage_[i] = element;
        }
    }

    Vector(initializer_list<T> list){
        capacity_ = list.size();  // The capacity is the number of elements in the list
        storage_ = new T[capacity_];
        size_ = list.size();
        unsigned int i = 0;
        for (const T& elem : list) {
            storage_[i++] = elem;  // Copy each element into the array
        }
    }
    
    // Method to get the current size of the vector
    unsigned int size() const{
        return size_;
    }

    //Method to get the current capacity of the vector
    unsigned int capacity() const{
        return capacity_;
    }
    
    // Method to access an element with bounds checking
    T& at(unsigned int pos){
        assert(pos < size_ && pos>=0); // Ensures the position is valid
        return storage_[pos];
    }
    
    // `at` method to access an element in a const version of the object
    const T& at(unsigned int pos) const {
        assert(pos < size_ && pos>=0);
        return storage_[pos];
    }
    
    // Overload of the [] operator for fast access without checking
    const T& operator[](unsigned int pos) const{
        return storage_[pos];
    }
    
    private:
    // Private method to resize the storage when necessary
    void resize(){
        
        unsigned int capacity2_ = capacity_ * 3 / 2; // Increases the capacity by 1.5 times
        //unsigned int capacity2_ = capacity_ * 2; // Increases the capacity by 2 times
        //unsigned int capacity2_ = capacity_ + 1; // Increases the capacity by +1
        //unsigned int capacity2_ = capacity_ + 2; // Increases the capacity by +2

        T* storage2_ = new T[capacity2_]; // New larger array
        
        // Copies the elements to the new array
        for(unsigned int i=0; i<size_; i++){
            storage2_[i] = storage_[i];
        }
        
        delete[] storage_; // Frees the memory of the old array
        storage_ = storage2_; // Points to the new array
        capacity_ = capacity2_; // Updates the capacity
    }
    
    public:
    // Adds an element at the end of the vector
    void push_back(const T& element){
        if (size_ == capacity_) {
            resize();
        }
        storage_[size_] = element;
        size_++;
    }
    
    // Adds an element at the beginning of the vector
    void push_front(const T& element){
        if (size_ == capacity_) {
            resize();
        }
        
        // Shifts the elements to the right
        for (unsigned int i = size_; i > 0; i--) {
            storage_[i] = storage_[i - 1];
        }
        storage_[0] = element;
        size_++;
    }
    
    // Removes the last element of the vector
    void pop_back(){
        assert(size_ > 0);
        size_--; return; 
    }
    
    // Removes the first element of the vector and shifts the rest
    void pop_front(){
        assert(size_ > 0);
        for (unsigned int i = 0; i < size_ - 1; i++) {
            storage_[i] = storage_[i + 1];
        }
            size_--;
    }
    
    // Checks if the vector is empty
    bool empty() const { return size_ == 0; }

    //Returns the unused memory space
    unsigned int waste(){ return capacity_ - size_;}

    //Frees the memory that is not used
    void shrink_to_fit(){
        T* storage2_ = new T[size_];
        for(unsigned int i = 0; i < size_; i++){
            storage2_[i] = storage_[i];
        }
        delete[] storage_;
        capacity_ = size_;
        storage_ = storage2_;
    }
    
    //insert inserts an element at an index position
    void insert(unsigned int index, const T& element){
        assert(index <= size_); //ensure it is in range

        if(size_ == capacity_){ resize() ; } //adjust size if it doesn't fit

        for(unsigned int i = size_; i > index; i--){
            storage_[i] = storage_[i-1]; // Moves the elements to the right
        }
        
        storage_[index] = element; //add the element to the vector
        size_++; //increase the size
    }
    //erase removes an element from the vector
    void erase (unsigned int index){
        assert(index<size_); //ensure it is in range
        if(index == 0){ pop_front(); return; }
        if(index == size_-1){pop_back(); return;}
        for(unsigned int i = index; i<size_-1; i++){
            storage_[i] = storage_[i+1]; //moves the elements 1 position to the left
        }
        size_--; //decrease the size
    }

    // Prints the elements of the vector
    void print(){
        for(unsigned int i = 0; i<size_; i++){
            cout << storage_[i]  << " " ;
        }
        cout << endl;
    }
};

#endif