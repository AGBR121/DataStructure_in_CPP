#include <iostream>
#include <cassert>
#include <chrono>

using namespace std;

template<typename T>
class RBT {
private:
    class Node {
        T element_;
        bool color; // Red = 1, Black = 0
        Node* left_;
        Node* right_;
        Node* parent_;
    public:
        Node() {
            element_ = T();
            left_ = nullptr;
            right_ = nullptr;
            parent_ = nullptr;
            color = 1;
        }

        Node(const T& data) {
            element_ = data;
            left_ = nullptr;
            right_ = nullptr;
            parent_ = nullptr;
            color = 1;
        }

        T getElement() const { return element_; }
        void setElement(const T& element) { element_ = element; }

        Node* getLeft() const { return left_; }
        void setLeft(Node* left) { left_ = left; }

        Node* getRight() const { return right_; }
        void setRight(Node* right) { right_ = right; }

        Node* getParent() const { return parent_; }
        void setParent(Node* parent) { parent_ = parent; }

        bool isRed() const { return color; }
        void setColor(bool c) { color = c; }
    };

    Node* root_;
    unsigned int size_ = 0;

    void leftRotate(Node* pivot) {
        Node* child = pivot->getRight();
        pivot->setRight(child->getLeft());
        if (child->getLeft()) {
            child->getLeft()->setParent(pivot);
        }
        child->setParent(pivot->getParent());

        if (!pivot->getParent()) {
            root_ = child;
        } else if (pivot == pivot->getParent()->getLeft()) {
            pivot->getParent()->setLeft(child);
        } else {
            pivot->getParent()->setRight(child);
        }

        child->setLeft(pivot);
        pivot->setParent(child);
    }

    void rightRotate(Node* pivot) {
        Node* child = pivot->getLeft();
        pivot->setLeft(child->getRight());
        if (child->getRight()) {
            child->getRight()->setParent(pivot);
        }
        child->setParent(pivot->getParent());

        if (!pivot->getParent()) {
            root_ = child;
        } else if (pivot == pivot->getParent()->getRight()) {
            pivot->getParent()->setRight(child);
        } else {
            pivot->getParent()->setLeft(child);
        }

        child->setRight(pivot);
        pivot->setParent(child);
    }


    void fixInsert(Node* node) {
        while (node->getParent() && node->getParent()->isRed()) {
            Node* parent = node->getParent();
            Node* grandparent = parent->getParent();

            if (parent == grandparent->getLeft()) {
                Node* uncle = grandparent->getRight();
                if (uncle && uncle->isRed()) {
                    parent->setColor(false);
                    uncle->setColor(false);
                    grandparent->setColor(true);
                    node = grandparent;
                } else {
                    if (node == parent->getRight()) {
                        node = parent;
                        leftRotate(node);
                    }
                    node->getParent()->setColor(false);
                    grandparent->setColor(true);
                    rightRotate(grandparent);
                }
            } else {
                Node* uncle = grandparent->getLeft();
                if (uncle && uncle->isRed()) {
                    parent->setColor(false);
                    uncle->setColor(false);
                    grandparent->setColor(true);
                    node = grandparent;
                } else {
                    if (node == parent->getLeft()) {
                        node = parent;
                        rightRotate(node);
                    }
                    node->getParent()->setColor(false);
                    grandparent->setColor(true);
                    leftRotate(grandparent);
                }
            }
        }
        root_->setColor(false);
    }

    void transplant(Node* old_node, Node* new_node) {
        if (!old_node->getParent()) {
            root_ = new_node;
        } else if (old_node == old_node->getParent()->getLeft()) {
            old_node->getParent()->setLeft(new_node);
        } else {
            old_node->getParent()->setRight(new_node);
        }
        if (new_node) {
            new_node->setParent(old_node->getParent());
        }
    }

    Node* minimum(Node* node) const {
        while (node->getLeft() != nullptr) {
            node = node->getLeft();
        }
        return node;
    }

    void fixDelete(Node* node) {
        while (node != root_ && (!node || !node->isRed())) {
            Node* parent = node ? node->getParent() : nullptr;
            if (!parent) break;

            if (node == parent->getLeft()) {
                Node* sibling = parent->getRight();
                if (sibling && sibling->isRed()) {
                    sibling->setColor(false);
                    parent->setColor(true);
                    leftRotate(parent);
                    sibling = parent->getRight();
                }

                if ((!sibling->getLeft() || !sibling->getLeft()->isRed()) &&
                    (!sibling->getRight() || !sibling->getRight()->isRed())) {
                    sibling->setColor(true);
                    node = parent;
                } else {
                    if (!sibling->getRight() || !sibling->getRight()->isRed()) {
                        if (sibling->getLeft()) sibling->getLeft()->setColor(false);
                        sibling->setColor(true);
                        rightRotate(sibling);
                        sibling = parent->getRight();
                    }
                    sibling->setColor(parent->isRed());
                    parent->setColor(false);
                    if (sibling->getRight()) sibling->getRight()->setColor(false);
                    leftRotate(parent);
                    node = root_;
                }
            } else {
                Node* sibling = parent->getLeft();
                if (sibling && sibling->isRed()) {
                    sibling->setColor(false);
                    parent->setColor(true);
                    rightRotate(parent);
                    sibling = parent->getLeft();
                }

                if ((!sibling->getLeft() || !sibling->getLeft()->isRed()) &&
                    (!sibling->getRight() || !sibling->getRight()->isRed())) {
                    sibling->setColor(true);
                    node = parent;
                } else {
                    if (!sibling->getLeft() || !sibling->getLeft()->isRed()) {
                        if (sibling->getRight()) sibling->getRight()->setColor(false);
                        sibling->setColor(true);
                        leftRotate(sibling);
                        sibling = parent->getLeft();
                    }
                    sibling->setColor(parent->isRed());
                    parent->setColor(false);
                    if (sibling->getLeft()) sibling->getLeft()->setColor(false);
                    rightRotate(parent);
                    node = root_;
                }
            }
        }

        if (node) node->setColor(false);
    }

    void deleteNode(Node* nodeToDelete) {
        Node* replacementNode = nodeToDelete;
        bool wasReplacementNodeRed = replacementNode->isRed();
        Node* nodeToFix = nullptr;
    
        if (nodeToDelete->getLeft() == nullptr) {
            nodeToFix = nodeToDelete->getRight();
            transplant(nodeToDelete, nodeToFix);
        } else if (nodeToDelete->getRight() == nullptr) {
            nodeToFix = nodeToDelete->getLeft();
            transplant(nodeToDelete, nodeToFix);
        } else {
            replacementNode = minimum(nodeToDelete->getRight());
            wasReplacementNodeRed = replacementNode->isRed();
            nodeToFix = replacementNode->getRight();
    
            if (replacementNode->getParent() == nodeToDelete) {
                if (nodeToFix != nullptr) {
                    nodeToFix->setParent(replacementNode);
                }
            } else {
                transplant(replacementNode, replacementNode->getRight());
                replacementNode->setRight(nodeToDelete->getRight());
                if (replacementNode->getRight()) {
                    replacementNode->getRight()->setParent(replacementNode);
                }
            }
    
            transplant(nodeToDelete, replacementNode);
            replacementNode->setLeft(nodeToDelete->getLeft());
            if (replacementNode->getLeft()) {
                replacementNode->getLeft()->setParent(replacementNode);
            }
            replacementNode->setColor(nodeToDelete->isRed());
        }
    
        delete nodeToDelete;
    
        if (!wasReplacementNodeRed && nodeToFix != nullptr) {
            fixDelete(nodeToFix);
        }
    }    

    void inorder(Node* node) const {
        assert(root_ != nullptr);
        if (node != nullptr) {
            inorder(node->getLeft());
            cout << node->getElement() << " ";
            inorder(node->getRight());
        }
    }

    void preorder(Node* node) const {
        assert(root_ != nullptr);
        if (node != nullptr) {
            cout << node->getElement() << " ";
            preorder(node->getLeft());
            preorder(node->getRight());
        }
    }

    void postorder(Node* node) const {
        assert(root_ != nullptr);
        if (node != nullptr) {
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

    void delete_node(const T& value) {
        Node* nodeToDelete = root_;
        while (nodeToDelete != nullptr) {
            if (value == nodeToDelete->getElement()) {
                break;
            } else if (value < nodeToDelete->getElement()) {
                nodeToDelete = nodeToDelete->getLeft();
            } else {
                nodeToDelete = nodeToDelete->getRight();
            }
        }
    
        if (nodeToDelete == nullptr) return;
        deleteNode(nodeToDelete);
        size_--;
    }    

    void add_node(const T& value) {
        Node* newNode = new Node(value);
        Node* parentNode = nullptr;
        Node* currentNode = root_;
    
        while (currentNode != nullptr) {
            parentNode = currentNode;
            if (newNode->getElement() < currentNode->getElement()) {
                currentNode = currentNode->getLeft();
            } else {
                currentNode = currentNode->getRight();
            }
        }
    
        newNode->setParent(parentNode);
        if (parentNode == nullptr) {
            root_ = newNode;
        } else if (newNode->getElement() < parentNode->getElement()) {
            parentNode->setLeft(newNode);
        } else {
            parentNode->setRight(newNode);
        }
    
        fixInsert(newNode);
        size_++;
    }
    

public:
    RBT() { root_ = nullptr; size_ = 0; }

    RBT(const T& element) {
        root_ = new Node(element);
        root_->setColor(0);
        size_ = 1;
    }

    Node* root() const { return root_; }

    void insert(const T& data) {
        if (find(data)) return; // If the element exists, don't add it
        add_node(data);
    }

    bool find(const T& value) {
        return find(root_, value);
    }

    void remove(const T& value) {
        assert(find(value)); // If the element exists, remove it
        delete_node(value);
    }

    void inorder() const { inorder(root_); }
    void preorder() const { preorder(root_); }
    void postorder() const { postorder(root_); }

    unsigned int size() const { return size_; }
};

void testRBT() {
    RBT<int> tree;
    srand(time(nullptr));
    for (int i = 1; i <= 10; i++) {
        tree.insert(rand() % 50);
        tree.insert(i);
    }

    cout << "Root: " << tree.root()->getElement() << endl;
    cout << "Inorder:" << endl;
    tree.inorder();
    cout << endl;
    cout << "Preorder:" << endl;
    tree.preorder();
    cout << endl;
    cout << "Postorder:" << endl;
    tree.postorder();
    cout << endl;
    cout << "Size:" << endl;
    cout << tree.size() << endl;

    tree.remove(2);
    cout << "Delete number 2" << endl;
    cout << "Inorder:" << endl;
    tree.inorder();
    cout << endl;
    cout << "Size:" << endl;
    cout << tree.size() << endl;
}

int main() {
    testRBT();
    return 0;
}
