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

// programa principal, lee los archivos .txt, los almacena y procesa
// ademas, ordena los arreglos con la funcion Sort() de C++
int main() {
    string carpeta = ".";
    vector<string> archivos_txt;
    string extension = ".txt";
    string prefijo = "dataset";  

    
    ofstream outfile("standard.txt");
    if (!outfile.is_open()) {
        cerr << "No se pudo abrir el archivo de salida: standard.txt" << endl;
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
        sort(arr.begin(), arr.end());
        auto fin = chrono::high_resolution_clock::now();
        chrono::duration<double, milli> tiempo_transcurrido = fin - inicio;

        cout << "Tiempo de ejecución: " << tiempo_transcurrido.count() << " milisegundos" << endl;
        outfile << "Tiempo de ejecución: " << tiempo_transcurrido.count() << " milisegundos" << endl;

        promedio += tiempo_transcurrido;
        file.close();
    }

    promedio = promedio / archivos_txt.size();  
    cout << "Tiempo de ejecución promedio: " << promedio.count() << " milisegundos" << endl;
    outfile << "Tiempo de ejecución promedio: " << promedio.count() << " milisegundos" << endl;

    outfile.close();
    return 0;
}
