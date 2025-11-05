// Estructuras de Datos y Algoritmos - Obligatorio 2025
// Test Suite para Desplazamiento y Renumeración
//
// TARJETA 5: Desplazamiento y Renumeración
// Tests para:
// 1. renumerarSubarbol()
// 2. desplazarYRenumerar()

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

// Función auxiliar para verificar números de versiones
bool verificarNumero(Version ver, int esperado, const char* nombre) {
    if (ver == nullptr) {
        cout << COLOR_RED << "  ERROR: " << nombre << " es NULL" << COLOR_RESET << endl;
        return false;
    }
    if (ver->numero != esperado) {
        cout << COLOR_RED << "  ERROR: " << nombre << " tiene numero " << ver->numero 
             << ", esperado " << esperado << COLOR_RESET << endl;
        return false;
    }
    return true;
}

// Función auxiliar para imprimir árbol (debugging)
void imprimirArbol(Version raiz, int nivel = 0) {
    if (raiz == nullptr) return;
    
    // Imprimir indentación
    for (int i = 0; i < nivel; i++) {
        cout << "  ";
    }
    
    // Imprimir número
    cout << raiz->numero << endl;
    
    // Imprimir hijos
    Version hijo = raiz->primerHijo;
    while (hijo != nullptr) {
        imprimirArbol(hijo, nivel + 1);
        hijo = hijo->siguienteHermano;
    }
}

void testRenumeracion(const char* nombre, bool resultado) {
    cout << COLOR_CYAN << "TEST: " << COLOR_RESET << nombre << " → ";
    
    if (resultado) {
        cout << COLOR_GREEN << "✓ PASS" << COLOR_RESET << endl;
        testsPasados++;
    } else {
        cout << COLOR_RED << "✗ FAIL" << COLOR_RESET << endl;
        testsFallados++;
    }
}

int main() {
    cout << COLOR_BLUE << "========================================" << COLOR_RESET << endl;
    cout << COLOR_BLUE << "  TEST DESPLAZAMIENTO Y RENUMERACIÓN" << COLOR_RESET << endl;
    cout << COLOR_BLUE << "  TARJETA 5 - Fase 2" << COLOR_RESET << endl;
    cout << COLOR_BLUE << "========================================" << COLOR_RESET << endl;
    cout << endl;
    
    // =======================
    // TESTS DE RENUMERAR SUBÁRBOL
    // =======================
    cout << COLOR_YELLOW << "Tests de renumerarSubarbol:" << COLOR_RESET << endl;
    
    // TEST 1: Renumerar nodo único sin hijos
    {
        Version v1 = crearVersion(5);
        renumerarSubarbol(v1, 2);
        bool ok = verificarNumero(v1, 7, "v1");
        testRenumeracion("Nodo único: 5 + 2 = 7", ok);
        liberarVersion(v1);
    }
    
    // TEST 2: Renumerar nodo con un hijo
    {
        Version v1 = crearVersion(3);
        Version v1_1 = crearVersion(5);
        v1->primerHijo = v1_1;
        v1_1->padre = v1;
        
        renumerarSubarbol(v1, 1);
        
        bool ok = verificarNumero(v1, 4, "v1") && 
                  verificarNumero(v1_1, 6, "v1_1");
        testRenumeracion("Nodo con 1 hijo: 3->4, 5->6", ok);
        
        liberarVersion(v1_1);
        liberarVersion(v1);
    }
    
    // TEST 3: Renumerar nodo con múltiples hijos hermanos
    {
        Version v2 = crearVersion(2);
        Version v2_1 = crearVersion(1);
        Version v2_2 = crearVersion(2);
        Version v2_3 = crearVersion(3);
        
        v2->primerHijo = v2_1;
        v2_1->padre = v2;
        v2_2->padre = v2;
        v2_3->padre = v2;
        
        v2_1->siguienteHermano = v2_2;
        v2_2->siguienteHermano = v2_3;
        
        renumerarSubarbol(v2, 1);
        
        bool ok = verificarNumero(v2, 3, "v2") &&
                  verificarNumero(v2_1, 2, "v2_1") &&
                  verificarNumero(v2_2, 3, "v2_2") &&
                  verificarNumero(v2_3, 4, "v2_3");
        testRenumeracion("Nodo con 3 hijos: 2->3 con hijos 1->2, 2->3, 3->4", ok);
        
        liberarVersion(v2_3);
        liberarVersion(v2_2);
        liberarVersion(v2_1);
        liberarVersion(v2);
    }
    
    // TEST 4: Renumerar árbol profundo (3 niveles)
    {
        // Crear 1 -> 1.2 -> 1.2.3
        Version v1 = crearVersion(1);
        Version v1_2 = crearVersion(2);
        Version v1_2_3 = crearVersion(3);
        
        v1->primerHijo = v1_2;
        v1_2->padre = v1;
        v1_2->primerHijo = v1_2_3;
        v1_2_3->padre = v1_2;
        
        renumerarSubarbol(v1, 1);
        
        bool ok = verificarNumero(v1, 2, "v1") &&
                  verificarNumero(v1_2, 3, "v1_2") &&
                  verificarNumero(v1_2_3, 4, "v1_2_3");
        testRenumeracion("Árbol profundo: 1->2, 2->3, 3->4 (3 niveles)", ok);
        
        liberarVersion(v1_2_3);
        liberarVersion(v1_2);
        liberarVersion(v1);
    }
    
    // TEST 5: Renumerar con delta negativo
    {
        Version v5 = crearVersion(5);
        Version v5_3 = crearVersion(3);
        v5->primerHijo = v5_3;
        v5_3->padre = v5;
        
        renumerarSubarbol(v5, -2);
        
        bool ok = verificarNumero(v5, 3, "v5") &&
                  verificarNumero(v5_3, 1, "v5_3");
        testRenumeracion("Delta negativo: 5->3, 3->1 (delta -2)", ok);
        
        liberarVersion(v5_3);
        liberarVersion(v5);
    }
    
    // TEST 6: Renumerar NULL no crashea
    {
        renumerarSubarbol(nullptr, 5);
        testRenumeracion("NULL no crashea", true);
    }
    
    cout << endl;
    
    // =======================
    // TESTS DE DESPLAZAR Y RENUMERAR
    // =======================
    cout << COLOR_YELLOW << "Tests de desplazarYRenumerar:" << COLOR_RESET << endl;
    
    // TEST 7: Desplazar versiones de primer nivel
    {
        // Crear [1, 2, 3]
        Version v1 = crearVersion(1);
        Version v2 = crearVersion(2);
        Version v3 = crearVersion(3);
        
        v1->siguienteHermano = v2;
        v2->siguienteHermano = v3;
        
        Version primera = v1;
        
        // Desplazar desde 2 con delta +1: [1, 3, 4]
        desplazarYRenumerar(nullptr, primera, 2, 1);
        
        bool ok = verificarNumero(v1, 1, "v1") &&
                  verificarNumero(v2, 3, "v2") &&
                  verificarNumero(v3, 4, "v3");
        testRenumeracion("Primer nivel [1,2,3] desplazar desde 2: [1,3,4]", ok);
        
        liberarVersion(v3);
        liberarVersion(v2);
        liberarVersion(v1);
    }
    
    // TEST 8: Desplazar con hijos (caso complejo)
    {
        // Crear 2 con hijos 2.1, 2.2
        Version v2 = crearVersion(2);
        Version v2_1 = crearVersion(1);
        Version v2_2 = crearVersion(2);
        
        v2->primerHijo = v2_1;
        v2_1->padre = v2;
        v2_2->padre = v2;
        v2_1->siguienteHermano = v2_2;
        
        // Crear lista [1, 2, 3]
        Version v1 = crearVersion(1);
        Version v3 = crearVersion(3);
        v1->siguienteHermano = v2;
        v2->siguienteHermano = v3;
        
        Version primera = v1;
        
        // Desplazar desde 2 con delta +1
        // Resultado esperado: v2->3 con hijos 3.1, 3.2
        desplazarYRenumerar(nullptr, primera, 2, 1);
        
        bool ok = verificarNumero(v1, 1, "v1") &&
                  verificarNumero(v2, 3, "v2") &&
                  verificarNumero(v2_1, 2, "v2_1") &&
                  verificarNumero(v2_2, 3, "v2_2") &&
                  verificarNumero(v3, 4, "v3");
        testRenumeracion("Desplazar v2 con hijos: 2->3, 2.1->3.1, 2.2->3.2", ok);
        
        liberarVersion(v2_2);
        liberarVersion(v2_1);
        liberarVersion(v3);
        liberarVersion(v2);
        liberarVersion(v1);
    }
    
    // TEST 9: Desplazar hijos de un padre específico
    {
        // Crear v1 con hijos [1, 2, 3]
        Version v1 = crearVersion(1);
        Version v1_1 = crearVersion(1);
        Version v1_2 = crearVersion(2);
        Version v1_3 = crearVersion(3);
        
        v1->primerHijo = v1_1;
        v1_1->padre = v1;
        v1_2->padre = v1;
        v1_3->padre = v1;
        
        v1_1->siguienteHermano = v1_2;
        v1_2->siguienteHermano = v1_3;
        
        Version dummy = nullptr;
        
        // Desplazar hijos desde 2 con delta +1
        desplazarYRenumerar(v1, dummy, 2, 1);
        
        bool ok = verificarNumero(v1_1, 1, "v1_1") &&
                  verificarNumero(v1_2, 3, "v1_2") &&
                  verificarNumero(v1_3, 4, "v1_3");
        testRenumeracion("Desplazar hijos [1,2,3] desde 2: [1,3,4]", ok);
        
        liberarVersion(v1_3);
        liberarVersion(v1_2);
        liberarVersion(v1_1);
        liberarVersion(v1);
    }
    
    // TEST 10: Desplazar todos (desde 1)
    {
        Version v1 = crearVersion(1);
        Version v2 = crearVersion(2);
        Version v3 = crearVersion(3);
        
        v1->siguienteHermano = v2;
        v2->siguienteHermano = v3;
        
        Version primera = v1;
        
        desplazarYRenumerar(nullptr, primera, 1, 1);
        
        bool ok = verificarNumero(v1, 2, "v1") &&
                  verificarNumero(v2, 3, "v2") &&
                  verificarNumero(v3, 4, "v3");
        testRenumeracion("Desplazar todos desde 1: [1,2,3] -> [2,3,4]", ok);
        
        liberarVersion(v3);
        liberarVersion(v2);
        liberarVersion(v1);
    }
    
    // TEST 11: Desplazar solo el último
    {
        Version v1 = crearVersion(1);
        Version v2 = crearVersion(2);
        Version v3 = crearVersion(3);
        
        v1->siguienteHermano = v2;
        v2->siguienteHermano = v3;
        
        Version primera = v1;
        
        desplazarYRenumerar(nullptr, primera, 3, 1);
        
        bool ok = verificarNumero(v1, 1, "v1") &&
                  verificarNumero(v2, 2, "v2") &&
                  verificarNumero(v3, 4, "v3");
        testRenumeracion("Desplazar solo último: [1,2,3] desde 3 -> [1,2,4]", ok);
        
        liberarVersion(v3);
        liberarVersion(v2);
        liberarVersion(v1);
    }
    
    // TEST 12: Desplazar con delta negativo (BorrarVersion)
    {
        Version v1 = crearVersion(1);
        Version v3 = crearVersion(3);
        Version v4 = crearVersion(4);
        
        v1->siguienteHermano = v3;
        v3->siguienteHermano = v4;
        
        Version primera = v1;
        
        // Simular que borramos v2, desplazamos desde 3 con delta -1
        desplazarYRenumerar(nullptr, primera, 3, -1);
        
        bool ok = verificarNumero(v1, 1, "v1") &&
                  verificarNumero(v3, 2, "v3") &&
                  verificarNumero(v4, 3, "v4");
        testRenumeracion("Desplazar con delta -1: [1,3,4] -> [1,2,3]", ok);
        
        liberarVersion(v4);
        liberarVersion(v3);
        liberarVersion(v1);
    }
    
    // TEST 13: Lista vacía no crashea
    {
        Version dummy = nullptr;
        desplazarYRenumerar(nullptr, dummy, 1, 1);
        testRenumeracion("Lista vacía no crashea", true);
    }
    
    cout << endl;
    
    // =======================
    // TESTS DE ESCENARIOS COMPLEJOS
    // =======================
    cout << COLOR_YELLOW << "Tests de escenarios complejos:" << COLOR_RESET << endl;
    
    // TEST 14: Árbol completo con múltiples niveles
    {
        // Crear estructura:
        // 1
        // 2 -> 2.1 -> 2.1.1
        //   -> 2.2
        // 3
        
        Version v1 = crearVersion(1);
        Version v2 = crearVersion(2);
        Version v3 = crearVersion(3);
        
        Version v2_1 = crearVersion(1);
        Version v2_2 = crearVersion(2);
        Version v2_1_1 = crearVersion(1);
        
        // Primer nivel
        v1->siguienteHermano = v2;
        v2->siguienteHermano = v3;
        
        // Hijos de v2
        v2->primerHijo = v2_1;
        v2_1->padre = v2;
        v2_2->padre = v2;
        v2_1->siguienteHermano = v2_2;
        
        // Hijos de v2_1
        v2_1->primerHijo = v2_1_1;
        v2_1_1->padre = v2_1;
        
        Version primera = v1;
        
        // Desplazar desde 2 en primer nivel
        // Resultado: v2->3 con hijos 3.1->3.1.1, 3.2
        desplazarYRenumerar(nullptr, primera, 2, 1);
        
        bool ok = verificarNumero(v1, 1, "v1") &&
                  verificarNumero(v2, 3, "v2") &&
                  verificarNumero(v2_1, 2, "v2_1") &&
                  verificarNumero(v2_2, 3, "v2_2") &&
                  verificarNumero(v2_1_1, 2, "v2_1_1") &&
                  verificarNumero(v3, 4, "v3");
        
        testRenumeracion("Árbol completo 3 niveles con desplazamiento", ok);
        
        liberarVersion(v2_1_1);
        liberarVersion(v2_2);
        liberarVersion(v2_1);
        liberarVersion(v3);
        liberarVersion(v2);
        liberarVersion(v1);
    }
    
    cout << endl;
    
    // Liberar memoria
    cout << COLOR_YELLOW << "Todos los tests completados." << COLOR_RESET << endl;
    
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
