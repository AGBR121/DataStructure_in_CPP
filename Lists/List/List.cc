#include<iostream>
#include<cassert>

using namespace std;

template<typename T>

class List
{
private:
    //Node tendra el elemento y el puntero al siguiente dato
    class Node{
        private:
        T data_; 
        Node* next_;
        public:
        //constructores
        Node(){
            data_ = T(); //dato vacio
            next_ = nullptr; //puntero nulo
        }
        Node(const T& element){
            data_ = element; //dato con un elemento
            next_ = nullptr; //puntero nulo
        }
        //cambiar la dirección de memoria a otro nodo
        void setNext(Node* next){
            next_ = next;
        }
        Node* getNext() { return next_; } //Devolver el puntero al valor siguiente
        T& getData() { return data_; } //Devolver el dato
        const T& getData() const { return data_; } //Devolver el dato sin modificarlo
    };
    Node* first_; //Nodo del primer elemento
    Node* last_; //nodo del ultimo elemento
    unsigned int size_; //Tamaño de la lista
public:
    //Constructor lista
    List(){
        first_ = nullptr;
        last_ = nullptr;
        size_ = 0;
    }
    //Devolver el tamaño de la lista
    unsigned int size() const { return size_; }
    //Devolver true si la lista esta vacía
    bool empty() const { return size_ == 0; }
    //push_front
    void push_front(const T& element){
        Node* n = new Node(element); //nuevo elemento
        n->setNext(first_); //puntero del siguiente del primer elemento = puntero del exprimer elemento
        first_ = n; //n nuevo first_
        if(size_ == 0){
            last_ = n; //si la lista estaba vacía, liast_ tambien apunta a n
        }
        size_++;
    }

    void push_back(const T& element){
        Node* n = new Node(element); //nuevo elemento
        if (empty()) {
            first_ = last_ = n; //si esta vacio, first y last apuntaran al mismo elemento
        } else {
            last_->setNext(n); //el anterior last ahora tendra en su valor de next el puntero del nuevo last
            last_ = n; //apuntar al nuevo last
        }
        size_++;
    }

    void pop_front(){
        if(empty()){ return; } //si la lista esta vacia, no haga nada
        Node* temp = first_; //apuntador nodo temporal del primer elemento
        first_ = first_->getNext(); //first ahora apuntara al siguiente elemento
        delete temp; //liberamos la memoria del nodo
        size_--;

        if (size_ == 0) { last_ = nullptr; } //si ya no hay nada, last no apunta a nada
    }

    void pop_back(){
        if(empty()){ return; } //si esta vacío, no hace nada
        if(first_ == last_){ //si first y last apuntan a lo mismo
            delete first_; //liberamos first
            first_ = last_ = nullptr; //ambos no van a apuntar
        }else{
            Node* current = first_; //nodo temporal que recorrerá hasta el penultimo elemento
            while (current->getNext() != last_) {
                current = current->getNext(); //obtenemos la direccion de memoria del ultimo elemento
            }
            delete last_; //liberamos el ultimo
            last_ = current; //el nuevo ultimo será el penultimo
            last_->setNext(nullptr); //el apuntador que tenia el penultimo cambia a nulo
        }
        size_--;
    }

    T& at(unsigned int pos){
        assert(pos < size_ && pos >=0); //error si la posicion supera el tamaño
        Node* current = first_; //nodo que recorrerá hasta llegar a la posicion
        for (unsigned int i = 0; i < pos; i++) {
            current = current->getNext();
        }
        return current->getData(); //devolvemos el dato encontrado
    }

    //lo mismo que arriba pero sin permitir un cambio en el dato
    const T& at(unsigned int pos) const{
        assert(pos < size_ && pos>=0);
        Node* current = first_;
        for (unsigned int i = 0; i < pos; i++) {
            current = current->getNext();
        }
        return current->getData();
    }

    void insert(unsigned int index, const T& element) {
        assert(index <= size_);
    
        if (index == 0) { 
            push_front(element); //si el indice es 0, solo hacemos push_front
            return;
        }
    
        if (index == size_) { 
            push_back(element); //si el indice es el ultimo dato, hacemos push_back
            return;
        }

        Node* current = first_; //apuntador al nodo para que recorra la lista hasta llegar al indice
        for (unsigned int i = 0; i < index - 1; i++) {
            current = current->getNext();
        }
    
        Node* newNode = new Node(element); //nuevo nodo en el arreglo que tendrá el elemento
        newNode->setNext(current->getNext()); //el nuevo nodo apuntará a donde apuntaba el indice
        current->setNext(newNode); //el anterior nodo ahora apunta al nuevo nodo
        
        size_++;
    }
    void remove(unsigned int index){
        assert(index < size_ || index > size_);
    
        if (index == 0) { 
            pop_front(); //si el indice es el primer dato, hacemos pop_front
            return;
        }
        if(index == size_){
            pop_back(); //si el indice es el ultimo dato, hacemos pop_back
            return;
        }
        Node* temp = first_; //apuntador de nodo que recorrerá la lista hasta llegar al indice -1
        for (unsigned int i = 0; i < index - 1; i++) {
            temp = temp->getNext();
        }

        Node* toDelete = temp->getNext(); //apuntar al nodo siguiente que será el que eliminaremos
        temp->setNext(toDelete->getNext()); //el nodo anterior al de eliminar ahora apunta al 
                                            //siguiente  del nodo a eliminar
    
        delete toDelete; //liberar el nodo
        
        size_--;
    }
    int find(const T& element) const {
        Node* temp = first_; //apuntar a los nodos hasta encontrar el elemento
        int index = 0;
    
        while (temp != nullptr) {
            if (temp->getData() == element) {
                return index; //retornamos la posicion
            }
            temp = temp->getNext();
            index++;
        }
        
        return -1; //no lo encontró
    }

    // Ordena la lista utilizando el algoritmo Merge Sort de forma recursiva.
    Node* MergeSort(Node* firstElement){
        // Caso base: si la lista está vacía o solo tiene un elemento, se devuelve tal cual.
        if (!firstElement || !firstElement->getNext()){ 
            return firstElement; 
        }

        // Se obtiene el nodo medio de la lista.
        Node* middle = getMiddle(firstElement);
        // Se obtiene la segunda mitad de la lista.
        Node* secondMiddle = middle->getNext();
        // Se separan ambas mitades.
        middle->setNext(nullptr);

        // Se ordenan las dos mitades recursivamente y luego se combinan.
        return Merge(MergeSort(firstElement), MergeSort(secondMiddle));
}

    // Encuentra el nodo medio de la lista utilizando el método de "puntero lento y rápido".
    Node* getMiddle(Node* firstElement){
        // Si la lista está vacía, se devuelve tal cual.
        if(!firstElement){ return firstElement; }

        // Se inicializan dos punteros, uno lento (avanza de uno en uno) y otro rápido (avanza de dos en dos).
        Node* slow = firstElement, *fast = firstElement->getNext();
        
        // Se recorre la lista hasta que el puntero rápido llegue al final.
        while(fast && fast->getNext()){
            slow = slow->getNext();        // Avanza de uno en uno.
            fast = fast->getNext()->getNext(); // Avanza de dos en dos.
        }
        
        // Se devuelve el puntero lento, que estará en la mitad de la lista.
        return slow;
    }

    // Une dos listas ordenadas en una sola lista ordenada.
    Node* Merge(Node* left, Node* right){
        // Si una de las listas está vacía, se devuelve la otra.
        if(!left){ return right; }
        if(!right){ return left; }

        // Se compara el valor de los nodos para ordenar correctamente.
        if (left->getData() < right->getData()) {
            // Si el nodo izquierdo es menor, se enlaza con la fusión de los siguientes nodos.
            left->setNext(Merge(left->getNext(), right));
            return left;
        } else {
            // Si el nodo derecho es menor o igual, se enlaza con la fusión de los siguientes nodos.
            right->setNext(Merge(left, right->getNext()));
            return right;
        }
    }

    // Llama a MergeSort para ordenar la lista.
    void sort(){
        first_ = MergeSort(first_);
    }

    // Imprime la lista en consola.
    void print() const {
        // Si la lista está vacía, no se imprime nada.
        if (!first_) { return; }

        // Se recorre la lista nodo por nodo.
        Node* current = first_; 
        while (current) {
            cout << current->getData() << " "; // Se imprime el dato del nodo actual.
            current = current->getNext(); // Se avanza al siguiente nodo.
        }
        cout << endl; // Se imprime un salto de línea al final.
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
