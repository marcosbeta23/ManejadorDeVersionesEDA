#include <iostream>
#include <cstdio>
#include <cstring>

extern "C" {
    #include "archivo.h"
    #include "definiciones.h"
}

using namespace std;

int main() {
    cout << "=== TEST BORRARVERSION COMPLETO ===" << endl;
    
    // Crear archivo
    Archivo a = CrearArchivo((char*)"test");
    char error[100];
    
    // Crear estructura:
    // 1
    // 2 -> 2.1, 2.2
    // 3 -> 3.1
    // 4
    
    cout << "\n1. Creando versiones..." << endl;
    CrearVersion(a, (char*)"1", error);
    CrearVersion(a, (char*)"2", error);
    CrearVersion(a, (char*)"2.1", error);
    CrearVersion(a, (char*)"2.2", error);
    CrearVersion(a, (char*)"3", error);
    CrearVersion(a, (char*)"3.1", error);
    CrearVersion(a, (char*)"4", error);
    
    cout << "\nARBOL INICIAL:" << endl;
    MostrarVersiones(a);
    
    // TEST 1: Borrar versión con hijos (2)
    cout << "\n2. TEST 1: Borrando version 2 (con hijos 2.1 y 2.2)..." << endl;
    TipoRet ret = BorrarVersion(a, (char*)"2");
    if (ret == OK) {
        cout << "✓ Borrado exitoso" << endl;
    } else {
        cout << "✗ Error al borrar: " << ret << endl;
    }
    
    cout << "\nARBOL DESPUES de borrar v2:" << endl;
    cout << "Deberia mostrar: 1, 2 (era 3 con hijo 2.1 que era 3.1), 3 (era 4)" << endl;
    MostrarVersiones(a);
    
    // TEST 2: Borrar versión de primer nivel simple
    cout << "\n3. TEST 2: Borrando version 3 (antes era 4, sin hijos)..." << endl;
    ret = BorrarVersion(a, (char*)"3");
    if (ret == OK) {
        cout << "✓ Borrado exitoso" << endl;
    } else {
        cout << "✗ Error al borrar: " << ret << endl;
    }
    
    cout << "\nARBOL DESPUES de borrar v3:" << endl;
    cout << "Deberia mostrar: 1, 2 (con hijo 2.1)" << endl;
    MostrarVersiones(a);
    
    // TEST 3: Borrar versión profunda
    cout << "\n4. TEST 3: Borrando version 2.1 (subversion)..." << endl;
    ret = BorrarVersion(a, (char*)"2.1");
    if (ret == OK) {
        cout << "✓ Borrado exitoso" << endl;
    } else {
        cout << "✗ Error al borrar: " << ret << endl;
    }
    
    cout << "\nARBOL DESPUES de borrar v2.1:" << endl;
    cout << "Deberia mostrar: 1, 2 (sin hijos)" << endl;
    MostrarVersiones(a);
    
    // TEST 4: Borrar otra versión para dejar solo 1
    cout << "\n5. TEST 4: Borrando version 2..." << endl;
    ret = BorrarVersion(a, (char*)"2");
    if (ret == OK) {
        cout << "✓ Borrado exitoso" << endl;
    } else {
        cout << "✗ Error al borrar: " << ret << endl;
    }
    
    cout << "\nARBOL DESPUES de borrar v2:" << endl;
    cout << "Deberia mostrar: solo 1" << endl;
    MostrarVersiones(a);
    
    // TEST 5: Borrar única versión "1" - archivo queda vacío
    cout << "\n6. TEST 5: Borrando unica version 1 (archivo queda vacio)..." << endl;
    ret = BorrarVersion(a, (char*)"1");
    if (ret == OK) {
        cout << "✓ Borrado exitoso - archivo vacio" << endl;
    } else {
        cout << "✗ Error al borrar: " << ret << endl;
    }
    
    cout << "\nARBOL DESPUES de borrar v1:" << endl;
    cout << "Deberia mostrar: (vacio)" << endl;
    MostrarVersiones(a);
    
    // TEST 6: Error - intentar borrar versión inexistente
    cout << "\n7. TEST 6: Intentar borrar version inexistente (5)..." << endl;
    ret = BorrarVersion(a, (char*)"5");
    if (ret == ERROR) {
        cout << "✓ Error detectado correctamente" << endl;
    } else {
        cout << "✗ Deberia haber dado error pero retorno: " << ret << endl;
    }
    
    // Liberar archivo
    BorrarArchivo(a);
    
    cout << "\n=== TODOS LOS TESTS COMPLETADOS ===" << endl;
    cout << "Ahora ejecutar con valgrind para verificar memoria" << endl;
    return 0;
}
