# Stack Implementation (`stack.cc`)

A generic LIFO stack built on top of the custom dynamic array from `../Vector/vector.h`. The file contains a single class template, `Stack<T>`, whose only state is a `Vector<T>` member (`data`) used as the backing store.

## File Layout

```
stack.cc
├── #include <iostream>, <cassert>
├── #include "../Vector/vector.h"
├── template<typename T> class Stack
│   ├── private stack state (data)
│   └── public stack operations (delegate to Vector)
└── int main()  // demo: push / pop / top / size / empty / print
```

## Class `Stack<T>`

A wrapper around a `Vector<T>` that only exposes LIFO operations. The **back** of the vector holds the **top** of the stack, so pushes and pops happen at the end of the vector.

### Stack state

- `data` — `Vector<T>` holding the elements; `data.back()` is the top of the stack

### Public operations

| Method              | Purpose                                             | Backed by            | Complexity |
|---------------------|-----------------------------------------------------|----------------------|------------|
| `Stack()`           | Default constructor (empty stack)                   | —                    | O(1)       |
| `push(const T&)`    | Adds an element on **top**                          | `data.push_back`     | O(1) amortized |
| `pop()`             | Removes the **top** element                         | `data.pop_back`      | O(1)       |
| `top()`             | Returns the top element (mutable and const overloads) | `data.at(size()-1)` | O(1)     |
| `empty()`           | `true` if the stack has no elements                 | `data.empty()`       | O(1)       |
| `size()`            | Returns the number of elements                      | `data.size()`        | O(1)       |
| `print()`           | Prints elements from top to bottom                  | `data.at(i)`         | O(n)       |

### Preconditions and safety

- `pop()` and `top()` call `assert(!data.empty())`, so they abort in debug builds if the stack is empty.
- `top()` has two overloads: `const T& top() const` for read-only access and `T& top()` so the top element can be modified in place (e.g. `stack.top() = 10;`).
- `print()` walks the vector backwards, from `size()-1` down to `0`, which is the natural top-to-bottom order for a stack.

## Dependency: `Vector<T>` (`../Vector/vector.h`)

The stack relies on the custom dynamic array, not `std::vector`. The relevant members of `Vector<T>` are:

| `Vector<T>` member | Role in the stack                                  |
|--------------------|----------------------------------------------------|
| `push_back(T)`     | Appends on top of the stack (resizes when needed)   |
| `pop_back()`       | Removes the top of the stack                        |
| `at(pos)`          | Bounds-checked access; `at(size()-1)` is the top    |
| `size()`           | Number of stacked elements                          |
| `empty()`          | Emptiness check                                     |

`Vector<T>` grows geometrically (`resize()` multiplies capacity by 1.5), which is what makes `Stack::push` amortized O(1).

### Design notes

- **LIFO mapping:** the stack's top is the vector's last element, so `push`/`pop`/`top` are all constant time.
- **Encapsulation:** `Stack` never exposes the vector directly; all access goes through its public methods.
- **Genericity:** `Stack` is a template, so any type `T` that the `Vector` supports (default-constructible, copy-assignable, and streamable) can be stacked.

### Usage sketch

```cpp
Stack<int> s;
s.push(1);
s.push(2);
s.push(3);
s.print();   // 3 2 1
s.pop();     // removes 3
s.top();     // 2
s.top() = 10;
s.print();   // 10 2 1
s.size();    // 3
s.empty();   // 0
```

### Compiling and running

```bash
g++ -std=c++20 -o stack Stack/stack.cc
./stack
```

Expected output of the `main()` demo:

```
5 4 3 2 1
4 3 2 1
4
10 3 2 1
4
0
```
