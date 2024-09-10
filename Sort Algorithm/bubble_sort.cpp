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

// funcion que hace la logica principal de bubble sort
void bubbleSort(vector<int>& arr) {
    int n = arr.size();
    bool swapped;
    for (int i = 0; i < n - 1; i++) {
        swapped = false;
        for (int j = 0; j < n - i - 1; j++) {
            if (arr[j] > arr[j + 1]) {
                swap(arr[j], arr[j + 1]);
                swapped = true;
            }
        }
        if (!swapped) break;
    }
}

//programa principal, lee los archivos .txt, los almacena y procesa
int main() {
    string carpeta = ".";
    vector<string> archivos_txt;
    string extension = ".txt";
    string prefijo = "dataset";  

    
    ofstream outfile("bubble_sort.txt");
    if (!outfile.is_open()) {
        cerr << "No se pudo abrir el archivo de salida: bubble_sort.txt" << endl;
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

        vector<int> arr;
        stringstream ss(arreglo_str);
        string temp;

        while (ss >> temp) {
            arr.push_back(stoi(temp));
        }

        auto inicio = chrono::high_resolution_clock::now();
        bubbleSort(arr);
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
