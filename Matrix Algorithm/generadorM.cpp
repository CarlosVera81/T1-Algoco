#include <iostream>
#include <fstream>
#include <vector>
#include <random>
#include <string>
#include <cstdlib>
#include <ctime>
#include <unistd.h>

using namespace std;

// funcion que genera un numero aleatorio para la creacion de matrices de distinta dimension
int generarNumeroAleatorio(int min, int max) {
    return min + rand() % (max - min + 1);
}

//funcion que genera los numeros aleatorios para las matrices
void generarMatriz(int filas, int columnas, vector<vector<int>>& matriz) {
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> dis(0, 5);

    matriz.resize(filas, vector<int>(columnas));

    for (int i = 0; i < filas; ++i) {
        for (int j = 0; j < columnas; ++j) {
            matriz[i][j] = dis(gen);
        }
    }
}
// Funcion que almacena las matrices generadas en archivos .txt
void guardarMatricesEnArchivo(const string& nombre_archivo, const vector<vector<vector<int>>>& matrices) {
    ofstream archivo(nombre_archivo);

    if (archivo.is_open()) {
    for (size_t i = 0; i < matrices.size(); ++i) {
        int filas = matrices[i].size();
        int columnas = matrices[i][0].size();

        archivo << filas << " " << columnas << "\n";
        int last=0;
        int multi=0;
        for (const auto& fila : matrices[i]) {
            for (const auto& elem : fila) {
                if(multi<(filas*columnas)-1){
                    archivo << elem << " ";
                    multi++;
                }
                else{
                    archivo << elem;
                }

            }
            if (i != matrices.size() - 1) {
                archivo << "\n";
            } else{
                if(last<filas-1){
                    archivo << "\n";
                }
                last++;
            }

        }


        if (i != matrices.size() - 1) {
            archivo << "\n";
        }
    }
        archivo.close();
        cout << "Matrices guardadas en " << nombre_archivo << endl;
    } else {
        cerr << "No se pudo abrir el archivo " << nombre_archivo << endl;
    }
}

// Funcion principal del programa, se usa para consultar al usuario
// los parametros que se usaran para generar las matrices
int main() {
    int num_archivos, num_matrices,minimo,maximo;
    string nombre_base = "matriz_";

    cout << "Ingrese el número de archivos a generar: ";
    cin >> num_archivos;
    cout << "Ingrese el número de matrices por archivo: ";
    cin >> num_matrices;
    int opcion;
    int filas, columnas;
    cout << "1. Matriz cuadrada (3x3, 4x4, etc.)\n";
    cout << "2. Matriz con dimensiones distintas (que se puedan multiplicar)\n";
    cout << "Ingrese la opción (1 o 2): ";
    cin >> opcion;

    if (opcion == 1) {
        cout << "Ingrese el tamaño de la matriz cuadrada (n x n): ";
        cin >> filas;
        columnas = filas;
        for (int i = 0; i < num_archivos; ++i) {
            vector<vector<vector<int>>> matrices;
            for (int j = 0; j < num_matrices; ++j) {
            vector<vector<int>> matriz;
            generarMatriz(filas, columnas, matriz);
            matrices.push_back(matriz);
            }
            
            string nombre_archivo = nombre_base + to_string(i + 1) + ".txt";
            guardarMatricesEnArchivo(nombre_archivo, matrices);
            
        }
    }
    else if (opcion==2){
        cout << "Ingrese el cantidad minima de filas/columnas: ";
        cin >> minimo;
        cout << "Ingrese el cantidad maxima de filas/columnas: ";
        cin >> maximo;
        for (int i = 0; i < num_archivos; ++i) {
            vector<vector<vector<int>>> matrices;
            sleep(1);
            srand(time(0));  

            vector<pair<int, int>> dimensiones;

            int f1 = generarNumeroAleatorio(minimo, maximo);  
            int c1 = generarNumeroAleatorio(minimo, maximo);  
            dimensiones.push_back(make_pair(f1, c1));

            
            for (int i = 1; i < num_matrices; ++i) {
                int filas = dimensiones[i-1].second;  
                int columnas = generarNumeroAleatorio(minimo, maximo);  
                dimensiones.push_back(make_pair(filas, columnas));
            }
            for (int i = 0; i < num_matrices; ++i) {
                vector<vector<int>> matriz;
                generarMatriz(dimensiones[i].first,dimensiones[i].second, matriz);
                matrices.push_back(matriz);
            }
            
            
        string nombre_archivo = nombre_base + to_string(i + 1) + ".txt";
        guardarMatricesEnArchivo(nombre_archivo, matrices);

        }
    }
    return 0;
}
