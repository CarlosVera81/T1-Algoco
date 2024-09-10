#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>
#include <chrono>
#include <algorithm>
#include <filesystem>  

namespace fs = std::filesystem;  

using namespace std;

// Funcion que hace la logica de merge
void merge(vector<int>& arr, int l, int m, int r) {
    int n1 = m - l + 1;
    int n2 = r - m;

    std::vector<int> L(n1), R(n2);

    for (int i = 0; i < n1; i++)
        L[i] = arr[l + i];

    for (int j = 0; j < n2; j++)
        R[j] = arr[m + 1 + j];

    int i = 0, j = 0, k = l;

    while (i < n1 && j < n2) {
        if (L[i] <= R[j]) {
            arr[k] = L[i];
            i++;
        } else {
            arr[k] = R[j];
            j++;
        }
        k++;
    }

    while (i < n1) {
        arr[k] = L[i];
        i++;
        k++;
    }

    while (j < n2) {
        arr[k] = R[j];
        j++;
        k++;
    }
}

//funcion principal de mergesort
void mergeSort(vector<int>& arr, int l, int r) {
    if (l < r) {
        int m = l + (r - l) / 2;

        mergeSort(arr, l, m);
        mergeSort(arr, m + 1, r);
        merge(arr, l, m, r);
    }
}

//programa principal, lee los archivos .txt, los almacena y procesa
int main() {
    string carpeta = ".";
    vector<string> archivos_txt;
    string extension = ".txt";
    string prefijo = "dataset";  

    
    ofstream outfile("merge_sort.txt");
    if (!outfile.is_open()) {
        cerr << "No se pudo abrir el archivo de salida: merge_sort.txt" << endl;
        return 1;
    }

    for (const auto& entry : fs::directory_iterator(carpeta)) {
        if (entry.path().extension() == extension && entry.path().stem().string().rfind(prefijo, 0) == 0) {
            archivos_txt.push_back(entry.path().string());
        }
    }

    chrono::duration<double, milli> promedio;
    for (const string& archivo : archivos_txt) {
        ifstream file(archivo);
        if (!file.is_open()) {
            cerr << "No se pudo abrir el archivo: " << archivo << endl;
            continue;
        }

        string tamanio_str, arreglo_str;
        getline(file, tamanio_str);
        getline(file, arreglo_str);
        int tamanio = stoi(tamanio_str);
        vector<int> arr;
        stringstream ss(arreglo_str);
        string temp;

        while (ss >> temp) {
            arr.push_back(stoi(temp));
        }

        auto inicio = chrono::high_resolution_clock::now();
        mergeSort(arr, 0, tamanio - 1);
        auto fin = chrono::high_resolution_clock::now();
        chrono::duration<double, milli> tiempo_transcurrido = fin - inicio;

        cout << "Tiempo de ejecución: " << tiempo_transcurrido.count() << " milisegundos" << endl;
        outfile << "Tiempo de ejecución: " << tiempo_transcurrido.count() << " milisegundos" << endl;

        promedio += tiempo_transcurrido;
        file.close();
    }

    promedio = promedio / 10;
    cout << "Tiempo de ejecución promedio: " << promedio.count() << " milisegundos" << endl;
    outfile << "Tiempo de ejecución promedio: " << promedio.count() << " milisegundos" << endl;

    outfile.close();
    return 0;
}
