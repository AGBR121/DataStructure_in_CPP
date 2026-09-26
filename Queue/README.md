# Queue Implementation (`queue.cc`)

A generic FIFO queue built on top of a doubly-anchored singly linked list. The file contains two class templates: `List<T>` (the underlying storage) and `Queue<T>` (the public queue interface).

## File Layout

```
queue.cc
├── #include <iostream>, <cassert>
├── template<typename T> class List
│   ├── private nested class Node
│   ├── list state (first_, last_, size_)
│   └── public list operations
└── template<typename T> class Queue
    └── public queue operations (delegate to List)
```

## Class `List<T>`

A singly linked list with head and tail pointers, used as the backing store for the queue.

### Nested class `Node`

| Member        | Description                              |
|---------------|------------------------------------------|
| `data_`       | Stored element of type `T`               |
| `next_`       | Pointer to the following `Node`          |
| `Node()`      | Default constructor (`T()`, `nullptr`)   |
| `Node(const T&)` | Constructor from an element          |
| `setNext()`   | Reassigns the `next_` pointer            |
| `getNext()`   | Returns `next_`                          |
| `getData()`   | Returns `data_` (mutable and const overloads) |

### List state

- `first_` — pointer to the head node
- `last_` — pointer to the tail node
- `size_` — number of elements

### Public operations

| Method              | Purpose                                      | Complexity |
|---------------------|----------------------------------------------|------------|
| `size()`            | Returns element count                        | O(1)       |
| `empty()`           | `true` if the list has no elements           | O(1)       |
| `push_front(T)`     | Inserts at the head                          | O(1)       |
| `push_back(T)`      | Inserts at the tail                          | O(1)       |
| `pop_front()`       | Removes the head node                        | O(1)       |
| `pop_back()`        | Removes the tail node (traverses the list)   | O(n)       |
| `at(pos)`           | Access by index (mutable/const overloads)    | O(n)       |
| `insert(index, T)`  | Inserts at an arbitrary position             | O(n)       |
| `remove(index)`     | Deletes at an arbitrary position             | O(n)       |
| `find(T)`           | Returns the index of an element, or `-1`     | O(n)       |
| `sort()`            | Merge sort (`MergeSort` / `getMiddle` / `Merge`) | O(n log n) |
| `print()`           | Prints elements space-separated to stdout    | O(n)       |

### Sorting helpers (private-style helpers used by `sort()`)

- **`MergeSort(Node*)`** — recursively splits the list at the middle and merges the sorted halves.
- **`getMiddle(Node*)`** — finds the midpoint with the slow/fast (tortoise and hare) pointer technique.
- **`Merge(Node*, Node*)`** — merges two sorted sublists by comparing node values.

## Class `Queue<T>`

The queue wraps a `List<T>` member (`list`) and exposes FIFO operations only:

| Method          | Behavior                              | Backed by            |
|-----------------|---------------------------------------|----------------------|
| `enqueue(T)`    | Adds an element to the **back**       | `list.push_back`     |
| `dequeue()`     | Removes the **front** element         | `list.pop_front`     |
| `peek()`        | Returns the front element             | `list.at(0)`         |
| `rear()`        | Returns the back element              | `list.at(size()-1)`  |
| `size()`        | Returns the queue size                | `list.size()`        |
| `empty()`       | Checks if the queue is empty          | `list.empty()`       |
| `print()`       | Prints the queue                      | `list.print`         |

### Design notes

- **FIFO mapping:** `enqueue` → tail insertion, `dequeue` → head removal, giving O(1) for both operations.
- **Encapsulation:** `Queue` never exposes the list directly; all access goes through its public methods.
- **Genericity:** Both classes are templates, so any type `T` with `==`, `<`, and stream output support can be used.

### Usage sketch

```cpp
Queue<int> q;
q.enqueue(1);
q.enqueue(2);
q.peek();    // 1
q.dequeue(); // removes 1
q.rear();    // 2
```
