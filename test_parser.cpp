// Test del Parser de Versiones Jerárquicas
// Fase 2 - EDA Obligatorio 2025

#include "include/utils.h"
#include <iostream>
#include <cstring>

using namespace std;

// Colores para output
#define COLOR_VERDE "\033[1;32m"
#define COLOR_ROJO "\033[1;31m"
#define COLOR_AMARILLO "\033[1;33m"
#define COLOR_AZUL "\033[1;34m"
#define COLOR_RESET "\033[0m"

void testParser(const char* version, bool deberiaFallar = false) {
    cout << COLOR_AZUL << "Test: parsearVersion(\"" << version << "\")" << COLOR_RESET << endl;
    
    int longitud = 0;
    int* resultado = parsearVersion(version, &longitud);
    
    if (resultado == nullptr) {
        if (deberiaFallar) {
            cout << COLOR_VERDE << "  ✓ Correctamente rechazado (caso inválido)" << COLOR_RESET << endl;
        } else {
            cout << COLOR_ROJO << "  ✗ ERROR: Retornó NULL cuando debería ser válido" << COLOR_RESET << endl;
        }
    } else {
        if (deberiaFallar) {
            cout << COLOR_ROJO << "  ✗ ERROR: Aceptó un caso inválido" << COLOR_RESET << endl;
        } else {
            cout << COLOR_VERDE << "  ✓ Parseado exitosamente" << COLOR_RESET << endl;
            cout << "    Longitud: " << longitud << endl;
            cout << "    Array: [";
            for (int i = 0; i < longitud; i++) {
                cout << resultado[i];
                if (i < longitud - 1) cout << ", ";
            }
            cout << "]" << endl;
        }
        liberarArrayVersion(resultado);
    }
    cout << endl;
}

int main() {
    cout << COLOR_AMARILLO << "================================================" << COLOR_RESET << endl;
    cout << COLOR_AMARILLO << "TESTS DEL PARSER DE VERSIONES JERÁRQUICAS" << COLOR_RESET << endl;
    cout << COLOR_AMARILLO << "================================================" << COLOR_RESET << endl << endl;
    
    cout << COLOR_AMARILLO << "--- CASOS VÁLIDOS ---" << COLOR_RESET << endl << endl;
    
    // Casos válidos: versiones simples
    testParser("1");
    testParser("2");
    testParser("3");
    testParser("10");
    testParser("99");
    
    // Casos válidos: dos niveles
    testParser("1.1");
    testParser("2.3");
    testParser("1.10");
    testParser("10.5");
    
    // Casos válidos: tres niveles
    testParser("1.2.3");
    testParser("2.1.4");
    testParser("10.20.30");
    
    // Casos válidos: cuatro niveles
    testParser("2.1.3.4");
    testParser("1.2.3.4");
    testParser("5.10.15.20");
    
    // Casos válidos: cinco o más niveles
    testParser("1.2.3.4.5");
    testParser("1.2.3.4.5.6.7.8");
    
    cout << COLOR_AMARILLO << "--- CASOS INVÁLIDOS ---" << COLOR_RESET << endl << endl;
    
    // Casos inválidos: strings vacíos o malformados
    testParser("", true);
    testParser(".", true);
    testParser("..", true);
    testParser("...", true);
    
    // Casos inválidos: punto al inicio o al final
    testParser("1.", true);
    testParser(".1", true);
    testParser("1.2.", true);
    testParser(".1.2", true);
    
    // Casos inválidos: puntos consecutivos
    testParser("1..2", true);
    testParser("1.2..3", true);
    testParser("1..2..3", true);
    
    // Casos inválidos: números cero o negativos
    testParser("0", true);
    testParser("1.0", true);
    testParser("0.1", true);
    testParser("1.2.0", true);
    
    // Casos inválidos: caracteres no numéricos
    testParser("a", true);
    testParser("1.a", true);
    testParser("1.2.b", true);
    testParser("1x2", true);
    testParser("1-2", true);
    testParser("1,2", true);
    testParser("1 2", true);
    
    cout << COLOR_AMARILLO << "================================================" << COLOR_RESET << endl;
    cout << COLOR_AMARILLO << "TESTS COMPLETADOS" << COLOR_RESET << endl;
    cout << COLOR_AMARILLO << "================================================" << COLOR_RESET << endl;
    
    return 0;
}
