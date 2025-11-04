// Test de debugging para desplazamiento con hijos
#include <iostream>
#include "../include/archivo.h"
#include "../include/utils.h"
#include "../include/version.h"

using namespace std;

void mostrarEstructura(Version v, int nivel = 0) {
    if (v == nullptr) return;
    
    for (int i = 0; i < nivel; i++) cout << "  ";
    cout << v->numero << endl;
    
    mostrarEstructura(v->primerHijo, nivel + 1);
    mostrarEstructura(v->siguienteHermano, nivel);
}

int main() {
    Archivo a = CrearArchivo((char*)"debug.txt");
    char error[100];
    
    cout << "Creando versiones iniciales..." << endl;
    CrearVersion(a, (char*)"1", error);
    CrearVersion(a, (char*)"2", error);
    CrearVersion(a, (char*)"2.1", error);
    CrearVersion(a, (char*)"2.2", error);
    CrearVersion(a, (char*)"3", error);
    
    cout << "\nESTRUCTURA ANTES del desplazamiento:" << endl;
    mostrarEstructura(a->primeraVersion);
    
    cout << "\nCreando nueva versión '2' (debería desplazar)..." << endl;
    TipoRet ret = CrearVersion(a, (char*)"2", error);
    
    if (ret == OK) {
        cout << "✓ Versión creada OK" << endl;
    } else {
        cout << "✗ ERROR: " << error << endl;
    }
    
    cout << "\nESTRUCTURA DESPUÉS del desplazamiento:" << endl;
    mostrarEstructura(a->primeraVersion);
    
    cout << "\nVerificando versiones..." << endl;
    
    int seq31[] = {3, 1};
    Version v31 = navegarAVersion(a->primeraVersion, seq31, 2);
    cout << "Version 3.1 " << (v31 ? "EXISTE" : "NO EXISTE") << endl;
    
    int seq32[] = {3, 2};
    Version v32 = navegarAVersion(a->primeraVersion, seq32, 2);
    cout << "Version 3.2 " << (v32 ? "EXISTE" : "NO EXISTE") << endl;
    
    BorrarArchivo(a);
    return 0;
}
