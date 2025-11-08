// Test de VersionIndependiente
// Verifica que se cree una version independiente con texto completo

#include <iostream>
#include <cstring>
#include <cassert>
using namespace std;

// Declaraciones forward de las funciones del sistema
extern "C" {
    typedef struct nodo_archivo* Archivo;
    typedef enum { OK, ERROR, NO_IMPLEMENTADA } TipoRet;
    
    Archivo CrearArchivo(char* nombre);
    TipoRet CrearVersion(Archivo &a, char* version, char* error);
    TipoRet InsertarLinea(Archivo &a, char* version, char* linea, unsigned int nroLinea, char* error);
    TipoRet BorrarLinea(Archivo &a, char* version, unsigned int nroLinea, char* error);
    TipoRet MostrarTexto(Archivo a, char* version);
    TipoRet MostrarVersiones(Archivo a);
    TipoRet Iguales(Archivo a, char* version1, char* version2, bool &iguales);
    TipoRet VersionIndependiente(Archivo &a, char* version);
    TipoRet BorrarArchivo(Archivo &a);
}

void test_version_independiente_basico() {
    cout << "\n=== TEST 1: Version Independiente Basico ===\n";
    
    char error[100];
    Archivo a = CrearArchivo((char*)"test.txt");
    assert(a != NULL);
    
    // Crear version 1 con contenido base
    assert(CrearVersion(a, (char*)"1", error) == OK);
    assert(InsertarLinea(a, (char*)"1", (char*)"Linea A", 1, error) == OK);
    assert(InsertarLinea(a, (char*)"1", (char*)"Linea B", 2, error) == OK);
    
    cout << "\nTexto de version 1:\n";
    MostrarTexto(a, (char*)"1");
    
    // Crear version 1.1 que modifica la version 1
    assert(CrearVersion(a, (char*)"1.1", error) == OK);
    assert(InsertarLinea(a, (char*)"1.1", (char*)"Linea C", 3, error) == OK);
    assert(BorrarLinea(a, (char*)"1.1", 1, error) == OK);  // Borra "Linea A"
    
    cout << "\nTexto de version 1.1 (debe tener B, C):\n";
    MostrarTexto(a, (char*)"1.1");
    
    // Crear version independiente
    cout << "\n>>> Ejecutando VersionIndependiente(1.1)...\n";
    TipoRet ret = VersionIndependiente(a, (char*)"1.1");
    
    if (ret == NO_IMPLEMENTADA) {
        cout << "ERROR: VersionIndependiente retorna NO_IMPLEMENTADA\n";
        assert(false);
    }
    
    assert(ret == OK);
    cout << "OK: VersionIndependiente ejecutada exitosamente\n";
    
    // Verificar que se creo version 2
    cout << "\nVersiones del archivo (debe aparecer version 2):\n";
    MostrarVersiones(a);
    
    // Verificar que version 2 tiene el mismo texto que 1.1
    cout << "\nTexto de version 2 (debe tener B, C igual que 1.1):\n";
    MostrarTexto(a, (char*)"2");
    
    // Verificar que son iguales
    bool iguales;
    assert(Iguales(a, (char*)"1.1", (char*)"2", iguales) == OK);
    cout << "\nVersiones 1.1 y 2 son " << (iguales ? "IGUALES" : "DISTINTAS") << "\n";
    assert(iguales == true);
    
    cout << "\n✅ TEST 1 PASSED: Version independiente creada correctamente\n";
    
    BorrarArchivo(a);
}

void test_version_independiente_es_realmente_independiente() {
    cout << "\n=== TEST 2: Verificar que es REALMENTE independiente ===\n";
    
    char error[100];
    Archivo a = CrearArchivo((char*)"test2.txt");
    
    // Crear version 1 y 1.1
    CrearVersion(a, (char*)"1", error);
    InsertarLinea(a, (char*)"1", (char*)"Linea A", 1, error);
    InsertarLinea(a, (char*)"1", (char*)"Linea B", 2, error);
    
    CrearVersion(a, (char*)"1.1", error);
    InsertarLinea(a, (char*)"1.1", (char*)"Linea C", 3, error);
    
    cout << "\nTexto ANTES de crear version independiente (1.1):\n";
    MostrarTexto(a, (char*)"1.1");
    
    // Crear version independiente
    VersionIndependiente(a, (char*)"1.1");
    
    cout << "\nTexto de version 2 (nueva independiente):\n";
    MostrarTexto(a, (char*)"2");
    
    // Verificar que son iguales
    bool iguales;
    Iguales(a, (char*)"1.1", (char*)"2", iguales);
    cout << "Antes de modificar: 1.1 y 2 son " << (iguales ? "IGUALES ✅" : "DISTINTAS ❌") << "\n";
    assert(iguales == true);
    
    // CLAVE: Modificar la version original 1.1
    cout << "\n>>> MODIFICANDO version original 1.1 (agregando Linea D)...\n";
    InsertarLinea(a, (char*)"1.1", (char*)"Linea D", 1, error);
    
    cout << "\nTexto de 1.1 DESPUES de modificar (debe tener D, A, B, C):\n";
    MostrarTexto(a, (char*)"1.1");
    
    cout << "\nTexto de 2 (independiente, debe seguir igual: A, B, C):\n";
    MostrarTexto(a, (char*)"2");
    
    // Verificar que AHORA son distintas
    Iguales(a, (char*)"1.1", (char*)"2", iguales);
    cout << "\nDespues de modificar: 1.1 y 2 son " << (iguales ? "IGUALES ❌" : "DISTINTAS ✅") << "\n";
    assert(iguales == false);
    
    cout << "\n✅ TEST 2 PASSED: Version 2 es REALMENTE independiente\n";
    
    BorrarArchivo(a);
}

void test_version_independiente_profunda() {
    cout << "\n=== TEST 3: Version Independiente de version profunda ===\n";
    
    char error[100];
    Archivo a = CrearArchivo((char*)"test3.txt");
    
    // Crear jerarquia: 1 -> 1.1 -> 1.1.1
    CrearVersion(a, (char*)"1", error);
    InsertarLinea(a, (char*)"1", (char*)"Base", 1, error);
    
    CrearVersion(a, (char*)"1.1", error);
    InsertarLinea(a, (char*)"1.1", (char*)"Nivel2", 2, error);
    
    CrearVersion(a, (char*)"1.1.1", error);
    InsertarLinea(a, (char*)"1.1.1", (char*)"Nivel3", 3, error);
    
    cout << "\nTexto de version 1.1.1 (profunda):\n";
    MostrarTexto(a, (char*)"1.1.1");
    
    // Crear version independiente de la profunda
    VersionIndependiente(a, (char*)"1.1.1");
    
    cout << "\nVersiones (debe aparecer 2 al final):\n";
    MostrarVersiones(a);
    
    cout << "\nTexto de version 2 (debe tener Base, Nivel2, Nivel3):\n";
    MostrarTexto(a, (char*)"2");
    
    // Verificar igualdad
    bool iguales;
    Iguales(a, (char*)"1.1.1", (char*)"2", iguales);
    cout << "\nVersiones 1.1.1 y 2 son " << (iguales ? "IGUALES ✅" : "DISTINTAS ❌") << "\n";
    assert(iguales == true);
    
    cout << "\n✅ TEST 3 PASSED: Version independiente de jerarquia profunda\n";
    
    BorrarArchivo(a);
}

void test_version_independiente_vacia() {
    cout << "\n=== TEST 4: Version Independiente de version vacia ===\n";
    
    char error[100];
    Archivo a = CrearArchivo((char*)"test4.txt");
    
    // Crear version vacia
    CrearVersion(a, (char*)"1", error);
    
    cout << "\nTexto de version 1 (vacia):\n";
    MostrarTexto(a, (char*)"1");
    
    // Crear version independiente de la vacia
    VersionIndependiente(a, (char*)"1");
    
    cout << "\nVersiones:\n";
    MostrarVersiones(a);
    
    cout << "\nTexto de version 2 (debe estar vacia tambien):\n";
    MostrarTexto(a, (char*)"2");
    
    // Verificar igualdad
    bool iguales;
    Iguales(a, (char*)"1", (char*)"2", iguales);
    cout << "\nVersiones 1 y 2 son " << (iguales ? "IGUALES ✅" : "DISTINTAS ❌") << "\n";
    assert(iguales == true);
    
    cout << "\n✅ TEST 4 PASSED: Version independiente de version vacia\n";
    
    BorrarArchivo(a);
}

int main() {
    cout << "\n";
    cout << "╔══════════════════════════════════════════════════════╗\n";
    cout << "║   TEST VERSIONINDEPENDIENTE - Validacion Completa   ║\n";
    cout << "╚══════════════════════════════════════════════════════╝\n";
    
    try {
        test_version_independiente_basico();
        test_version_independiente_es_realmente_independiente();
        test_version_independiente_profunda();
        test_version_independiente_vacia();
        
        cout << "\n";
        cout << "╔══════════════════════════════════════════════════════╗\n";
        cout << "║            ✅ TODOS LOS TESTS PASARON ✅            ║\n";
        cout << "║  VersionIndependiente funciona PERFECTAMENTE 🚀      ║\n";
        cout << "╚══════════════════════════════════════════════════════╝\n";
        cout << "\n";
        
        return 0;
    } catch (...) {
        cout << "\n❌ TESTS FALLARON\n";
        return 1;
    }
}
