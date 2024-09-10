#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <filesystem>
#include <chrono>

using namespace std;
namespace fs = std::filesystem;

typedef vector<vector<long int>> Matrix;

//Funcion para sumar dos matrices, para la logica del algoritmo de strassen
Matrix add(const Matrix &A, const Matrix &B) {
    int n = A.size();
    Matrix result(n, vector<long int>(n));
    for (int i = 0; i < n; ++i)
        for (int j = 0;j<n; ++j)
            result[i][j] = A[i][j] + B[i][j];
    return result;
}
//Funcion para restar dos matrices, para la logica del algoritmo de strassen
Matrix subtract(const Matrix &A, const Matrix &B) {
    int n = A.size();
    Matrix result(n, vector<long int>(n));
    for (int i = 0; i < n; ++i)
        for (int j = 0; j < n; ++j)
            result[i][j] = A[i][j] - B[i][j];
    return result;
}

//Funcion que hace la logica principal del algoritmo de strassen
Matrix strassenMultiply(const Matrix &A, const Matrix &B) {
    int n = A.size();
    if (n == 1) {
        return Matrix{{A[0][0] * B[0][0]}};
    }

    int newSize = n / 2;
    Matrix A11(newSize, vector<long int>(newSize));
    Matrix A12(newSize, vector<long int>(newSize));
    Matrix A21(newSize, vector<long int>(newSize));
    Matrix A22(newSize, vector<long int>(newSize));
    Matrix B11(newSize, vector<long int>(newSize));
    Matrix B12(newSize, vector<long int>(newSize));
    Matrix B21(newSize, vector<long int>(newSize));
    Matrix B22(newSize, vector<long int>(newSize));

    
    for (int i = 0; i < newSize; ++i) {
        for (int j = 0; j < newSize; ++j) {
            A11[i][j] = A[i][j];
            A12[i][j] = A[i][j + newSize];
            A21[i][j] = A[i + newSize][j];
            A22[i][j] = A[i + newSize][j + newSize];

            B11[i][j] = B[i][j];
            B12[i][j] = B[i][j + newSize];
            B21[i][j] = B[i + newSize][j];
            B22[i][j] = B[i + newSize][j + newSize];
        }
    }

   
    Matrix M1 = strassenMultiply(add(A11, A22), add(B11, B22));
    Matrix M2 = strassenMultiply(add(A21, A22), B11);
    Matrix M3 = strassenMultiply(A11, subtract(B12, B22));
    Matrix M4 = strassenMultiply(A22, subtract(B21, B11));
    Matrix M5 = strassenMultiply(add(A11, A12), B22);
    Matrix M6 = strassenMultiply(subtract(A21, A11), add(B11, B12));
    Matrix M7 = strassenMultiply(subtract(A12, A22), add(B21, B22));

   
    Matrix C11 = add(subtract(add(M1, M4), M5), M7);
    Matrix C12 = add(M3, M5);
    Matrix C21 = add(M2, M4);
    Matrix C22 = add(subtract(add(M1, M3), M2), M6);

    
    Matrix C(n, vector<long int>(n));
    for (int i = 0; i < newSize; ++i) {
        for (int j = 0; j < newSize; ++j) {
            C[i][j] = C11[i][j];
            C[i][j + newSize] = C12[i][j];
            C[i + newSize][j] = C21[i][j];
            C[i + newSize][j + newSize] = C22[i][j];
        }
    }
    return C;
}

// Funcion para leer matrices del archivo y almacenarlas
void leerMatrizDesdeArchivo(ifstream &archivo, Matrix &matriz) {
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

//funcion que va probando los distintos datasets
void procesarArchivo(const fs::path &archivo_path, ofstream &log, vector<double> &tiempos) {
    ifstream archivo(archivo_path);
    if (!archivo.is_open()) {
        cerr << "No se pudo abrir el archivo " << archivo_path.filename().string() << endl;
        return;
    }

    vector<Matrix> matrices;

    while (!archivo.eof()) {
        Matrix matriz;
        leerMatrizDesdeArchivo(archivo, matriz);
        matrices.push_back(matriz);
    }
    archivo.close();

    if (matrices.size() < 2) {
        cerr << "El archivo " << archivo_path.filename().string() << " debe contener al menos dos matrices para realizar la multiplicación." << endl;
        return;
    }

 
    Matrix resultado = matrices[0];
    auto inicio = chrono::high_resolution_clock::now();
    for (size_t i = 1; i < matrices.size(); ++i) {
        resultado = strassenMultiply(resultado, matrices[i]);
    }
    auto fin = chrono::high_resolution_clock::now();

 
    chrono::duration<double, milli> tiempo_transcurrido = fin - inicio;
    tiempos.push_back(tiempo_transcurrido.count());
    log << "Tiempo de ejecución: " << tiempo_transcurrido.count() << " milisegundos" << endl;
}

//Programa principal, tambien se usa para calcular el tiempo promedio
int main() {
    ofstream log("strassen.txt");
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
        log << "tiempo de ejecucion promedio: " << promedio << " milisegundos" << endl;
    }

    log.close();
    return 0;
}
