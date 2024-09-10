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

vector<int> quickSort(vector<int>& ar) {
    if (ar.size() <= 1)
        return ar;

    size_t mid = ar.size() / 2;
    int pivot = ar[mid];

    vector<int> smaller;
    vector<int> greater;

    for (size_t ind = 0; ind < ar.size(); ind++) {
        int val = ar[ind];
        if (ind != mid) {
            if (val < pivot)
                smaller.push_back(val);
            else if (val > pivot)
                greater.push_back(val);
            else {
                if (ind < mid)
                    smaller.push_back(val);
                else
                    greater.push_back(val);
            }
        }
    }

    vector<int> ans;
    vector<int> sa1 = quickSort(smaller);
    vector<int> sa2 = quickSort(greater);

    for (int val1 : sa1)
        ans.push_back(val1);

    ans.push_back(pivot);

    for (int val2 : sa2)
        ans.push_back(val2);

    return ans;
}

int main() {
    string carpeta = ".";
    vector<string> archivos_txt;
    string extension = ".txt";
    string prefijo = "dataset";  // Prefijo que los archivos deben tener

    // Archivo de salida
    ofstream outfile("stable.txt");
    if (!outfile.is_open()) {
        cerr << "No se pudo abrir el archivo de salida: stable.txt" << endl;
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
        arr = quickSort(arr);
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
