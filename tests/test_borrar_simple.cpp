// Test simple para BorrarVersion
#include <iostream>
#include <cstring>
#include <sstream>

extern "C" {
    #include "archivo.h"
    #include "definiciones.h"
}

using namespace std;

// Helper para verificar si una versión existe intentando insertar una línea
bool versionExiste(Archivo a, const char* versionStr) {
    char error[100];
    // Intentamos insertar una línea en la versión
    // Si retorna OK, la versión existe
    TipoRet ret = InsertarLinea(a, (char*)versionStr, (char*)"test_line", 1, error);
    
    // Si la versión existe, borramos la línea que insertamos
    if (ret == OK) {
        BorrarLinea(a, (char*)versionStr, 1, error);
        return true;
    }
    
    return false;
}

int main() {
    cout << "\n=== TEST BORRARVERSION ===" << endl;
    
    Archivo a = CrearArchivo((char*)"test");
    char error[100];
    int testsPasados = 0;
    int testsFallados = 0;
    
    // TEST 1: Crear estructura inicial
    cout << "\n1. Creando estructura inicial..." << endl;
    CrearVersion(a, (char*)"1", error);
    CrearVersion(a, (char*)"2", error);
    CrearVersion(a, (char*)"2.1", error);
    CrearVersion(a, (char*)"2.2", error);
    CrearVersion(a, (char*)"3", error);
    CrearVersion(a, (char*)"3.1", error);
    CrearVersion(a, (char*)"4", error);
    
    cout << "   Versiones creadas: 1, 2, 2.1, 2.2, 3, 3.1, 4" << endl;
    
    // Verificar estructura inicial
    if (versionExiste(a, "1") && versionExiste(a, "2") && 
        versionExiste(a, "2.1") && versionExiste(a, "2.2") &&
        versionExiste(a, "3") && versionExiste(a, "3.1") && 
        versionExiste(a, "4")) {
        cout << "   ✓ Todas las versiones existen" << endl;
        testsPasados++;
    } else {
        cout << "   ✗ Error en creación de versiones" << endl;
        testsFallados++;
    }
    
    // TEST 2: Borrar versión con hijos (versión 2)
    cout << "\n2. Borrando versión 2 (que tiene hijos 2.1 y 2.2)..." << endl;
    TipoRet ret = BorrarVersion(a, (char*)"2");
    
    if (ret == OK) {
        cout << "   ✓ BorrarVersion retornó OK" << endl;
        testsPasados++;
    } else {
        cout << "   ✗ BorrarVersion falló" << endl;
        testsFallados++;
    }
    
    // TEST 3: Verificar que v2, v2.1 y v2.2 fueron borradas
    cout << "\n3. Verificando que v2, v2.1 y v2.2 fueron borradas..." << endl;
    if (!versionExiste(a, "2") && !versionExiste(a, "2.1") && !versionExiste(a, "2.2")) {
        cout << "   ✓ Versión 2 y sus hijos fueron borrados correctamente" << endl;
        testsPasados++;
    } else {
        cout << "   ✗ Error: algunas versiones no fueron borradas" << endl;
        if (versionExiste(a, "2")) cout << "      - v2 todavía existe" << endl;
        if (versionExiste(a, "2.1")) cout << "      - v2.1 todavía existe" << endl;
        if (versionExiste(a, "2.2")) cout << "      - v2.2 todavía existe" << endl;
        testsFallados++;
    }
    
    // TEST 4: Verificar renumeración (v3 debe ser ahora v2)
    cout << "\n4. Verificando renumeración..." << endl;
    bool renumeracionCorrecta = true;
    
    // La antigua v3 ahora debe ser v2
    if (versionExiste(a, "2")) {
        cout << "   ✓ Version 2 existe (era v3)" << endl;
        testsPasados++;
    } else {
        cout << "   ✗ Version 2 no existe (debería ser la antigua v3)" << endl;
        renumeracionCorrecta = false;
        testsFallados++;
    }
    
    // La antigua v3.1 ahora debe ser v2.1
    if (versionExiste(a, "2.1")) {
        cout << "   ✓ Version 2.1 existe (era v3.1)" << endl;
        testsPasados++;
    } else {
        cout << "   ✗ Version 2.1 no existe (debería ser la antigua v3.1)" << endl;
        renumeracionCorrecta = false;
        testsFallados++;
    }
    
    // La antigua v4 ahora debe ser v3
    if (versionExiste(a, "3")) {
        cout << "   ✓ Version 3 existe (era v4)" << endl;
        testsPasados++;
    } else {
        cout << "   ✗ Version 3 no existe (debería ser la antigua v4)" << endl;
        renumeracionCorrecta = false;
        testsFallados++;
    }
    
    // TEST 5: Verificar que v1 sigue intacta
    cout << "\n5. Verificando que v1 no fue afectada..." << endl;
    if (versionExiste(a, "1")) {
        cout << "   ✓ Version 1 sigue existiendo" << endl;
        testsPasados++;
    } else {
        cout << "   ✗ Version 1 fue afectada incorrectamente" << endl;
        testsFallados++;
    }
    
    // TEST 6: Intentar borrar versión inexistente
    cout << "\n6. Intentando borrar versión inexistente (5)..." << endl;
    ret = BorrarVersion(a, (char*)"5");
    if (ret == ERROR) {
        cout << "   ✓ BorrarVersion retornó ERROR correctamente" << endl;
        testsPasados++;
    } else {
        cout << "   ✗ BorrarVersion debería haber retornado ERROR" << endl;
        testsFallados++;
    }
    
    // Liberar archivo
    BorrarArchivo(a);
    
    // Resumen
    cout << "\n=== RESUMEN ===" << endl;
    cout << "Tests pasados: " << testsPasados << endl;
    cout << "Tests fallados: " << testsFallados << endl;
    
    if (testsFallados == 0) {
        cout << "\n✓✓✓ TODOS LOS TESTS PASARON ✓✓✓" << endl;
        return 0;
    } else {
        cout << "\n✗✗✗ ALGUNOS TESTS FALLARON ✗✗✗" << endl;
        return 1;
    }
}
