#include <iostream>
#include <vector>
#include <random>
#include <iomanip>
#include <algorithm>
#include <string>
#include <ctime>

// Headers del libro
#include "BinarySearchTree.h"
#include "AVLTree.h"
#include "BTreeNode.h"

using namespace std;

// --- FUNCIÓN DE TIEMPO ---
inline double elapsed(bool reset = false) {
    static clock_t start = clock();
    if (reset) start = clock();
    return (1000.00 * double(clock() - start)) / double(CLOCKS_PER_SEC);
}

// --- GENERACIÓN DE DATOS ---
vector<int> generarAleatorio(int n) {
    vector<int> v(n);
    for (int i = 0; i < n; i++) v[i] = i;
    random_device rd;
    mt19937 g(rd());
    shuffle(v.begin(), v.end(), g);
    return v;
}

vector<int> generarSemiOrdenado(int n) {
    vector<int> v(n);
    for (int i = 0; i < n; i++) v[i] = i;
    random_device rd;
    mt19937 g(rd());
    int chunkSize = n / 10;
    if (chunkSize < 1) chunkSize = 1;
    for (int i = 0; i < n; i += chunkSize) {
        auto end = min(v.begin() + i + chunkSize, v.end());
        shuffle(v.begin() + i, end, g);
    }
    return v;
}

// --- CORE DEL EXPERIMENTO ---
template <typename T>
void realizarExperimento(T& tree, const vector<int>& baseData, string treeName, string situacion) {
    int N = baseData.size();
    
    // Elegir 10 elementos aleatorios que SEGURO existen en el árbol
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> dis(0, N - 1);
    vector<int> targets;
    for(int i = 0; i < 10; i++) targets.push_back(baseData[dis(gen)]);

    cout << "\n>>> [" << treeName << "] - " << situacion << " (N = " << N << ")" << endl;
    cout << fixed << setprecision(6);

    // 1. CONSTRUCCIÓN (N elementos)
    elapsed(true);
    for (int val : baseData) tree.insert(val);
    double t_construccion = elapsed();
    cout << "  1. Construccion (" << N << "):  " << t_construccion << " ms" << endl;

    // 2. BÚSQUEDA (10 elementos aleatorios)
    elapsed(true);
    for (int val : targets) tree.find(val);
    double t_busqueda = elapsed();
    cout << "  2. Busqueda (10):       " << t_busqueda << " ms" << endl;

    // 3. ELIMINACIÓN (10 elementos aleatorios)
    elapsed(true);
    for (int val : targets) tree.remove(val);
    double t_eliminacion = elapsed();
    cout << "  3. Eliminacion (10):    " << t_eliminacion << " ms" << endl;

    // 4. INSERCIÓN (Re-insertar los 10 eliminados)
    elapsed(true);
    for (int val : targets) tree.insert(val);
    double t_insercion = elapsed();
    cout << "  4. Insercion (10):      " << t_insercion << " ms" << endl;
}

int main() {
    // Tamaños exactos de la indicación
    vector<long> tam = {100, 1000, 10000, 100000, 1000000};

    cout << "========================================================" << endl;
    cout << "      INICIANDO BENCHMARK: BST vs AVL (UBUNTU)          " << endl;
    cout << "========================================================" << endl;

    for (long N : tam) {
        cout << "\n\n" << string(56, '=') << endl;
        cout << "                 EVALUANDO N = " << N << endl;
        cout << string(56, '=') << endl;

        // --- SITUACIÓN A (Completamente Aleatorio) ---
        vector<int> dataA = generarAleatorio(N);
        
        BinarySearchTree<int> bstA;
        realizarExperimento(bstA, dataA, "BST", "SIT. A (Aleatorio)");

        AVLTree<int> avlA;
        realizarExperimento(avlA, dataA, "AVL", "SIT. A (Aleatorio)");

        cout << "\n--------------------------------------------------------" << endl;

        // --- SITUACIÓN B (Poco Desorden) ---
        vector<int> dataB = generarSemiOrdenado(N);

        BinarySearchTree<int> bstB;
        realizarExperimento(bstB, dataB, "BST", "SIT. B (Semi-Ordenado)");

        AVLTree<int> avlB;
        realizarExperimento(avlB, dataB, "AVL", "SIT. B (Semi-Ordenado)");
    }

    return 0;
}