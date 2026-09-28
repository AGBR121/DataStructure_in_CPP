# Binary Search Tree Implementation (`binary_tree.cc`)

A generic binary search tree (BST) built from scratch with raw `new`/`delete`. The file contains a single class template, `BST<T>`, with a private nested `Node` type, a `Node*` root, and an element counter. There are no external dependencies.

## File Layout

```
binary_tree.cc
├── #include <iostream>, <cassert>, <chrono>
├── template<typename T> class BST
│   ├── private nested class Node (element_, left_, right_ + getters/setters)
│   ├── tree state (root_, size_)
│   ├── private recursive helpers (insert, find, findMin, remove, traversals)
│   └── public tree operations (delegate to the helpers)
├── void Prueba()  // demo: random inserts + traversals + remove
└── int main()     // calls Prueba()
```

## Class `BST<T>`

A binary search tree that keeps the invariant **left subtree values < node value < right subtree values**. Every insertion, lookup, and deletion walks down that invariant, so each is logarithmic when the tree stays balanced.

### Nested class `Node`

| Member             | Description                                        |
|--------------------|----------------------------------------------------|
| `element_`         | Stored value of type `T`                           |
| `left_`            | Pointer to the left child (`nullptr` when absent)  |
| `right_`           | Pointer to the right child (`nullptr` when absent) |
| `Node()`           | Default constructor (`T()`, both children `nullptr`) |
| `Node(const T&)`    | Constructor from an element                        |
| `getElement()`     | Returns `element_`                                |
| `setElement(const T&)` | Reassigns `element_`                          |
| `getLeft()`        | Returns `left_`                                    |
| `setLeft(Node*)`   | Reassigns the `left_` pointer                      |
| `getRight()`       | Returns `right_`                                   |
| `setRight(Node*)`  | Reassigns the `right_` pointer                     |

### Tree state

- `root_` — `Node*` pointer to the current root, `nullptr` while the tree is empty
- `size_` — `unsigned int` number of elements currently stored

### Private recursive helpers

| Helper                      | Purpose                                                                 | Complexity |
|-----------------------------|-------------------------------------------------------------------------|------------|
| `insert(Node*, T)`          | Returns the subtree root after inserting `value`; descends left if `value < node`, right if `value > node`, and returns the node untouched if the value is already present | O(h) |
| `find(Node*, const T&)`     | Returns `true` if `element` is in the subtree, following the BST invariant to prune the search | O(h) |
| `findMin(Node*)`            | Returns the leftmost node of the subtree (the minimum element)          | O(h) |
| `remove(Node*, const T&)`   | Returns the subtree root after deleting `value`; handles the 0-, 1-, and 2-child cases | O(h) |
| `inorder(Node*)`            | Prints Left-Root-Right                                                | O(n) |
| `preorder(Node*)`           | Prints Root-Left-Right                                                | O(n) |
| `postorder(Node*)`          | Prints Left-Right-Root                                                | O(n) |

`h` is the height of the subtree, which is O(log n) for a balanced tree and degenerates to O(n) for a completely skewed one.

### Removal cases

`remove(Node*, const T&)` handles the three standard shapes:

- **No children** — the node is deleted and `nullptr` is returned in its place.
- **One child** — the single child is returned to the parent, splicing the node out.
- **Two children** — `findMin` locates the in-order successor (smallest value in the right subtree), its value is copied into the node, and the successor is then removed from the right subtree. This keeps the subtree-root-returning recursion valid without needing a parent pointer.

### Public operations

| Method                    | Purpose                                                     | Backed by                | Complexity |
|---------------------------|-------------------------------------------------------------|--------------------------|------------|
| `BST()`                   | Default constructor (empty tree)                            | —                        | O(1)       |
| `BST(const T&)`           | Constructs a tree holding a single element                  | `new Node(element)`      | O(1)       |
| `insert(const T&)`        | Inserts an element, ignoring duplicates                     | `find` + `insert(root_)` | O(h) + O(h) |
| `find(const T&)`          | `true` if the element exists in the tree                     | `find(root_)`            | O(h)       |
| `remove(const T&)`        | Deletes an element that exists in the tree                  | `remove(root_)`          | O(h)       |
| `inorder()`               | Prints the tree in Left-Root-Right (sorted) order            | `inorder(root_)`         | O(n)       |
| `preorder()`              | Prints the tree in Root-Left-Right order                     | `preorder(root_)`        | O(n)       |
| `postorder()`             | Prints the tree in Left-Right-Root order                     | `postorder(root_)`       | O(n)       |
| `size()`                  | Returns the number of elements                               | `size_`                  | O(1)       |
| `root()`                  | Returns the `Node*` root, for manual inspection              | `root_`                  | O(1)       |

### Preconditions and safety

- `remove()` calls `assert(find(root_, value))` first, so removing an absent element aborts in debug builds instead of corrupting the tree.
- `inorder`, `preorder`, and `postorder` call `assert(root_ != nullptr)`, so traversing an empty tree aborts in debug builds.
- `findMin` calls `assert(node != nullptr)`, guarding the two-children removal path.
- `insert` performs a `find` before inserting, so duplicate values are silently dropped and `size_` stays consistent with the real node count.
- `size_` is maintained manually by `insert` (`++`) and `remove` (`--`); it is not recomputed from the tree, so every mutation must go through those two methods.
- The tree is **non-recursive-free by design**: `insert`, `find`, and `remove` are recursive, so a skewed tree of `n` nodes uses O(n) stack depth.

### Design notes

- **Recursive helpers return the subtree root** instead of taking a parent pointer, which keeps the tree structure entirely private — no parent links and no `Node*` bookkeeping in the callers.
- **Traversal order encodes tree shape:** inorder yields the sorted sequence, preorder yields a serialization suitable for reconstructing the tree, and postorder is the natural order for freeing a tree bottom-up.
- **Genericity:** `BST` is a template, so any `T` that is default-constructible, copy-assignable, streamable, and comparable with `==`, `<`, and `>` can be stored.
- **Encapsulation leak:** `root()` exposes the internal `Node*` (and `Node` is a private nested type), so external code can technically walk the raw pointers. It exists so the demo can print the root value and hand-write node tests.

### Usage sketch

```cpp
BST<int> tree;
tree.insert(30);
tree.insert(10);
tree.insert(40);
tree.preorder();     // 30 10 40
tree.inorder();      // 10 30 40
tree.postorder();    // 10 40 30
tree.find(40);       // true
tree.find(99);       // false
tree.size();         // 3
tree.remove(30);     // node with two children: successor 40 takes its place
tree.inorder();      // 10 40
tree.size();         // 2
```

### Compiling and running

```bash
g++ -std=c++20 -o binary_tree Trees/Binary_tree/binary_tree.cc
./binary_tree
```

The `Prueba()` demo seeds the RNG with `time(nullptr)` and inserts `rand() % 50` alongside `1..10`, so **the output differs on every run**. One possible run:

```
Root: 30
Inorder:
1 2 3 4 5 6 7 8 9 10 13 24 29 30 32 34 40 48
Preorder:
30 1 2 24 3 4 5 8 6 7 13 9 10 29 34 32 48 40
Postorder:
7 6 10 9 13 8 5 4 3 29 24 2 1 32 40 48 34 30
Size:
18
Delete number 2
Inorder:
1 3 4 5 6 7 8 9 10 13 24 29 30 32 34 40 48
Size:
17
```

Note that `<chrono>` is included but `time` comes from `<ctime>`, which the file does not include; it works in practice because libstdc++ pulls `<ctime>` in transitively through other headers. Adding `<ctime>` explicitly would make the code standard-conforming.
