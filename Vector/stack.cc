#include<iostream>
#include<cassert>

#include "Vector.h"

using namespace std;

template<typename T>
class Stack{
    private:
    //Tendrá un vector de tipo T
    Vector<T> data;

    public:
    //Constructor por defecto
    Stack() {}

    // Métodos del stack

    //Método para agregar un elemento al stack
    void push(const T& element) { data.push_back(element); }
    //Método para eliminar un elemento del stack
    void pop() { assert(!data.empty()); data.pop_back(); }
    //Método para obtener el elemento en la cima del stack sin modificarlo (top o peek, son lo mismo)
    const T& top() const { assert(!data.empty()); return data.at(data.size()-1); }
    //Método para obtener el elemento en la cima del stack y modificarlo
    T& top() { assert(!data.empty()); return data.at(data.size()-1); }
    //Método para verificar si el stack está vacío
    bool empty() const { return data.empty(); }
    //Método para obtener el tamaño del stack
    unsigned int size() const { return data.size(); }
    //Método para imprimir los datos del stack
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