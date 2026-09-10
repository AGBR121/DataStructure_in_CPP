#include<iostream>
#include<cassert>

using namespace std;

template<typename T>

class List
{
private:
    // Node will hold the element and the pointer to the next data
    class Node{
        private:
        T data_; 
        Node* next_;
        Node* previus_;
        public:
        // constructors
        Node(){
            data_ = T(); // empty data
            next_ = nullptr; // null pointer
            previus_ = nullptr; // null pointer
        }
        Node(const T& element){
            data_ = element; // data with an element
            next_ = nullptr; // null pointer
            previus_ = nullptr; // null pointer
        }
        // change the memory address of the next node to another node
        void setNext(Node* next){
            next_ = next;
        }

        // change the memory address of the previous node to another node
        void setPrevius(Node* previus){
            previus_ = previus;
        }

        Node* getPrevius(){ return previus_; }
        Node* getNext() { return next_; } // Return the pointer to the next value
        T& getData() { return data_; } // Return the data
        const T& getData() const { return data_; } // Return the data without modifying it
    };
    Node* first_; // Node of the first element
    Node* last_; // node of the last element
    unsigned int size_; // Size of the list
public:
    // List constructor
    List(){
        first_ = nullptr;
        last_ = nullptr;
        size_ = 0;
    }
    // Return the size of the list
    unsigned int size() const { return size_; }
    // Return true if the list is empty
    bool empty() const { return size_ == 0; }
    // push_front
    void push_front(const T& element){
        Node* n = new Node(element); // new element
        first_->setPrevius(n); // the former first now has a previous
        n->setNext(first_); // the next pointer of the new node = pointer of the former first element
        first_ = n; // n is the new first_
        if(size_ == 0){
            last_ = n; // if the list was empty, last_ also points to n
        }
        size_++;
    }

    void push_back(const T& element){
        Node* n = new Node(element); // new element
        if (empty()) {
            first_ = last_ = n; // if it's empty, first and last will point to the same element
        } else {
            n->setPrevius(last_);// the new last has a previous, which is the former last
            last_->setNext(n); // the previous last now has the pointer of the new last as its next value
            last_ = n; // point to the new last
        }
        size_++;
    }

    void pop_front(){
        if(empty()){ return; } // if the list is empty, do nothing
        Node* temp = first_; // temporary node pointer of the first element
        first_ = first_->getNext(); // first now points to the next element
        first_->setPrevius(nullptr);// the first one has no previous
        delete temp; // free the node's memory
        size_--;
        if (size_ == 0) { last_ = nullptr; } // if there's nothing left, last points to nothing
    }

    void pop_back(){
        assert(!empty()); // if it's empty, error
        if(first_ == last_){ // if first and last point to the same thing
            delete first_; // free first
            first_ = last_ = nullptr; // neither will point to anything
        }else{
            Node* temp = last_->getPrevius(); // get the second-to-last node
            temp->setNext(nullptr); // change the next-node address of the second-to-last to null
            delete last_; // remove the last one
            last_ = temp; // the new last is the second-to-last

        }
        size_--;
    }

    T& at(unsigned int pos){
        assert(pos < size_ && pos >=0); // error if the position exceeds the size
        Node* current = first_; // node that will traverse until reaching the position
        for (unsigned int i = 0; i < pos; i++) {
            current = current->getNext();
        }
        return current->getData(); // return the found data
    }

    // same as above but without allowing the data to be changed
    const T& at(unsigned int pos) const{
        assert(pos < size_ && pos>=0);
        Node* current = first_;
        for (unsigned int i = 0; i < pos; i++) {
            current = current->getNext();
        }
        return current->getData();
    }

    // Add a piece of data
    void insert(unsigned int index, const T& element) {
        assert(index <= size_);
    
        if (index == 0) { 
            push_front(element); // if the index is 0, just do push_front
            return;
        }
    
        if (index == size_) { 
            push_back(element); // if the index is the last data, do push_back
            return;
        }

        Node* current = first_; // node pointer that will traverse the list until reaching the index
        for (unsigned int i = 0; i < index - 1; i++) {
            current = current->getNext();
        }
    
        Node* newNode = new Node(element); // new node in the array that will hold the element
        newNode->setNext(current->getNext()); // the new node will point to where the index used to point
        newNode->setPrevius(current);// the previous of the new node will be current
        current->setNext(newNode); // the previous node now points to the new node
        
        size_++;
    }
    void remove(unsigned int index){
        assert(index < size_ || index > size_);
    
        if (index == 0) { 
            pop_front(); // if the index is the first data, do pop_front
            return;
        }
        if(index == size_){
            pop_back(); // if the index is the last data, do pop_back
            return;
        }
        Node* temp = first_; // node pointer that will traverse the list until reaching index - 1
        for (unsigned int i = 0; i < index - 1; i++) {
            temp = temp->getNext();
        }

        Node* toDelete = temp->getNext(); // point to the next node, which will be the one we delete
        temp->setNext(toDelete->getNext()); // the node before the one to delete now points to the
                                            // node after the one to delete
        Node* next = temp->getNext(); // get the node after the one to delete
        next->setPrevius(temp);// change what it pointed to

        delete toDelete; // free the node
        
        size_--;
    }
    int find(const T& element) const {
        Node* temp = first_; // traverse the nodes until finding the element
        int index = 0;
    
        while (temp != nullptr) {
            if (temp->getData() == element) {
                return index; // return the position
            }
            temp = temp->getNext();
            index++;
        }
        
        return -1; // not found
    }

    // Sorts the list using the Merge Sort algorithm recursively.
    Node* MergeSort(Node* firstElement){
        // Base case: if the list is empty or has only one element, return it as is.
        if (!firstElement || !firstElement->getNext()){ 
            return firstElement; 
        }

        // Get the middle node of the list.
        Node* middle = getMiddle(firstElement);
        // Get the second half of the list.
        Node* secondMiddle = middle->getNext();
        // Split both halves apart.
        middle->setNext(nullptr);

        // Recursively sort both halves and then merge them.
        return Merge(MergeSort(firstElement), MergeSort(secondMiddle));
}

    // Finds the middle node of the list using the "slow and fast pointer" method.
    Node* getMiddle(Node* firstElement){
        // If the list is empty, return it as is.
        if(!firstElement){ return firstElement; }

        // Initialize two pointers, one slow (advances by one) and one fast (advances by two).
        Node* slow = firstElement, *fast = firstElement->getNext();
        
        // Traverse the list until the fast pointer reaches the end.
        while(fast && fast->getNext()){
            slow = slow->getNext();        // Advances by one.
            fast = fast->getNext()->getNext(); // Advances by two.
        }
        
        // Return the slow pointer, which will be in the middle of the list.
        return slow;
    }

    // Merges two sorted lists into a single sorted list.
    Node* Merge(Node* left, Node* right){
        // If one of the lists is empty, return the other one.
        if(!left){ return right; }
        if(!right){ return left; }

        // Compare the nodes' values to sort them correctly.
        if (left->getData() < right->getData()) {
            // If the left node is smaller, link it with the merge of the remaining nodes.
            left->setNext(Merge(left->getNext(), right));
            return left;
        } else {
            // If the right node is smaller or equal, link it with the merge of the remaining nodes.
            right->setNext(Merge(left, right->getNext()));
            return right;
        }
    }

    // Calls MergeSort to sort the list.
    void sort(){
        first_ = MergeSort(first_);
    }

    // Prints the list to the console.
    void print() const {
        // If the list is empty, print nothing.
        if (!first_) { return; }

        // Traverse the list node by node.
        Node* current = first_; 
        while (current) {
            cout << current->getData() << " "; // Print the current node's data.
            current = current->getNext(); // Move to the next node.
        }
        cout << endl; // Print a newline at the end.
    }

};

int main(){
    List<int> list;
    for(unsigned int i = 1; i<11; i++){
        list.push_back(i);
    }
    for(unsigned int i = 11; i<21; i++){
        list.push_front(i);
    }
    list.print();
    cout << "pop_back" << endl;
    list.pop_back();
    list.print();
    cout << "pop_front" << endl;
    list.pop_front();
    list.print();
    cout << "remove(4)" << endl;
    list.remove(4);
    list.print();
    cout << "insert(4,56)" << endl;
    list.insert(4, 56);
    list.print();
    cout << "at(2)" << endl;
    cout << list.at(2) << endl;
    cout << "empty:" << endl;
    if(!list.empty()){
        cout << "Lista con datos" << endl;
    }
    cout << "find(4)" << endl;
    cout << list.find(4) << endl;
    cout << "insert(7,97)" << endl;
    list.insert(7, 97);
    list.print();
    cout << "insert(0,2)" << endl;
    list.insert(0, 2);
    list.print();
    cout << "at(3) = 22" << endl;
    list.at(3) = 22;
    list.print();
    cout << "sort()" << endl;
    list.sort();
    list.print();
}