// Estructuras de Datos y Algoritmos - Obligatorio 2025
// Test Suite para Validaciones de CrearVersion
//
// TARJETA 4: Validaciones de CrearVersion
// Tests para:
// 1. validarPadreExiste()
// 2. validarSinHuecos()

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
Version construirArbolPrueba() {
    // Crear versiones de primer nivel: 1, 2, 3
    Version v1 = crearVersion(1);
    Version v2 = crearVersion(2);
    Version v3 = crearVersion(3);
    
    v1->siguienteHermano = v2;
    v2->siguienteHermano = v3;
    v3->siguienteHermano = nullptr;
    
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
    
    // Crear hijos de versión 2: 2.1, 2.2
    Version v2_1 = crearVersion(1);
    Version v2_2 = crearVersion(2);
    
    v2_1->padre = v2;
    v2_2->padre = v2;
    
    v2_1->siguienteHermano = v2_2;
    v2_2->siguienteHermano = nullptr;
    
    v2->primerHijo = v2_1;
    
    return v1;  // Retornar raíz
}

// Función auxiliar para construir árbol con hueco
Version construirArbolConHueco() {
    // Crear versiones: 1, 3 (falta 2)
    Version v1 = crearVersion(1);
    Version v3 = crearVersion(3);
    
    v1->siguienteHermano = v3;
    v3->siguienteHermano = nullptr;
    
    return v1;
}

void testValidacionPadre(const char* nombre, Version raiz, int* secuencia, int longitud, bool debeExistir) {
    cout << COLOR_CYAN << "TEST: " << COLOR_RESET << nombre << " → ";
    
    bool resultado = validarPadreExiste(raiz, secuencia, longitud);
    
    bool exito = (resultado == debeExistir);
    
    if (exito) {
        cout << COLOR_GREEN << "✓ PASS" << COLOR_RESET;
        cout << " (padre ";
        if (debeExistir) {
            cout << "existe correctamente)";
        } else {
            cout << "NO existe, validación correcta)";
        }
        cout << endl;
        testsPasados++;
    } else {
        cout << COLOR_RED << "✗ FAIL" << COLOR_RESET;
        cout << " (esperado: " << (debeExistir ? "true" : "false");
        cout << ", obtenido: " << (resultado ? "true" : "false") << ")";
        cout << endl;
        testsFallados++;
    }
}

void testValidacionHuecos(const char* nombre, Version padre, Version hermanos, int numeroNuevo, bool debeSerValido) {
    cout << COLOR_CYAN << "TEST: " << COLOR_RESET << nombre << " → ";
    
    bool resultado = validarSinHuecos(padre, hermanos, numeroNuevo);
    
    bool exito = (resultado == debeSerValido);
    
    if (exito) {
        cout << COLOR_GREEN << "✓ PASS" << COLOR_RESET;
        cout << " (número " << numeroNuevo;
        if (debeSerValido) {
            cout << " válido)";
        } else {
            cout << " inválido, validación correcta)";
        }
        cout << endl;
        testsPasados++;
    } else {
        cout << COLOR_RED << "✗ FAIL" << COLOR_RESET;
        cout << " (esperado: " << (debeSerValido ? "válido" : "inválido");
        cout << ", obtenido: " << (resultado ? "válido" : "inválido") << ")";
        cout << endl;
        testsFallados++;
    }
}

int main() {
    cout << COLOR_BLUE << "========================================" << COLOR_RESET << endl;
    cout << COLOR_BLUE << "  TEST VALIDACIONES DE CREARVERSION" << COLOR_RESET << endl;
    cout << COLOR_BLUE << "  TARJETA 4 - Fase 2" << COLOR_RESET << endl;
    cout << COLOR_BLUE << "========================================" << COLOR_RESET << endl;
    cout << endl;
    
    // =======================
    // TESTS DE PADRE EXISTE
    // =======================
    cout << COLOR_YELLOW << "Tests de validarPadreExiste:" << COLOR_RESET << endl;
    
    Version raiz = construirArbolPrueba();
    
    // Caso 1: Versión de primer nivel (no tiene padre, siempre true)
    int seq1[] = {1};
    testValidacionPadre("Primer nivel [1]", raiz, seq1, 1, true);
    
    int seq2[] = {4};
    testValidacionPadre("Primer nivel [4]", raiz, seq2, 1, true);
    
    // Caso 2: Segundo nivel con padre existente
    int seq3[] = {1, 4};
    testValidacionPadre("Segundo nivel [1,4] - padre 1 existe", raiz, seq3, 2, true);
    
    int seq4[] = {2, 5};
    testValidacionPadre("Segundo nivel [2,5] - padre 2 existe", raiz, seq4, 2, true);
    
    // Caso 3: Segundo nivel con padre NO existente
    int seq5[] = {4, 1};
    testValidacionPadre("Segundo nivel [4,1] - padre 4 NO existe", raiz, seq5, 2, false);
    
    int seq6[] = {5, 1};
    testValidacionPadre("Segundo nivel [5,1] - padre 5 NO existe", raiz, seq6, 2, false);
    
    // Caso 4: Tercer nivel con padre existente
    int seq7[] = {1, 2, 1};
    testValidacionPadre("Tercer nivel [1,2,1] - padre 1.2 existe", raiz, seq7, 3, true);
    
    // Caso 5: Tercer nivel con padre NO existente
    int seq8[] = {1, 5, 1};
    testValidacionPadre("Tercer nivel [1,5,1] - padre 1.5 NO existe", raiz, seq8, 3, false);
    
    int seq9[] = {3, 1, 1};
    testValidacionPadre("Tercer nivel [3,1,1] - padre 3.1 NO existe", raiz, seq9, 3, false);
    
    cout << endl;
    
    // =======================
    // TESTS DE SIN HUECOS
    // =======================
    cout << COLOR_YELLOW << "Tests de validarSinHuecos:" << COLOR_RESET << endl;
    
    // CASO 1: Siguiente consecutivo (OK)
    // Hermanos [1,2,3], nuevo 4 → OK
    testValidacionHuecos("Hermanos [1,2,3], nuevo 4", nullptr, raiz, 4, true);
    
    // CASO 2: Desplazamiento (OK)
    // Hermanos [1,2,3], nuevo 2 → OK (desplazamiento)
    testValidacionHuecos("Hermanos [1,2,3], nuevo 2", nullptr, raiz, 2, true);
    testValidacionHuecos("Hermanos [1,2,3], nuevo 1", nullptr, raiz, 1, true);
    
    // CASO 3: Llenar hueco (OK)
    Version arbolHueco = construirArbolConHueco();
    testValidacionHuecos("Hermanos [1,3], nuevo 2", nullptr, arbolHueco, 2, true);
    
    // CASO 4: Crear hueco (ERROR)
    // Hermanos [1,2], nuevo 5 → ERROR (hueco en 3 y 4)
    Version v1 = crearVersion(1);
    Version v2 = crearVersion(2);
    v1->siguienteHermano = v2;
    v2->siguienteHermano = nullptr;
    testValidacionHuecos("Hermanos [1,2], nuevo 5", nullptr, v1, 5, false);
    testValidacionHuecos("Hermanos [1,2], nuevo 6", nullptr, v1, 6, false);
    
    // CASO 5: Número siguiente válido
    testValidacionHuecos("Hermanos [1,2], nuevo 3", nullptr, v1, 3, true);
    
    // CASO 6: Primer hijo debe ser 1
    testValidacionHuecos("Sin hermanos, nuevo 1", nullptr, nullptr, 1, true);
    testValidacionHuecos("Sin hermanos, nuevo 2", nullptr, nullptr, 2, false);
    
    // CASO 7: Números inválidos (<=0)
    testValidacionHuecos("Hermanos [1,2,3], nuevo 0", nullptr, raiz, 0, false);
    testValidacionHuecos("Hermanos [1,2,3], nuevo -1", nullptr, raiz, -1, false);
    
    // CASO 8: Hijos de un padre específico
    Version v1_padre = navegarAVersion(raiz, seq1, 1);
    testValidacionHuecos("Hijos de v1 [1,2,3], nuevo 4", v1_padre, nullptr, 4, true);
    testValidacionHuecos("Hijos de v1 [1,2,3], nuevo 2", v1_padre, nullptr, 2, true);
    
    // CASO 9: Hijos de v2 [1,2]
    int seq_v2[] = {2};
    Version v2_padre = navegarAVersion(raiz, seq_v2, 1);
    testValidacionHuecos("Hijos de v2 [1,2], nuevo 3", v2_padre, nullptr, 3, true);
    testValidacionHuecos("Hijos de v2 [1,2], nuevo 5", v2_padre, nullptr, 5, false);
    
    cout << endl;
    
    // =======================
    // TESTS DE CASOS LÍMITE
    // =======================
    cout << COLOR_YELLOW << "Tests de casos límite:" << COLOR_RESET << endl;
    
    // Árbol vacío
    testValidacionHuecos("Árbol vacío, nuevo 1", nullptr, nullptr, 1, true);
    testValidacionHuecos("Árbol vacío, nuevo 2", nullptr, nullptr, 2, false);
    
    // NULL checks
    testValidacionPadre("Secuencia NULL", raiz, nullptr, 1, false);
    testValidacionPadre("Longitud 0", raiz, seq1, 0, false);
    testValidacionPadre("Longitud negativa", raiz, seq1, -1, false);
    
    cout << endl;
    
    // Liberar memoria
    cout << COLOR_YELLOW << "Liberando memoria..." << COLOR_RESET << endl;
    liberarArbolVersiones(raiz);
    liberarArbolVersiones(arbolHueco);
    liberarArbolVersiones(v1);
    
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
