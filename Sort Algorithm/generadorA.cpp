#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <random>
#include <numeric> 
#include <unordered_set> 

using namespace std;


// programa principal, pide al usuario que ingrese los parametros con los cuales sera creado el arreglo, guarda
// el resultado en un archivo .txt
int main() {
    string a3;
    int a2;
    

    cout << "Ingrese cantidad de datasets a generar: ";
    cin >> a2;
    cout << "Ingrese tamaño de los datasets a generar (xs/s/m/l/xl/xxl/qs/qss): ";
    cin >> a3;
    
    int a4 = 0;
    
    cout << "Ingrese porcentaje de desorden de los datos (1-100): ";
    cin >> a4;
   

    char incluir_duplicados;
    int porcentaje_duplicados = 0;
    cout << "¿Desea incluir elementos duplicados? (s/n): ";
    cin >> incluir_duplicados;
    if (incluir_duplicados == 's') {
        cout << "Ingrese porcentaje de elementos duplicados (1-100): ";
        cin >> porcentaje_duplicados;
    }

    char invertir;
    cout << "¿Desea generar un arreglo inversamente ordenado? (s/n): ";
    cin >> invertir;
    
    for (int j = 0; j < a2; ++j) {
        vector<int> datos;
        int tamaño = 0;
    
        if (a3 == "xs") {
            tamaño = 10;
        } else if (a3 == "s") {
            tamaño = 100;
        } else if (a3 == "m") {
            tamaño = 1000;
        } else if (a3 == "l") {
            tamaño = 10000;
        } else if (a3 == "xl") {
            tamaño = 100000;
        } else if (a3 == "xxl") {
            tamaño = 1000000;
        } else if (a3 == "qs") {
            tamaño = 10000000;
        } else if (a3 == "qss") {
            tamaño = 100000000;
        } else {
            cerr << "Tamaño desconocido. Por favor, ingrese uno válido." << endl;
            return 1;
        }
    
        datos.resize(tamaño);
        iota(datos.begin(), datos.end(), 1);

        if (invertir == 's') {
            reverse(datos.begin(), datos.end());
        }

    
        if (incluir_duplicados == 's') {
            long int n_duplicados = static_cast<int>(tamaño * porcentaje_duplicados / 100.0f);
            random_device rd;
            mt19937 g(rd());

            unordered_set<int> elementos_duplicados;
            while (elementos_duplicados.size() < static_cast<unordered_set<int>::size_type>(n_duplicados)) {
                int idx = g() % tamaño;
                datos.push_back(datos[idx]);
                elementos_duplicados.insert(datos[idx]);
            }

            shuffle(datos.begin(), datos.end(), g);
        }
    
        
        float porcentaje_desorden = static_cast<float>(a4) / 100.0f;
        int n_desorden = static_cast<int>(datos.size() * porcentaje_desorden);

        random_device rd;
        mt19937 g(rd());

        vector<int> indices(datos.size());
        iota(indices.begin(), indices.end(), 0);
        shuffle(indices.begin(), indices.end(), g);
        indices.resize(n_desorden);

        vector<int> valores_desordenados;
        for (int idx : indices) {
            valores_desordenados.push_back(datos[idx]);
        }

        shuffle(valores_desordenados.begin(), valores_desordenados.end(), g);

        for (size_t i = 0; i < indices.size(); ++i) {
            datos[indices[i]] = valores_desordenados[i];
        }
    
    
        string nombre_archivo = "dataset_" + to_string(j + 1) + ".txt";
        ofstream archivo(nombre_archivo);
    
        if (archivo.is_open()) {
            archivo << datos.size() << "\n";
            for (const int& numero : datos) {
                archivo << numero << " ";
            }
            archivo.close();
            cout << "Dataset " << j + 1 << " generado" << endl;
        } else {
            cerr << "No se pudo abrir el archivo " << nombre_archivo << endl;
            return 1;
        }
    }
    
    return 0;
}
