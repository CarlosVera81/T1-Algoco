#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <filesystem>
#include <chrono>

using namespace std;
namespace fs = std::filesystem;

// Funcion para leer matrices del archivo y almacenarlas
void leerMatrizDesdeArchivo(ifstream &archivo, vector<vector<long int>> &matriz) {
    int filas, columnas;
    archivo >> filas >> columnas;

    matriz.resize(filas, vector<long int>(columnas));
    for (int i = 0; i < filas; ++i) {
        for (int j = 0; j < columnas; ++j) {
            long long temp;
            archivo >> temp;
            matriz[i][j] = temp;
        }
    }
}

//funcion que hace la logica de multiplicar las matrices
void multiplicarMatrices(const vector<vector<long int>> &A, const vector<vector<long int>> &B, vector<vector<long int>> &C) {
    long int m = A.size();        
    long int n = A[0].size();     
    long int p = B[0].size();     

   
    C.assign(m, vector<long int>(p, 0));

    for (int i = 0; i < m; ++i) {
        for (int j = 0; j < p; ++j) {
            for (int k = 0; k < n; ++k) {
                C[i][j] += A[i][k] * B[k][j];
            }
        }
    }
}

//funcion que va probando los distintos datasets
void procesarArchivo(const fs::path &archivo_path, ofstream &log, vector<double> &tiempos) {
    ifstream archivo(archivo_path);
    if (!archivo.is_open()) {
        cerr << "No se pudo abrir el archivo " << archivo_path.filename().string() << endl;
        return;
    }

    vector<vector<vector<long int>>> matrices;
    while (!archivo.eof()) {
        vector<vector<long int>> matriz;
        leerMatrizDesdeArchivo(archivo, matriz);
        matrices.push_back(matriz);
    }
    archivo.close();

    if (matrices.size() < 2) {
        cerr << "El archivo " << archivo_path.filename().string() << " debe contener al menos dos matrices para realizar la multiplicación." << endl;
        return;
    }

 
    vector<vector<long int>> resultado = matrices[0];
    auto inicio = chrono::high_resolution_clock::now();
    for (size_t i = 1; i < matrices.size(); ++i) {
        vector<vector<long int>> temp_resultado;
        multiplicarMatrices(resultado, matrices[i], temp_resultado);
        resultado = temp_resultado;  
    }
    auto fin = chrono::high_resolution_clock::now();


    chrono::duration<double, milli> tiempo_transcurrido = fin - inicio;
    tiempos.push_back(tiempo_transcurrido.count());
    log << "Tiempo de ejecución: " << tiempo_transcurrido.count() << " milisegundos" << endl;
}

//Programa principal, tambien se usa para calcular el tiempo promedio
int main() {
    ofstream log("tradicional.txt");
    if (!log.is_open()) {
        cerr << "No se pudo abrir el archivo de log." << endl;
        return 1;
    }

    vector<double> tiempos;
    
    
    for (const auto &entry : fs::directory_iterator(fs::current_path())) {
        if (entry.is_regular_file() && entry.path().filename().string().rfind("matriz", 0) == 0) {
            procesarArchivo(entry.path(), log, tiempos);
        }
    }

   
    if (!tiempos.empty()) {
        double promedio = 0;
        for (double tiempo : tiempos) {
            promedio += tiempo;
        }
        promedio /= tiempos.size();
        log << "Tiempo de ejecución promedio:" << promedio << " milisegundos" << endl;
    }

    log.close();
    return 0;
}
