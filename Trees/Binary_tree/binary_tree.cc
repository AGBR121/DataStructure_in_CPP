#include <iostream>
#include <cassert>
#include <chrono>

using namespace std;

template<typename T>
class BST {
    private:
    class Node{
        private:
        T element_;
        Node* left_;
        Node* right_;

        public:
        Node(){
            element_ = T();
            left_ = nullptr;
            right_ = nullptr;
        }

        Node(const T& data){
            element_ = data;
            left_ = nullptr;
            right_ = nullptr;
        }

        //Getter and setter of the Node class
        T getElement() const { return element_; }
        void setElement(const T& element) { element_ = element; }

        Node* getLeft() const { return left_; }
        void setLeft(Node* left) { left_ = left; }

        Node* getRight() const { return right_; }
        void setRight(Node* right) { right_ = right; }
    };

    private:

    Node* root_;

    unsigned int size_;


    Node* insert(Node* node, T value){
        if(!node){ return new Node(value); } //If the value isn't in the tree, we add it

        if(node->getElement() == value){return node;} //Else, we don't add it

        if (value < node->getElement()) {
            node->setLeft(insert(node->getLeft(), value));
        } else if (value > node->getElement()) {
            node->setRight(insert(node->getRight(), value));
        }
        return node; //Return the node and we don't change the tree
    }

    //Left-Root-Right
    void inorder(Node* node){
        assert(root_ != nullptr);
        if(node != nullptr){
            inorder(node->getLeft());
            cout << node->getElement()<< " ";
            inorder(node->getRight());
        }
    }

    //Root-Left-Right
    void preorder(Node* node){
        assert(root_ != nullptr);
        if(node != nullptr){
            cout << node->getElement() <<" ";
            preorder(node->getLeft());
            preorder(node->getRight());
        }
    }

    //Left-Right-Root
    void postorder(Node* node){
        assert(root_ != nullptr);
        if(node != nullptr){
            postorder(node->getLeft());
            postorder(node->getRight());
            cout << node->getElement() << " ";
        }
    }

    //Return true or false if the element is in the tree
    bool find(Node* node, const T& element) {
        if (!node) return false;
        if (element == node->getElement()) return true;
        return element < node->getElement() ? find(node->getLeft(), element) : find(node->getRight(), element); 
    }

    //Find the smallest element in the tree
    Node* findMin(Node* node) {
        assert(node != nullptr);
        while (node->getLeft()) {
            node = node->getLeft();
        }
        return node;
    }

    Node* remove(Node* node, const T& value) {
        assert(size_ != 0);

        if (!node){ return nullptr; }
    
        if (value < node->getElement()){
            node->setLeft(remove(node->getLeft(), value));
        } else if (value > node->getElement()){
            node->setRight(remove(node->getRight(), value)); 
        } else {
            //If the node has 1 child
            if (!node->getLeft() || !node->getRight()) { 
                Node* temp = node->getLeft() ? node->getLeft() : node->getRight();
                delete node;
                return temp;
            }
            //Find the minimum successor of the right subtree
            Node* minNode = findMin(node->getRight());
            //Copy its value
            node->setElement(minNode->getElement());
            //Remove the duplicate
            node->setRight(remove(node->getRight(), minNode->getElement()));
        }
        return node;
    }

    
    public:
    
    //Constructors
    BST(){ root_ = nullptr; size_ = 0; }
    BST(const T& element){ root_ = new Node(element); size_ = 1;}

    void insert(const T& element){ 
        if (!find(element)) { //If the element exist, don't add it
            root_ = insert(root_, element); 
            size_++; 
        }
    }

    void inorder() { inorder(root_); }

    void preorder() { preorder(root_); }

    void postorder() { postorder(root_); }

    bool find(const T& element){ return find(root_, element); }

    void remove(const T& value) { 
        assert(find(root_, value)); // If the element exist, remove it
        root_ = remove(root_, value);
        size_--;
    }
    
    unsigned int size(){ return size_; }

    Node* root(){ return root_; }

};

void Prueba() {
    BST<int> tree;
    srand(time(nullptr)); 
    for (int i = 1; i <= 10; i++) {
        tree.insert(rand() % 50);
        tree.insert(i);
    }
    cout << "Root: " << tree.root()->getElement() << endl;
    cout << "Inorder:" << endl ;
    tree.inorder();
    cout << endl;
    cout << "Preorder:" << endl ;
    tree.preorder();
    cout << endl;
    cout << "Postorder:" << endl ;
    tree.postorder();
    cout << endl;
    cout << "Size:" << endl ;
    cout << tree.size() << endl;
    tree.remove(2);
    cout << "Delete number 2" << endl ;
    cout << "Inorder:" << endl ;
    tree.inorder();
    cout << endl;
    cout << "Size:" << endl ;
    cout << tree.size() << endl;
}

int main() {
    Prueba();
    return 0;
}