// Estructuras de Datos y Algoritmos - Obligatorio 2025
// Test Suite para CrearVersion Completa
//
// TARJETA 6: CrearVersion Completa con Jerarquía
// Tests de integración completa

#include <iostream>
#include <cstring>
#include "archivo.h"
#include "utils.h"
#include "version.h"

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

void testCrearVersion(const char* nombre, bool esperaExito) {
    cout << COLOR_CYAN << "TEST: " << COLOR_RESET << nombre << " → ";
    
    bool exito = !esperaExito;  // Invertir para la lógica del test
    
    if (exito == esperaExito) {
        cout << COLOR_GREEN << "✓ PASS" << COLOR_RESET << endl;
        testsPasados++;
    } else {
        cout << COLOR_RED << "✗ FAIL" << COLOR_RESET << endl;
        testsFallados++;
    }
}

// Función auxiliar para verificar que una versión existe
bool versionExiste(Archivo a, const char* versionStr) {
    int* secuencia = nullptr;
    int longitud = 0;
    secuencia = parsearVersion(versionStr, &longitud);
    
    if (secuencia == nullptr) {
        return false;
    }
    
    Version v = navegarAVersion(a->primeraVersion, secuencia, longitud);
    liberarArrayVersion(secuencia);
    
    return (v != nullptr);
}

// Función auxiliar para contar versiones de primer nivel
int contarVersionesPrimerNivel(Archivo a) {
    int count = 0;
    Version actual = a->primeraVersion;
    while (actual != nullptr) {
        count++;
        actual = actual->siguienteHermano;
    }
    return count;
}

// Función auxiliar para contar hijos de una versión
int contarHijos(Version padre) {
    int count = 0;
    Version actual = padre->primerHijo;
    while (actual != nullptr) {
        count++;
        actual = actual->siguienteHermano;
    }
    return count;
}

int main() {
    cout << COLOR_BLUE << "========================================" << COLOR_RESET << endl;
    cout << COLOR_BLUE << "  TEST CREARVERSION COMPLETA" << COLOR_RESET << endl;
    cout << COLOR_BLUE << "========================================" << COLOR_RESET << endl;
    cout << endl;
    
    // =======================
    // TESTS DE VERSIONES DE PRIMER NIVEL
    // =======================
    cout << COLOR_YELLOW << "Tests de versiones de primer nivel:" << COLOR_RESET << endl;
    
    // TEST 1: Crear primera versión "1"
    {
        Archivo a = CrearArchivo((char*)"test1.txt");
        char error[100];
        
        TipoRet ret = CrearVersion(a, (char*)"1", error);
        bool ok = (ret == OK) && versionExiste(a, "1") && (contarVersionesPrimerNivel(a) == 1);
        
        if (ok) {
            cout << COLOR_CYAN << "TEST: " << COLOR_RESET << "Crear primera versión '1' → " 
                 << COLOR_GREEN << "✓ PASS" << COLOR_RESET << endl;
            testsPasados++;
        } else {
            cout << COLOR_CYAN << "TEST: " << COLOR_RESET << "Crear primera versión '1' → " 
                 << COLOR_RED << "✗ FAIL" << COLOR_RESET;
            if (ret != OK) cout << " (Error: " << error << ")";
            cout << endl;
            testsFallados++;
        }
        
        BorrarArchivo(a);
    }
    
    // TEST 2: Crear versiones consecutivas "1", "2", "3"
    {
        Archivo a = CrearArchivo((char*)"test2.txt");
        char error[100];
        
        CrearVersion(a, (char*)"1", error);
        CrearVersion(a, (char*)"2", error);
        TipoRet ret = CrearVersion(a, (char*)"3", error);
        
        bool ok = (ret == OK) && versionExiste(a, "1") && versionExiste(a, "2") && 
                  versionExiste(a, "3") && (contarVersionesPrimerNivel(a) == 3);
        
        if (ok) {
            cout << COLOR_CYAN << "TEST: " << COLOR_RESET << "Crear versiones '1', '2', '3' → " 
                 << COLOR_GREEN << "✓ PASS" << COLOR_RESET << endl;
            testsPasados++;
        } else {
            cout << COLOR_CYAN << "TEST: " << COLOR_RESET << "Crear versiones '1', '2', '3' → " 
                 << COLOR_RED << "✗ FAIL" << COLOR_RESET << endl;
            testsFallados++;
        }
        
        BorrarArchivo(a);
    }
    
    // TEST 3: Error - crear versión con hueco (1, 3)
    {
        Archivo a = CrearArchivo((char*)"test3.txt");
        char error[100];
        
        CrearVersion(a, (char*)"1", error);
        TipoRet ret = CrearVersion(a, (char*)"3", error);
        
        bool ok = (ret == ERROR) && !versionExiste(a, "3");
        
        if (ok) {
            cout << COLOR_CYAN << "TEST: " << COLOR_RESET << "Error hueco [1] crear '3' → " 
                 << COLOR_GREEN << "✓ PASS" << COLOR_RESET << " (rechazado correctamente)" << endl;
            testsPasados++;
        } else {
            cout << COLOR_CYAN << "TEST: " << COLOR_RESET << "Error hueco [1] crear '3' → " 
                 << COLOR_RED << "✗ FAIL" << COLOR_RESET << " (debería rechazar)" << endl;
            testsFallados++;
        }
        
        BorrarArchivo(a);
    }
    
    cout << endl;
    
    // =======================
    // TESTS DE SUBVERSIONES
    // =======================
    cout << COLOR_YELLOW << "Tests de subversiones:" << COLOR_RESET << endl;
    
    // TEST 4: Crear subversión "1.1"
    {
        Archivo a = CrearArchivo((char*)"test4.txt");
        char error[100];
        
        CrearVersion(a, (char*)"1", error);
        TipoRet ret = CrearVersion(a, (char*)"1.1", error);
        
        int seq[] = {1};
        Version v1 = navegarAVersion(a->primeraVersion, seq, 1);
        bool ok = (ret == OK) && versionExiste(a, "1.1") && (v1 != nullptr) && 
                  (contarHijos(v1) == 1);
        
        if (ok) {
            cout << COLOR_CYAN << "TEST: " << COLOR_RESET << "Crear subversión '1.1' → " 
                 << COLOR_GREEN << "✓ PASS" << COLOR_RESET << endl;
            testsPasados++;
        } else {
            cout << COLOR_CYAN << "TEST: " << COLOR_RESET << "Crear subversión '1.1' → " 
                 << COLOR_RED << "✗ FAIL" << COLOR_RESET << endl;
            testsFallados++;
        }
        
        BorrarArchivo(a);
    }
    
    // TEST 5: Crear múltiples subversiones "1.1", "1.2", "1.3"
    {
        Archivo a = CrearArchivo((char*)"test5.txt");
        char error[100];
        
        CrearVersion(a, (char*)"1", error);
        CrearVersion(a, (char*)"1.1", error);
        CrearVersion(a, (char*)"1.2", error);
        TipoRet ret = CrearVersion(a, (char*)"1.3", error);
        
        int seq[] = {1};
        Version v1 = navegarAVersion(a->primeraVersion, seq, 1);
        bool ok = (ret == OK) && versionExiste(a, "1.1") && versionExiste(a, "1.2") && 
                  versionExiste(a, "1.3") && (v1 != nullptr) && (contarHijos(v1) == 3);
        
        if (ok) {
            cout << COLOR_CYAN << "TEST: " << COLOR_RESET << "Crear subversiones '1.1', '1.2', '1.3' → " 
                 << COLOR_GREEN << "✓ PASS" << COLOR_RESET << endl;
            testsPasados++;
        } else {
            cout << COLOR_CYAN << "TEST: " << COLOR_RESET << "Crear subversiones '1.1', '1.2', '1.3' → " 
                 << COLOR_RED << "✗ FAIL" << COLOR_RESET << endl;
            testsFallados++;
        }
        
        BorrarArchivo(a);
    }
    
    // TEST 6: Error - crear subversión sin padre "2.1" (padre '2' no existe)
    {
        Archivo a = CrearArchivo((char*)"test6.txt");
        char error[100];
        
        CrearVersion(a, (char*)"1", error);
        TipoRet ret = CrearVersion(a, (char*)"2.1", error);
        
        bool ok = (ret == ERROR) && !versionExiste(a, "2.1");
        
        if (ok) {
            cout << COLOR_CYAN << "TEST: " << COLOR_RESET << "Error padre inexistente '2.1' → " 
                 << COLOR_GREEN << "✓ PASS" << COLOR_RESET << " (rechazado correctamente)" << endl;
            testsPasados++;
        } else {
            cout << COLOR_CYAN << "TEST: " << COLOR_RESET << "Error padre inexistente '2.1' → " 
                 << COLOR_RED << "✗ FAIL" << COLOR_RESET << " (debería rechazar)" << endl;
            testsFallados++;
        }
        
        BorrarArchivo(a);
    }
    
    // TEST 7: Crear versión profunda "1.2.3.4"
    {
        Archivo a = CrearArchivo((char*)"test7.txt");
        char error[100];
        
        CrearVersion(a, (char*)"1", error);
        CrearVersion(a, (char*)"1.1", error);
        CrearVersion(a, (char*)"1.2", error);
        CrearVersion(a, (char*)"1.2.1", error);
        CrearVersion(a, (char*)"1.2.2", error);
        CrearVersion(a, (char*)"1.2.3", error);
        CrearVersion(a, (char*)"1.2.3.1", error);
        CrearVersion(a, (char*)"1.2.3.2", error);
        CrearVersion(a, (char*)"1.2.3.3", error);
        TipoRet ret = CrearVersion(a, (char*)"1.2.3.4", error);
        
        bool ok = (ret == OK) && versionExiste(a, "1.2.3.4");
        
        if (ok) {
            cout << COLOR_CYAN << "TEST: " << COLOR_RESET << "Crear versión profunda '1.2.3.4' → " 
                 << COLOR_GREEN << "✓ PASS" << COLOR_RESET << endl;
            testsPasados++;
        } else {
            cout << COLOR_CYAN << "TEST: " << COLOR_RESET << "Crear versión profunda '1.2.3.4' → " 
                 << COLOR_RED << "✗ FAIL" << COLOR_RESET << endl;
            testsFallados++;
        }
        
        BorrarArchivo(a);
    }
    
    cout << endl;
    
    // =======================
    // TESTS DE DESPLAZAMIENTO
    // =======================
    cout << COLOR_YELLOW << "Tests de desplazamiento:" << COLOR_RESET << endl;
    
    // TEST 8: Desplazamiento simple - crear '2' cuando ya existe
    {
        Archivo a = CrearArchivo((char*)"test8.txt");
        char error[100];
        
        CrearVersion(a, (char*)"1", error);
        CrearVersion(a, (char*)"2", error);
        CrearVersion(a, (char*)"3", error);
        
        // Ahora crear nueva versión '2', debería desplazar el '2' actual a '3'
        TipoRet ret = CrearVersion(a, (char*)"2", error);
        
        // Verificar que ahora hay 4 versiones de primer nivel
        bool ok = (ret == OK) && (contarVersionesPrimerNivel(a) == 4);
        
        if (ok) {
            cout << COLOR_CYAN << "TEST: " << COLOR_RESET << "Desplazamiento simple [1,2,3] crear '2' → " 
                 << COLOR_GREEN << "✓ PASS" << COLOR_RESET << " (ahora hay 4 versiones)" << endl;
            testsPasados++;
        } else {
            cout << COLOR_CYAN << "TEST: " << COLOR_RESET << "Desplazamiento simple [1,2,3] crear '2' → " 
                 << COLOR_RED << "✗ FAIL" << COLOR_RESET << endl;
            testsFallados++;
        }
        
        BorrarArchivo(a);
    }
    
    // TEST 9: Desplazamiento con hijos
    {
        Archivo a = CrearArchivo((char*)"test9.txt");
        char error[100];
        
        CrearVersion(a, (char*)"1", error);
        CrearVersion(a, (char*)"2", error);
        CrearVersion(a, (char*)"2.1", error);
        CrearVersion(a, (char*)"2.2", error);
        CrearVersion(a, (char*)"3", error);
        
        // Crear nueva versión '2', debería desplazar el '2' actual (con hijos) a '3'
        TipoRet ret = CrearVersion(a, (char*)"2", error);
        
        // Ahora debería haber 4 versiones de primer nivel
        // Y la versión que era '2.1' ahora debería ser '3.1'
        bool ok = (ret == OK) && (contarVersionesPrimerNivel(a) == 4) && 
                  versionExiste(a, "3.1") && versionExiste(a, "3.2");
        
        if (ok) {
            cout << COLOR_CYAN << "TEST: " << COLOR_RESET << "Desplazamiento con hijos 2→3, 2.1→3.1 → " 
                 << COLOR_GREEN << "✓ PASS" << COLOR_RESET << endl;
            testsPasados++;
        } else {
            cout << COLOR_CYAN << "TEST: " << COLOR_RESET << "Desplazamiento con hijos 2→3, 2.1→3.1 → " 
                 << COLOR_RED << "✗ FAIL" << COLOR_RESET << endl;
            testsFallados++;
        }
        
        BorrarArchivo(a);
    }
    
    cout << endl;
    
    // =======================
    // TESTS DE CASOS COMPLEJOS
    // =======================
    cout << COLOR_YELLOW << "Tests de casos complejos:" << COLOR_RESET << endl;
    
    // TEST 10: Estructura compleja con múltiples niveles
    {
        Archivo a = CrearArchivo((char*)"test10.txt");
        char error[100];
        
        // Crear estructura compleja
        CrearVersion(a, (char*)"1", error);
        CrearVersion(a, (char*)"1.1", error);
        CrearVersion(a, (char*)"1.2", error);
        CrearVersion(a, (char*)"1.2.1", error);
        CrearVersion(a, (char*)"2", error);
        CrearVersion(a, (char*)"2.1", error);
        CrearVersion(a, (char*)"2.1.1", error);
        TipoRet ret = CrearVersion(a, (char*)"2.1.2", error);
        
        bool ok = (ret == OK) && versionExiste(a, "1.2.1") && 
                  versionExiste(a, "2.1.1") && versionExiste(a, "2.1.2");
        
        if (ok) {
            cout << COLOR_CYAN << "TEST: " << COLOR_RESET << "Estructura compleja múltiples niveles → " 
                 << COLOR_GREEN << "✓ PASS" << COLOR_RESET << endl;
            testsPasados++;
        } else {
            cout << COLOR_CYAN << "TEST: " << COLOR_RESET << "Estructura compleja múltiples niveles → " 
                 << COLOR_RED << "✗ FAIL" << COLOR_RESET << endl;
            testsFallados++;
        }
        
        BorrarArchivo(a);
    }
    
    cout << endl;
    
    // RESUMEN
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
