#include <iostream>
#include "../include/definiciones.h"
#include "../include/version.h"
#include "../include/utils.h"

using namespace std;

void mostrarArbol(Version v, int nivel = 0) {
    if (v == NULL) return;
    
    for (int i = 0; i < nivel; i++) cout << "  ";
    cout << v->numero << endl;
    
    mostrarArbol(v->primerHijo, nivel + 1);
    mostrarArbol(v->siguienteHermano, nivel);
}

int main() {
    cout << "=== TEST RENUMERACION INVERSA ===" << endl;
    
    // Crear estructura de árbol manualmente
    // 1
    // 2 -> 2.1
    //      2.2
    // 3 -> 3.1
    // 4
    
    Version v1 = crearVersion(1);
    Version v2 = crearVersion(2);
    Version v2_1 = crearVersion(1);
    Version v2_2 = crearVersion(2);
    Version v3 = crearVersion(3);
    Version v3_1 = crearVersion(1);
    Version v4 = crearVersion(4);
    
    // Establecer relaciones
    v1->siguienteHermano = v2;
    v2->siguienteHermano = v3;
    v3->siguienteHermano = v4;
    
    v2->primerHijo = v2_1;
    v2_1->padre = v2;
    v2_1->siguienteHermano = v2_2;
    v2_2->padre = v2;
    
    v3->primerHijo = v3_1;
    v3_1->padre = v3;
    
    cout << "\nARBOL ANTES de borrar version 2:" << endl;
    mostrarArbol(v1);
    
    // Simular borrado de v2:
    // 1. Guardar referencia al siguiente hermano de v2
    Version hermanoSiguiente = v2->siguienteHermano;
    
    // 2. Actualizar puntero del hermano anterior (v1)
    v1->siguienteHermano = v2->siguienteHermano;
    
    // 3. IMPORTANTE: Antes de liberar, debemos desconectar v2 de sus hermanos
    // porque liberarArbolVersiones libera hermanos también
    Version hijosV2 = v2->primerHijo;
    v2->primerHijo = NULL;
    v2->siguienteHermano = NULL;
    
    cout << "\n[Liberando version 2...]" << endl;
    liberarVersion(v2); // Liberar solo el nodo v2
    
    cout << "[Liberando hijos de v2 (2.1, 2.2)...]" << endl;
    liberarArbolVersiones(hijosV2); // Liberar los hijos
    
    // 4. Renumerar hermanos posteriores
    cout << "[Renumerando hermanos posteriores con delta -1...]" << endl;
    renumerarHermanosPosteriores(hermanoSiguiente, -1);
    
    cout << "\nARBOL DESPUES de borrar version 2 (3->2, 3.1->2.1, 4->3):" << endl;
    mostrarArbol(v1);
    
    // Verificación
    cout << "\n=== VERIFICACION ===" << endl;
    Version actual = v1->siguienteHermano; // Debería ser el antiguo v3, ahora 2
    if (actual && actual->numero == 2) {
        cout << "✓ Version 3 renumerada a 2" << endl;
        if (actual->primerHijo && actual->primerHijo->numero == 1) {
            cout << "✓ Hijo 3.1 renumerado a 2.1" << endl;
        } else {
            cout << "✗ ERROR: Hijo no renumerado correctamente" << endl;
        }
        
        actual = actual->siguienteHermano;
        if (actual && actual->numero == 3) {
            cout << "✓ Version 4 renumerada a 3" << endl;
        } else {
            cout << "✗ ERROR: Version 4 no renumerada correctamente" << endl;
        }
    } else {
        cout << "✗ ERROR: Renumeracion fallida" << endl;
    }
    
    // Liberar memoria restante
    liberarArbolVersiones(v1);
    
    cout << "\n=== TEST COMPLETADO ===" << endl;
    return 0;
}
