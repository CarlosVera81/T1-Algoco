#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <filesystem>
#include <chrono>

using namespace std;
namespace fs = std::filesystem;

// Funcion que se encarga de leer la matriz desde el archivo y almacenarla
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

//Funcion que transpone la segunda matriz
void transponerMatriz(const vector<vector<long int>> &matriz, vector<vector<long int>> &transpuesta) {
    int filas = matriz.size();
    int columnas = matriz[0].size();
    transpuesta.assign(columnas, vector<long int>(filas));

    for (int i = 0; i < filas; ++i) {
        for (int j = 0; j < columnas; ++j) {
            transpuesta[j][i] = matriz[i][j];
        }
    }
}

//funcion que hace la logica de multiplicar las matrices
void multiplicarMatrices(const vector<vector<long int>> &A, const vector<vector<long int>> &B, vector<vector<long int>> &C) {
    long int m = A.size();        
    long int n = A[0].size();     
    long int p = B.size();        

    C.assign(m, vector<long int>(p, 0));

   
    for (int i = 0; i < m; ++i) {
        for (int j = 0; j < p; ++j) {
            for (int k = 0; k < n; ++k) {
                C[i][j] += A[i][k] * B[j][k]; 
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

   
    vector<vector<long int>> segunda_matriz_transpuesta;
    vector<vector<long int>> resultado;
    auto inicio = chrono::high_resolution_clock::now();
    transponerMatriz(matrices[1], segunda_matriz_transpuesta);
    multiplicarMatrices(matrices[0], segunda_matriz_transpuesta, resultado);

  
    for (size_t i = 2; i < matrices.size(); ++i) {
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
    ofstream log("optimizado.txt");
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
