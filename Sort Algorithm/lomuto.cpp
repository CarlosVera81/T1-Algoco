#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>
#include <chrono>
#include <algorithm>
#include <filesystem>

using namespace std;
namespace fs = std::filesystem;

//logica de la particion de Lomuto
int partition(vector<int>& arr, int low, int high) {
    int pivot = arr[high];  
    int i = (low - 1);  

    for (int j = low; j <= high - 1; j++) {
        
        if (arr[j] <= pivot) {
            i++;  
            swap(arr[i], arr[j]);
        }
    }
    swap(arr[i + 1], arr[high]);
    return (i + 1);
}

// logica principal del algoritmo de quicksort
void quickSort(vector<int>& arr, int low, int high) {
    if (low < high) {
      
        int pi = partition(arr, low, high);
        quickSort(arr, low, pi - 1);
        quickSort(arr, pi + 1, high);
    }
}

//programa principal, lee los archivos .txt, los almacena y procesa
int main() {
    string carpeta = ".";
    vector<string> archivos_txt;
    string extension = ".txt";
    string prefijo = "dataset";  


    ofstream outfile("lomuto.txt");
    if (!outfile.is_open()) {
        cerr << "No se pudo abrir el archivo de salida: lomuto.txt" << endl;
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
        quickSort(arr, 0, tamanio - 1);
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
