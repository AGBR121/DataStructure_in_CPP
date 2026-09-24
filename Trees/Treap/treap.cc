#include <iostream>
#include <cassert>
#include <chrono>

using namespace std;

template<typename T>
class Treap {
    private:
    class Node {
        private:
        T element_;
        int priority_;
        Node* left_;
        Node* right_;

        public:
        Node() {
            element_ = T();
            priority_ = rand();
            left_ = nullptr;
            right_ = nullptr;
        }

        Node(const T& data) {
            element_ = data;
            priority_ = rand();
            left_ = nullptr;
            right_ = nullptr;
        }

        T getElement() const { return element_; }
        void setElement(const T& element) { element_ = element; }

        int getPriority() const { return priority_; }
        void setPriority(int priority) { priority_ = priority; }

        Node* getLeft() const { return left_; }
        void setLeft(Node* left) { left_ = left; }

        Node* getRight() const { return right_; }
        void setRight(Node* right) { right_ = right; }
    };

    Node* root_;
    unsigned int size_;

    void rotateRight(Node*& node) {
        Node* leftChild = node->getLeft();
        node->setLeft(leftChild->getRight());
        leftChild->setRight(node);
        node = leftChild;
    }

    void rotateLeft(Node*& node) {
        Node* rightChild = node->getRight();
        node->setRight(rightChild->getLeft());
        rightChild->setLeft(node);
        node = rightChild;
    }

    void inorder(Node* node) {
        assert(root_ != nullptr);
        if (node) {
            inorder(node->getLeft());
            cout << node->getElement() << " ";
            inorder(node->getRight());
        }
    }

    void preorder(Node* node) {
        assert(root_ != nullptr);
        if (node) {
            cout << node->getElement() << " ";
            preorder(node->getLeft());
            preorder(node->getRight());
        }
    }

    void postorder(Node* node) {
        assert(root_ != nullptr);
        if (node) {
            postorder(node->getLeft());
            postorder(node->getRight());
            cout << node->getElement() << " ";
        }
    }

    bool find(Node* node, const T& element) {
        if (!node) return false;
        if (element == node->getElement()) return true;
        return element < node->getElement() ? find(node->getLeft(), element) : find(node->getRight(), element);
    }

    Node* insert(Node* node, const T& value) {
        if (!node) return new Node(value);

        if (value < node->getElement()) {
            node->setLeft(insert(node->getLeft(), value));
            if (node->getLeft()->getPriority() > node->getPriority()) {
                rotateRight(node);
            }
        } else if (value > node->getElement()) {
            node->setRight(insert(node->getRight(), value));
            if (node->getRight()->getPriority() > node->getPriority()) {
                rotateLeft(node);
            }
        }
        return node;
    }

    Node* remove(Node* node, const T& value) {
        if (!node) return nullptr;

        if (value < node->getElement()) {
            node->setLeft(remove(node->getLeft(), value));
        } else if (value > node->getElement()) {
            node->setRight(remove(node->getRight(), value));
        } else {
            if (!node->getLeft()) {
                Node* temp = node->getRight();
                delete node;
                return temp;
            } else if (!node->getRight()) {
                Node* temp = node->getLeft();
                delete node;
                return temp;
            } else if (node->getLeft()->getPriority() > node->getRight()->getPriority()) {
                rotateRight(node);
                node->setRight(remove(node->getRight(), value));
            } else {
                rotateLeft(node);
                node->setLeft(remove(node->getLeft(), value));
            }
        }
        return node;
    }

    public:
    Treap() : root_(nullptr), size_(0) {}
    Treap(const T& element) {
        root_ = new Node(element);
        size_ = 1;
    }

    void insert(const T& element) {
        if (!find(root_, element)) { // If the element doesn't exist, add it
            root_ = insert(root_, element);
            size_++;
        }
    }
    void remove(const T& value) {
        assert(find(root_, value)); // If the element exist, remove it
        root_ = remove(root_, value);
        size_--;
    }

    void inorder() { inorder(root_); }
    void preorder() { preorder(root_); }
    void postorder() { postorder(root_); }

    bool find(const T& element) { return find(root_, element); }

    unsigned int size() const { return size_; }
    Node* root() const { return root_; }

};

void testTreap() {
    Treap<int> treap;
    srand(time(nullptr)); 
    for (int i = 1; i <= 20; i++) {
        treap.insert(rand() % 200);
        treap.insert(i);
    }

    cout << "Root: " <<treap.root()->getElement() << endl; // Should print the root element

    cout << "Inorder: ";
    treap.inorder();
    cout << endl;

    cout << "Preorder: ";
    treap.preorder();
    cout << endl;

    cout << "Postorder: ";
    treap.postorder();
    cout << endl;

    cout << "Size: " << treap.size() << endl;

    treap.remove(20);
    cout << "Delete 20: ";
    treap.inorder();
    cout << endl;
    
    cout << "Size of Treap: " << treap.size() << endl;
}

int main() {
    testTreap();
    return 0;
}