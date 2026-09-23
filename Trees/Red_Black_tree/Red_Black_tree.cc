#include <iostream>
#include <cassert>
#include <chrono>

using namespace std;

// Red-Black Tree implementation: a self-balancing binary search tree where each
// node is colored red or black to keep the tree height O(log n).
// Properties: root is black, no two consecutive reds, every path has same black count.
template<typename T>
class RBT {
private:
    // Node holds the element, its color and the pointers to parent/children
    class Node {
        T element_;
        bool color; // Red = 1, Black = 0
        Node* left_;
        Node* right_;
        Node* parent_;
    public:
        // Default constructor: empty node, colored red by default
        Node() {
            element_ = T();
            left_ = nullptr;
            right_ = nullptr;
            parent_ = nullptr;
            color = 1; // new nodes start red (fixed later by fixInsert)
        }

        // Constructor with data: new nodes start red
        Node(const T& data) {
            element_ = data;
            left_ = nullptr;
            right_ = nullptr;
            parent_ = nullptr;
            color = 1;
        }

        // Getter and setter of the Node class
        T getElement() const { return element_; }
        void setElement(const T& element) { element_ = element; }

        Node* getLeft() const { return left_; }
        void setLeft(Node* left) { left_ = left; }

        Node* getRight() const { return right_; }
        void setRight(Node* right) { right_ = right; }

        Node* getParent() const { return parent_; }
        void setParent(Node* parent) { parent_ = parent; }

        bool isRed() const { return color; } // Return true if the node is red
        void setColor(bool c) { color = c; }
    };

    Node* root_; // Root of the tree
    unsigned int size_ = 0; // Number of elements in the tree

    // Rotate left around pivot: pivot's right child moves up, pivot goes down-left.
    // Used to fix red-red violations on the right side.
    void leftRotate(Node* pivot) {
        Node* child = pivot->getRight(); // the child that will move up
        pivot->setRight(child->getLeft()); // pivot adopts the child's left subtree
        if (child->getLeft()) {
            child->getLeft()->setParent(pivot); // reparent the moved subtree
        }
        child->setParent(pivot->getParent()); // child takes pivot's place

        if (!pivot->getParent()) {
            root_ = child; // if pivot was the root, the child is the new root
        } else if (pivot == pivot->getParent()->getLeft()) {
            pivot->getParent()->setLeft(child); // link from the parent
        } else {
            pivot->getParent()->setRight(child);
        }

        child->setLeft(pivot); // pivot becomes the left child
        pivot->setParent(child);
    }

    // Rotate right around pivot: pivot's left child moves up, pivot goes down-right.
    // Used to fix red-red violations on the left side.
    void rightRotate(Node* pivot) {
        Node* child = pivot->getLeft(); // the child that will move up
        pivot->setLeft(child->getRight()); // pivot adopts the child's right subtree
        if (child->getRight()) {
            child->getRight()->setParent(pivot); // reparent the moved subtree
        }
        child->setParent(pivot->getParent()); // child takes pivot's place

        if (!pivot->getParent()) {
            root_ = child; // if pivot was the root, the child is the new root
        } else if (pivot == pivot->getParent()->getRight()) {
            pivot->getParent()->setRight(child); // link from the parent
        } else {
            pivot->getParent()->setLeft(child);
        }

        child->setRight(pivot); // pivot becomes the right child
        pivot->setParent(child);
    }


    // Restore the Red-Black properties after an insertion.
    // Walks up the tree fixing red-red conflicts by recoloring or rotating.
    void fixInsert(Node* node) {
        while (node->getParent() && node->getParent()->isRed()) { // while parent is red
            Node* parent = node->getParent();
            Node* grandparent = parent->getParent();

            if (parent == grandparent->getLeft()) { // parent is the left child
                Node* uncle = grandparent->getRight();
                if (uncle && uncle->isRed()) {
                    // Case 1: uncle is red -> recolor and move the problem up
                    parent->setColor(false);
                    uncle->setColor(false);
                    grandparent->setColor(true);
                    node = grandparent;
                } else {
                    // Case 2/3: uncle is black -> rotate to fix
                    if (node == parent->getRight()) {
                        node = parent;
                        leftRotate(node); // make it a left-left case first
                    }
                    node->getParent()->setColor(false);
                    grandparent->setColor(true);
                    rightRotate(grandparent); // finish the fix
                }
            } else { // parent is the right child (mirrored cases)
                Node* uncle = grandparent->getLeft();
                if (uncle && uncle->isRed()) {
                    // Case 1: uncle is red -> recolor and move the problem up
                    parent->setColor(false);
                    uncle->setColor(false);
                    grandparent->setColor(true);
                    node = grandparent;
                } else {
                    // Case 2/3: uncle is black -> rotate to fix
                    if (node == parent->getLeft()) {
                        node = parent;
                        rightRotate(node); // make it a right-right case first
                    }
                    node->getParent()->setColor(false);
                    grandparent->setColor(true);
                    leftRotate(grandparent); // finish the fix
                }
            }
        }
        root_->setColor(false); // the root is always black
    }

    // Replace old_node with new_node keeping the parent links consistent
    void transplant(Node* old_node, Node* new_node) {
        if (!old_node->getParent()) {
            root_ = new_node; // old_node was the root
        } else if (old_node == old_node->getParent()->getLeft()) {
            old_node->getParent()->setLeft(new_node);
        } else {
            old_node->getParent()->setRight(new_node);
        }
        if (new_node) {
            new_node->setParent(old_node->getParent()); // link the new node up
        }
    }

    // Find the smallest element in the subtree (leftmost node)
    Node* minimum(Node* node) const {
        while (node->getLeft() != nullptr) {
            node = node->getLeft(); // keep going left
        }
        return node;
    }

    // Restore the Red-Black properties after a deletion.
    // Fixes the "double black" deficiency by rotations and recoloring.
    void fixDelete(Node* node) {
        while (node != root_ && (!node || !node->isRed())) { // while node is black (or null)
            Node* parent = node ? node->getParent() : nullptr;
            if (!parent) break;

            if (node == parent->getLeft()) { // node is the left child
                Node* sibling = parent->getRight();
                if (sibling && sibling->isRed()) {
                    // Case: red sibling -> rotate to make it black
                    sibling->setColor(false);
                    parent->setColor(true);
                    leftRotate(parent);
                    sibling = parent->getRight();
                }

                if ((!sibling->getLeft() || !sibling->getLeft()->isRed()) &&
                    (!sibling->getRight() || !sibling->getRight()->isRed())) {
                    // Case: both nephews black -> recolor sibling red and move up
                    sibling->setColor(true);
                    node = parent;
                } else {
                    if (!sibling->getRight() || !sibling->getRight()->isRed()) {
                        // Case: right nephew black -> rotate sibling right first
                        if (sibling->getLeft()) sibling->getLeft()->setColor(false);
                        sibling->setColor(true);
                        rightRotate(sibling);
                        sibling = parent->getRight();
                    }
                    // Case: right nephew red -> recolor and rotate parent left
                    sibling->setColor(parent->isRed());
                    parent->setColor(false);
                    if (sibling->getRight()) sibling->getRight()->setColor(false);
                    leftRotate(parent);
                    node = root_; // the tree is balanced
                }
            } else { // node is the right child (mirrored cases)
                Node* sibling = parent->getLeft();
                if (sibling && sibling->isRed()) {
                    // Case: red sibling -> rotate to make it black
                    sibling->setColor(false);
                    parent->setColor(true);
                    rightRotate(parent);
                    sibling = parent->getLeft();
                }

                if ((!sibling->getLeft() || !sibling->getLeft()->isRed()) &&
                    (!sibling->getRight() || !sibling->getRight()->isRed())) {
                    // Case: both nephews black -> recolor sibling red and move up
                    sibling->setColor(true);
                    node = parent;
                } else {
                    if (!sibling->getLeft() || !sibling->getLeft()->isRed()) {
                        // Case: left nephew black -> rotate sibling left first
                        if (sibling->getRight()) sibling->getRight()->setColor(false);
                        sibling->setColor(true);
                        leftRotate(sibling);
                        sibling = parent->getLeft();
                    }
                    // Case: left nephew red -> recolor and rotate parent right
                    sibling->setColor(parent->isRed());
                    parent->setColor(false);
                    if (sibling->getLeft()) sibling->getLeft()->setColor(false);
                    rightRotate(parent);
                    node = root_; // the tree is balanced
                }
            }
        }

        if (node) node->setColor(false); // the root is always black
    }

    // Physically remove a node from the tree, replacing it with its successor if needed
    void deleteNode(Node* nodeToDelete) {
        Node* replacementNode = nodeToDelete;
        bool wasReplacementNodeRed = replacementNode->isRed(); // remember its color
        Node* nodeToFix = nullptr;

        if (nodeToDelete->getLeft() == nullptr) {
            // No left child -> replace with the right subtree
            nodeToFix = nodeToDelete->getRight();
            transplant(nodeToDelete, nodeToFix);
        } else if (nodeToDelete->getRight() == nullptr) {
            // No right child -> replace with the left subtree
            nodeToFix = nodeToDelete->getLeft();
            transplant(nodeToDelete, nodeToFix);
        } else {
            // Two children -> use the inorder successor (smallest of the right subtree)
            replacementNode = minimum(nodeToDelete->getRight());
            wasReplacementNodeRed = replacementNode->isRed();
            nodeToFix = replacementNode->getRight();

            if (replacementNode->getParent() == nodeToDelete) {
                // The successor is a direct child of the node to delete
                if (nodeToFix != nullptr) {
                    nodeToFix->setParent(replacementNode);
                }
            } else {
                // Move the successor out of its old position
                transplant(replacementNode, replacementNode->getRight());
                replacementNode->setRight(nodeToDelete->getRight());
                if (replacementNode->getRight()) {
                    replacementNode->getRight()->setParent(replacementNode);
                }
            }

            // Put the successor in the place of the deleted node
            transplant(nodeToDelete, replacementNode);
            replacementNode->setLeft(nodeToDelete->getLeft());
            if (replacementNode->getLeft()) {
                replacementNode->getLeft()->setParent(replacementNode);
            }
            replacementNode->setColor(nodeToDelete->isRed()); // inherit the color
        }

        delete nodeToDelete; // free the memory

        // Only fix if we removed a black node (red deletions don't break properties)
        if (!wasReplacementNodeRed && nodeToFix != nullptr) {
            fixDelete(nodeToFix);
        }
    }

    // Left-Root-Right (prints the elements in ascending order)
    void inorder(Node* node) const {
        assert(root_ != nullptr);
        if (node != nullptr) {
            inorder(node->getLeft());
            cout << node->getElement() << " ";
            inorder(node->getRight());
        }
    }

    // Root-Left-Right
    void preorder(Node* node) const {
        assert(root_ != nullptr);
        if (node != nullptr) {
            cout << node->getElement() << " ";
            preorder(node->getLeft());
            preorder(node->getRight());
        }
    }

    // Left-Right-Root
    void postorder(Node* node) const {
        assert(root_ != nullptr);
        if (node != nullptr) {
            postorder(node->getLeft());
            postorder(node->getRight());
            cout << node->getElement() << " ";
        }
    }

    // Return true or false if the element is in the tree
    bool find(Node* node, const T& element) {
        if (!node) return false;
        if (element == node->getElement()) return true;
        return element < node->getElement() ? find(node->getLeft(), element) : find(node->getRight(), element);
    }

    // Search the tree and remove the node with the given value
    void delete_node(const T& value) {
        Node* nodeToDelete = root_;
        while (nodeToDelete != nullptr) {
            if (value == nodeToDelete->getElement()) {
                break; // found it
            } else if (value < nodeToDelete->getElement()) {
                nodeToDelete = nodeToDelete->getLeft(); // go left
            } else {
                nodeToDelete = nodeToDelete->getRight(); // go right
            }
        }

        if (nodeToDelete == nullptr) return; // value not in the tree
        deleteNode(nodeToDelete);
        size_--;
    }

    // Insert a new node as a leaf (like a normal BST) and then fix the colors
    void add_node(const T& value) {
        Node* newNode = new Node(value);
        Node* parentNode = nullptr;
        Node* currentNode = root_;

        // Walk down to the leaf position
        while (currentNode != nullptr) {
            parentNode = currentNode;
            if (newNode->getElement() < currentNode->getElement()) {
                currentNode = currentNode->getLeft(); // go left
            } else {
                currentNode = currentNode->getRight(); // go right
            }
        }

        newNode->setParent(parentNode);
        if (parentNode == nullptr) {
            root_ = newNode; // the tree was empty
        } else if (newNode->getElement() < parentNode->getElement()) {
            parentNode->setLeft(newNode); // link as left child
        } else {
            parentNode->setRight(newNode); // link as right child
        }

        fixInsert(newNode); // restore the Red-Black properties
        size_++;
    }


public:
    // Constructors
    RBT() { root_ = nullptr; size_ = 0; }

    RBT(const T& element) {
        root_ = new Node(element);
        root_->setColor(0); // the root is black
        size_ = 1;
    }

    Node* root() const { return root_; } // Return the root of the tree

    // Insert an element (duplicates are ignored)
    void insert(const T& data) {
        if (find(data)) return; // If the element exists, don't add it
        add_node(data);
    }

    // Return true or false if the element is in the tree
    bool find(const T& value) {
        return find(root_, value);
    }

    // Remove an element from the tree
    void remove(const T& value) {
        assert(find(value)); // If the element exists, remove it
        delete_node(value);
    }

    void inorder() const { inorder(root_); } // Print in ascending order
    void preorder() const { preorder(root_); }
    void postorder() const { postorder(root_); }

    unsigned int size() const { return size_; } // Return the number of elements
};

// Test the Red-Black Tree implementation
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
