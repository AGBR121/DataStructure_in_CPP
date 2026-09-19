#include<iostream>
#include<cassert>
#include <chrono>
#include "Vector.h"

using namespace std;
using namespace std::chrono;

Vector<int> removeDuplicates(const Vector<int>& vector){
    Vector<int> withoutDuplicates;
    for(unsigned int i = 0; i < vector.size(); i++){
        bool duplicate = false;
        for(unsigned int j = 0; j < withoutDuplicates.size(); j++){
            if(withoutDuplicates[j] == vector[i]){
                duplicate = true;
                break;
            }
        }
        if(!duplicate){
            withoutDuplicates.push_back(vector[i]);
        }
    }
    return withoutDuplicates; 
}

// Función que recibe dos vectores ordenados y los mezcla en un nuevo vector ordenado
template<typename T>
Vector<T> mergeSortedVectors(Vector<T>& vector1, Vector<T>& vector2){
    if(!vector1.size()){ //Si el vector 1 esta vacio, devolvemos el 2
        return vector2;
    }
    if(!vector2.size()){
        return vector1; //Si el vector 2 esta vacío, devolvemos el 1
    }
    if(!vector1.size() && !vector2.size()){
        return Vector<T>(); //Si ambos estan vacios, devolvemos un vector vacío
    }
    Vector<T> mergedVector; //Vector donde se guardaran los elementos ordenados
    unsigned int i = 0; //Indice para recorrer el vector 1
    unsigned int j = 0;//Indice para recorrer el vector 2

    while(i < vector1.size() && j < vector2.size()){ //Insertar los elementos en orden
        if(vector1[i] < vector2[j]){
            mergedVector.push_back(vector1[i]);
            i++;
        }else{
            mergedVector.push_back(vector2[j]);
            j++;
        }
    }
    while(i < vector1.size()){ //Verificar si quedaron elementos en el vector 1
        mergedVector.push_back(vector1[i]);
        i++;
    }
    while(j < vector2.size()){//Verificar si quedaron elementos en el vector 2
        mergedVector.push_back(vector2[j]);
        j++;
    }
    return mergedVector; //Devolver el vector ordenado
}

// Función de prueba para probar la clase Vector y sus metodos
void Prueba(){
    Vector<int> x(10,0); // Crea un vector de 10 elementos inicializados en 0
    cout << "Size of x: " << x.size() << endl;
    cout << "values of x: " ;
    x.print();
    
    x.at(5) = 100; // Modifica el valor en la posición 5
    cout << "position using at: " << x.at(5) << endl;
    cout << "position using []: " << x[5] << endl;
    x.print();
    
    x.push_back(10); // Agrega un 10 al final
    x.push_front(50); // Agrega un 50 al inicio
    cout << "use push_front and push_back" << endl;
    x.print();
    
    x.pop_front(); // Elimina el primer elemento
    x.pop_back(); // Elimina el último elemento
    cout << "use pop_front and pop_back" << endl;
    x.print();
    x.insert(8, 2); //Agrega un 2 en la posicion 8
    cout << "insert(8,2)"<<endl;
    x.print();
    cout << "erase(8)" << endl; //elimina lo que haya en la posicion 8
    x.erase(8);
    x.print();
    cout << x.empty() << endl; // Verifica si el vector está vacío
    return;
}


//Funcion para probar la funcion removeDuplicates
void RemovingDuplicates(){
    Vector<int> numbers = {1, 2, 2, 3, 4, 4, 5};
    Vector<int> uniqueNumbers = removeDuplicates(numbers);

    uniqueNumbers.print(); // Expected: {1, 2, 3, 4, 5}

    Vector<int> numbers2 = {1,1,1,1,1,1};
    Vector<int> uniqueNumbers2 = removeDuplicates(numbers2);
    uniqueNumbers2.print(); // Expected: {1}

    Vector<int> numbers3 = {};
    Vector<int> uniqueNumbers3 = removeDuplicates(numbers3);
    uniqueNumbers3.print(); // Expected: {}

    Vector<int> numbers4 = {1};
    Vector<int> uniqueNumbers4 = removeDuplicates(numbers4);
    uniqueNumbers4.print(); // Expected: {1}
}


//Funcion para probar la funcion mergeSortedVectors
void MergeSortVectorsPrueba(){
    Vector<int> vector1 = {1, 3, 5};
    Vector<int> vector2 = {2, 4, 6};
    Vector<int> mergedVector = mergeSortedVectors(vector1, vector2);

    mergedVector.print(); // Expected: {1, 2, 3, 4, 5, 6}

    Vector<int> vector3 = {1, 2, 3};
    Vector<int> vector4 = {};
    Vector<int> mergedVector2 = mergeSortedVectors(vector3, vector4);
    mergedVector2.print(); // Expected: {1, 2, 3}

    Vector<int> vector5 = {};
    Vector<int> vector6 = {4,5,6};
    Vector<int> mergedVector3 = mergeSortedVectors(vector5, vector6);
    mergedVector3.print(); // Expected: {4, 5, 6}

    Vector<int> vector7 = {1,1,1,1};
    Vector<int> vector8 = {1,1,1,1};
    Vector<int> mergedVector4 = mergeSortedVectors(vector7, vector8);
    mergedVector4.print(); // Expected: {1,1,1,1,1,1,1,1}
}

void AnalizeTimeInsertAndErase() {
    for (int k = 0; k < 3; k++) {
        cout << (k == 0 ? "Insert and erase 100000 elements in the middle three times" :
                k == 1 ? "Insert and erase 100000 elements at the beginning three times" :
                         "Insert and erase 100000 elements at the end three times") 
             << endl;

        milliseconds timeAverageInsert(0);
        milliseconds timeAverageErase(0);

        for (unsigned int j = 1; j <= 3; j++) {
            Vector<int> numbers = {1, 2, 3, 4, 5};

            auto inicio = high_resolution_clock::now();
            for (unsigned int i = 0; i < 100000; i++) {
                unsigned int place = (k == 0) ? numbers.size() / 2 :
                                     (k == 2) ? numbers.size() - 1 : 0;
                numbers.insert(place, i);
            }
            auto fin = high_resolution_clock::now();
            auto duracion = duration_cast<milliseconds>(fin - inicio);
            cout << "Time on " << j << " insertion: " << duracion.count() << " ms" << endl;
            timeAverageInsert += duracion;

            auto inicio2 = high_resolution_clock::now();
            for (unsigned int i = 0; i < 100000; i++) {
                if (numbers.empty()) break;
                unsigned int place = (k == 0) ? numbers.size() / 2 :
                                     (k == 2) ? numbers.size() - 1 : 0;
                numbers.erase(place);
            }
            auto fin2 = high_resolution_clock::now();
            auto duracion2 = duration_cast<milliseconds>(fin2 - inicio2);
            cout << "Time on " << j << " erase: " << duracion2.count() << " ms" << endl;
            timeAverageErase += duracion2;
        }

        cout << "Average time for insertion: " << (timeAverageInsert.count() / 3) << " ms" << endl;
        cout << "Average time for erasing: " << (timeAverageErase.count() / 3) << " ms" << endl;
        cout << endl;
    }
}

void AnalizeTimeRemoveDuplicates() {
    milliseconds timeAverage(0);

    for (unsigned int i = 1; i <= 3; i++) {
        Vector<int> numbers;

        switch (i) {
            case 1: 
                cout << "Case 1: No duplicates" << endl;
                for (unsigned int j = 0; j < 100000; j++) {
                    numbers.push_back(j);
                }
                break;

            case 2: 
                cout << "Case 2: Some duplicates" << endl;
                for (unsigned int j = 0; j < 50000; j++) {
                    numbers.push_back(j);
                    numbers.push_back(j);
                }
                break;

            case 3: 
                cout << "Case 3: All elements are the same" << endl;
                for (unsigned int j = 0; j < 100000; j++) {
                    numbers.push_back(1);
                }
                break;
        }

        milliseconds totalDuration(0);

        for (unsigned int k = 1; k <= 5; k++) { 
            auto inicio = high_resolution_clock::now();
            Vector<int> newNumbers = removeDuplicates(numbers);
            auto fin = high_resolution_clock::now();
            auto duracion = duration_cast<milliseconds>(fin - inicio);

            cout << "Time on run " << k << ": " << duracion.count() << " ms" << endl;
            totalDuration += duracion;
        }

        cout << "Average time for case " << i << ": " << (totalDuration.count() / 5) << " ms" << endl;
        cout << endl;
    }
}


int main(){
    //Prueba();
    //RemovingDuplicates();
    //MergeSortVectorsPrueba();
    //AnalizeTimeInsertAndErase();
    //AnalizeTimeRemoveDuplicates();
    return 0;
}
