// Estructuras de Datos y Algoritmos - Obligatorio 2025
// Test Suite para Navegación del Árbol N-ario
//
// TARJETA 3: Navegación del Árbol
// Casos de prueba:
// 1. Versión de primer nivel: [1]
// 2. Subversión de segundo nivel: [2, 3]
// 3. Versión profunda: [1, 2, 3, 4]
// 4. Versión inexistente: [5, 1] → NULL

#include <iostream>
#include <cstring>
#include "../include/utils.h"
#include "../include/version.h"

using namespace std;

// Colores para output
#define COLOR_RESET   "\033[0m"
#define COLOR_GREEN   "\033[32m"
#define COLOR_RED     "\033[31m"
#define COLOR_YELLOW  "\033[33m"
#define COLOR_BLUE    "\033[34m"
#define COLOR_CYAN    "\033[36m"

// Contadores
int testsPasados = 0;
int testsFallados = 0;

// Función auxiliar para crear árbol de prueba
void construirArbolPrueba(Version& raiz) {
    // Crear versiones de primer nivel: 1, 2, 3
    Version v1 = crearVersion(1);
    Version v2 = crearVersion(2);
    Version v3 = crearVersion(3);
    
    v1->siguienteHermano = v2;
    v2->siguienteHermano = v3;
    v3->siguienteHermano = nullptr;
    
    raiz = v1;
    
    // Crear hijos de versión 1: 1.1, 1.2, 1.3
    Version v1_1 = crearVersion(1);
    Version v1_2 = crearVersion(2);
    Version v1_3 = crearVersion(3);
    
    v1_1->padre = v1;
    v1_2->padre = v1;
    v1_3->padre = v1;
    
    v1_1->siguienteHermano = v1_2;
    v1_2->siguienteHermano = v1_3;
    v1_3->siguienteHermano = nullptr;
    
    v1->primerHijo = v1_1;
    
    // Crear hijos de versión 2: 2.1, 2.2, 2.3
    Version v2_1 = crearVersion(1);
    Version v2_2 = crearVersion(2);
    Version v2_3 = crearVersion(3);
    
    v2_1->padre = v2;
    v2_2->padre = v2;
    v2_3->padre = v2;
    
    v2_1->siguienteHermano = v2_2;
    v2_2->siguienteHermano = v2_3;
    v2_3->siguienteHermano = nullptr;
    
    v2->primerHijo = v2_1;
    
    // Crear hijos de versión 1.2: 1.2.1, 1.2.2, 1.2.3
    Version v1_2_1 = crearVersion(1);
    Version v1_2_2 = crearVersion(2);
    Version v1_2_3 = crearVersion(3);
    
    v1_2_1->padre = v1_2;
    v1_2_2->padre = v1_2;
    v1_2_3->padre = v1_2;
    
    v1_2_1->siguienteHermano = v1_2_2;
    v1_2_2->siguienteHermano = v1_2_3;
    v1_2_3->siguienteHermano = nullptr;
    
    v1_2->primerHijo = v1_2_1;
    
    // Crear hijos de versión 1.2.3: 1.2.3.1, 1.2.3.2, 1.2.3.3, 1.2.3.4
    Version v1_2_3_1 = crearVersion(1);
    Version v1_2_3_2 = crearVersion(2);
    Version v1_2_3_3 = crearVersion(3);
    Version v1_2_3_4 = crearVersion(4);
    
    v1_2_3_1->padre = v1_2_3;
    v1_2_3_2->padre = v1_2_3;
    v1_2_3_3->padre = v1_2_3;
    v1_2_3_4->padre = v1_2_3;
    
    v1_2_3_1->siguienteHermano = v1_2_3_2;
    v1_2_3_2->siguienteHermano = v1_2_3_3;
    v1_2_3_3->siguienteHermano = v1_2_3_4;
    v1_2_3_4->siguienteHermano = nullptr;
    
    v1_2_3->primerHijo = v1_2_3_1;
}

// Función auxiliar para construir string de versión desde secuencia
void construirStringVersion(int* secuencia, int longitud, char* resultado) {
    resultado[0] = '\0';
    for (int i = 0; i < longitud; i++) {
        char num[10];
        sprintf(num, "%d", secuencia[i]);
        strcat(resultado, num);
        if (i < longitud - 1) {
            strcat(resultado, ".");
        }
    }
}

// Función de prueba
void testNavegacion(const char* nombre, Version raiz, int* secuencia, int longitud, bool debeExistir) {
    cout << COLOR_CYAN << "TEST: " << COLOR_RESET << nombre << " → ";
    
    char versionStr[100];
    if (secuencia != nullptr && longitud > 0) {
        construirStringVersion(secuencia, longitud, versionStr);
    } else {
        strcpy(versionStr, "(inválido)");
    }
    
    Version resultado = navegarAVersion(raiz, secuencia, longitud);
    
    bool exito = false;
    
    if (debeExistir) {
        if (resultado != nullptr) {
            // Verificar que sea el nodo correcto
            // El último número de la secuencia debe coincidir con el número del nodo
            if (resultado->numero == secuencia[longitud - 1]) {
                exito = true;
            }
        }
    } else {
        if (resultado == nullptr) {
            exito = true;
        }
    }
    
    if (exito) {
        cout << COLOR_GREEN << "✓ PASS" << COLOR_RESET;
        cout << " (versión \"" << versionStr << "\"";
        if (debeExistir) {
            cout << " encontrada)";
        } else {
            cout << " correctamente NULL)";
        }
        cout << endl;
        testsPasados++;
    } else {
        cout << COLOR_RED << "✗ FAIL" << COLOR_RESET;
        cout << " (versión \"" << versionStr << "\"";
        if (debeExistir) {
            cout << " NO encontrada, pero debería existir)";
        } else {
            cout << " encontrada, pero debería ser NULL)";
        }
        cout << endl;
        testsFallados++;
    }
}

int main() {
    cout << COLOR_BLUE << "========================================" << COLOR_RESET << endl;
    cout << COLOR_BLUE << "  TEST NAVEGACIÓN DEL ÁRBOL N-ARIO" << COLOR_RESET << endl;
    cout << COLOR_BLUE << "  TARJETA 3 - Fase 2" << COLOR_RESET << endl;
    cout << COLOR_BLUE << "========================================" << COLOR_RESET << endl;
    cout << endl;
    
    // Construir árbol de prueba
    cout << COLOR_YELLOW << "Construyendo árbol de prueba..." << COLOR_RESET << endl;
    cout << "Estructura del árbol:" << endl;
    cout << "1" << endl;
    cout << "  1.1" << endl;
    cout << "  1.2" << endl;
    cout << "    1.2.1" << endl;
    cout << "    1.2.2" << endl;
    cout << "    1.2.3" << endl;
    cout << "      1.2.3.1" << endl;
    cout << "      1.2.3.2" << endl;
    cout << "      1.2.3.3" << endl;
    cout << "      1.2.3.4" << endl;
    cout << "  1.3" << endl;
    cout << "2" << endl;
    cout << "  2.1" << endl;
    cout << "  2.2" << endl;
    cout << "  2.3" << endl;
    cout << "3" << endl;
    cout << endl;
    
    Version raiz = nullptr;
    construirArbolPrueba(raiz);
    
    // TESTS DE NAVEGACIÓN
    cout << COLOR_YELLOW << "Tests de navegación exitosa:" << COLOR_RESET << endl;
    
    // Caso 1: Versión de primer nivel
    int seq1[] = {1};
    testNavegacion("Versión primer nivel [1]", raiz, seq1, 1, true);
    
    int seq2[] = {2};
    testNavegacion("Versión primer nivel [2]", raiz, seq2, 1, true);
    
    int seq3[] = {3};
    testNavegacion("Versión primer nivel [3]", raiz, seq3, 1, true);
    
    // Caso 2: Subversión de segundo nivel
    int seq4[] = {1, 1};
    testNavegacion("Subversión segundo nivel [1, 1]", raiz, seq4, 2, true);
    
    int seq5[] = {1, 2};
    testNavegacion("Subversión segundo nivel [1, 2]", raiz, seq5, 2, true);
    
    int seq6[] = {2, 3};
    testNavegacion("Subversión segundo nivel [2, 3]", raiz, seq6, 2, true);
    
    // Caso 3: Versión de tercer nivel
    int seq7[] = {1, 2, 1};
    testNavegacion("Versión tercer nivel [1, 2, 1]", raiz, seq7, 3, true);
    
    int seq8[] = {1, 2, 3};
    testNavegacion("Versión tercer nivel [1, 2, 3]", raiz, seq8, 3, true);
    
    // Caso 4: Versión profunda (4 niveles)
    int seq9[] = {1, 2, 3, 1};
    testNavegacion("Versión profunda [1, 2, 3, 1]", raiz, seq9, 4, true);
    
    int seq10[] = {1, 2, 3, 4};
    testNavegacion("Versión profunda [1, 2, 3, 4]", raiz, seq10, 4, true);
    
    cout << endl;
    cout << COLOR_YELLOW << "Tests de navegación fallida (versiones inexistentes):" << COLOR_RESET << endl;
    
    // Caso 5: Versión inexistente de primer nivel
    int seq11[] = {4};
    testNavegacion("Versión inexistente [4]", raiz, seq11, 1, false);
    
    int seq12[] = {5};
    testNavegacion("Versión inexistente [5]", raiz, seq12, 1, false);
    
    // Caso 6: Camino inexistente (primer nivel existe, segundo no)
    int seq13[] = {1, 5};
    testNavegacion("Camino inexistente [1, 5]", raiz, seq13, 2, false);
    
    int seq14[] = {5, 1};
    testNavegacion("Camino inexistente [5, 1]", raiz, seq14, 2, false);
    
    // Caso 7: Camino profundo inexistente
    int seq15[] = {1, 2, 5};
    testNavegacion("Camino inexistente [1, 2, 5]", raiz, seq15, 3, false);
    
    int seq16[] = {1, 2, 3, 5};
    testNavegacion("Camino inexistente [1, 2, 3, 5]", raiz, seq16, 4, false);
    
    // Caso 8: Versión sin hijos
    int seq17[] = {3, 1};
    testNavegacion("Versión sin hijos [3, 1]", raiz, seq17, 2, false);
    
    cout << endl;
    cout << COLOR_YELLOW << "Tests de casos especiales:" << COLOR_RESET << endl;
    
    // Caso 9: Árbol vacío
    Version arbolVacio = nullptr;
    testNavegacion("Árbol vacío [1]", arbolVacio, seq1, 1, false);
    
    // Caso 10: Secuencia NULL
    int* secNull = nullptr;
    testNavegacion("Secuencia NULL", raiz, secNull, 1, false);
    
    // Caso 11: Longitud 0
    testNavegacion("Longitud 0", raiz, seq1, 0, false);
    
    cout << endl;
    
    // Liberar memoria
    cout << COLOR_YELLOW << "Liberando memoria del árbol de prueba..." << COLOR_RESET << endl;
    liberarArbolVersiones(raiz);
    
    // RESUMEN
    cout << endl;
    cout << COLOR_BLUE << "========================================" << COLOR_RESET << endl;
    cout << COLOR_BLUE << "  RESUMEN DE RESULTADOS" << COLOR_RESET << endl;
    cout << COLOR_BLUE << "========================================" << COLOR_RESET << endl;
    cout << endl;
    cout << "Tests ejecutados: " << (testsPasados + testsFallados) << endl;
    cout << COLOR_GREEN << "Tests pasados:    " << testsPasados << COLOR_RESET << endl;
    cout << COLOR_RED << "Tests fallados:   " << testsFallados << COLOR_RESET << endl;
    cout << endl;
    
    if (testsFallados == 0) {
        cout << COLOR_GREEN << "✓ TODOS LOS TESTS PASARON" << COLOR_RESET << endl;
        return 0;
    } else {
        cout << COLOR_RED << "✗ HAY TESTS FALLANDO" << COLOR_RESET << endl;
        return 1;
    }
}
