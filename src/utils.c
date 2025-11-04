// Estructuras de Datos y Algoritmos - Obligatorio 2025
// Tecnologo en Informatica FIng - DGETP - UTEC
//
// utils.c
// Modulo de implementacion de utilidades.

#include "utils.h"
#include <cstring>
#include <iostream>

using namespace std;

// ============================================
// UTILIDADES DE STRINGS
// ============================================

char* copiarString(const char* str) {
    if (str == nullptr) return nullptr;
    
    char* copia = new char[strlen(str) + 1];
    strcpy(copia, str);
    return copia;
}

void liberarString(char*& str) {
    if (str != nullptr) {
        delete[] str;
        str = nullptr;
    }
}

// ============================================
// UTILIDADES DE RECONSTRUCCIÓN DE TEXTO
// ============================================

ListaLineas crearListaLineas() {
    return nullptr;
}

void insertarLineaEnLista(ListaLineas& lista, unsigned int nroLinea, const char* texto) {
    nodo_linea* nuevo = new nodo_linea;
    nuevo->texto = copiarString(texto);
    nuevo->numero = nroLinea;
    nuevo->siguiente = nullptr;
    
    if (lista == nullptr || nroLinea == 1) {
        nuevo->siguiente = lista;
        lista = nuevo;
        
        nodo_linea* actual = nuevo->siguiente;
        unsigned int num = nroLinea + 1;
        while (actual != nullptr) {
            actual->numero = num++;
            actual = actual->siguiente;
        }
        return;
    }
    
    nodo_linea* anterior = lista;
    unsigned int pos = 1;
    
    while (anterior->siguiente != nullptr && pos < nroLinea - 1) {
        anterior = anterior->siguiente;
        pos++;
    }
    
    nuevo->siguiente = anterior->siguiente;
    anterior->siguiente = nuevo;
    
    nodo_linea* actual = nuevo->siguiente;
    unsigned int num = nroLinea + 1;
    while (actual != nullptr) {
        actual->numero = num++;
        actual = actual->siguiente;
    }
}

void borrarLineaEnLista(ListaLineas& lista, unsigned int nroLinea) {
    if (lista == nullptr) return;
    
    if (nroLinea == 1) {
        nodo_linea* temp = lista;
        lista = lista->siguiente;
        liberarString(temp->texto);
        delete temp;
        
        nodo_linea* actual = lista;
        unsigned int num = 1;
        while (actual != nullptr) {
            actual->numero = num++;
            actual = actual->siguiente;
        }
        return;
    }
    
    nodo_linea* anterior = lista;
    unsigned int pos = 1;
    
    while (anterior->siguiente != nullptr && pos < nroLinea - 1) {
        anterior = anterior->siguiente;
        pos++;
    }
    
    if (anterior->siguiente == nullptr) return;
    
    nodo_linea* temp = anterior->siguiente;
    anterior->siguiente = temp->siguiente;
    liberarString(temp->texto);
    delete temp;
    
    nodo_linea* actual = anterior->siguiente;
    unsigned int num = nroLinea;
    while (actual != nullptr) {
        actual->numero = num++;
        actual = actual->siguiente;
    }
}

void imprimirListaLineas(ListaLineas lista) {
    nodo_linea* actual = lista;
    while (actual != nullptr) {
        cout << actual->numero << ". " << actual->texto << endl;
        actual = actual->siguiente;
    }
}

void liberarListaLineas(ListaLineas& lista) {
    while (lista != nullptr) {
        nodo_linea* temp = lista;
        lista = lista->siguiente;
        liberarString(temp->texto);
        delete temp;
    }
}

unsigned int contarLineas(ListaLineas lista) {
    unsigned int count = 0;
    nodo_linea* actual = lista;
    while (actual != nullptr) {
        count++;
        actual = actual->siguiente;
    }
    return count;
}

// ============================================
// UTILIDADES DE VERSIONES
// ============================================

void aplicarModificaciones(ListaLineas& texto, Modificacion mods) {
    Modificacion actual = mods;
    
    while (actual != nullptr) {
        if (actual->tipo == INSERCION) {
            insertarLineaEnLista(texto, actual->nroLinea, actual->textoLinea);
        } else if (actual->tipo == BORRADO) {
            borrarLineaEnLista(texto, actual->nroLinea);
        }
        actual = actual->siguiente;
    }
}

Version buscarVersionEnLista(Version lista, int numero) {
    Version actual = lista;
    while (actual != nullptr) {
        if (actual->numero == numero) return actual;
        actual = actual->siguienteHermano;
    }
    return nullptr;
}

// Agrega una modificación al final de la lista de modificaciones de una versión
void agregarModificacion(Version ver, Modificacion mod) {
    if (ver == nullptr || mod == nullptr) return;
    
    // Si la versión no tiene modificaciones, esta es la primera
    if (ver->primeraModificacion == nullptr) {
        ver->primeraModificacion = mod;
        return;
    }
    
    // Buscar el último nodo de la lista de modificaciones
    Modificacion actual = ver->primeraModificacion;
    while (actual->siguiente != nullptr) {
        actual = actual->siguiente;
    }
    
    // Agregar al final
    actual->siguiente = mod;
}


// ============================================
// PARSER DE VERSIONES JERÁRQUICAS (FASE 2)
// ============================================

int* parsearVersion(const char* version, int* longitud) {
    if (version == nullptr || longitud == nullptr) return nullptr;
    
    *longitud = 0;
    
    // Validar que el string no esté vacío
    if (strlen(version) == 0) return nullptr;
    
    // Validar que no empiece ni termine con punto
    if (version[0] == '.' || version[strlen(version) - 1] == '.') {
        return nullptr;
    }
    
    // Hacer una copia del string para no modificar el original
    char* copia = copiarString(version);
    
    // Contar cuántos niveles hay (cantidad de números separados por puntos)
    int niveles = 1;
    for (size_t i = 0; i < strlen(copia); i++) {
        if (copia[i] == '.') {
            niveles++;
            // Validar que no haya puntos consecutivos
            if (i > 0 && copia[i-1] == '.') {
                liberarString(copia);
                return nullptr;
            }
        } else if (copia[i] < '0' || copia[i] > '9') {
            // Validar que solo haya dígitos y puntos
            liberarString(copia);
            return nullptr;
        }
    }
    
    // Alocar array para los números
    int* numeros = new int[niveles];
    
    // Parsear los números
    char* token = strtok(copia, ".");
    int indice = 0;
    
    while (token != nullptr && indice < niveles) {
        // Validar que el token no esté vacío
        if (strlen(token) == 0) {
            delete[] numeros;
            liberarString(copia);
            return nullptr;
        }
        
        // Convertir a entero
        int numero = atoi(token);
        
        // Validar que sea un número válido (mayor que 0)
        if (numero <= 0) {
            delete[] numeros;
            liberarString(copia);
            return nullptr;
        }
        
        numeros[indice] = numero;
        indice++;
        token = strtok(nullptr, ".");
    }
    
    liberarString(copia);
    
    // Verificar que se parsearon todos los niveles esperados
    if (indice != niveles) {
        delete[] numeros;
        return nullptr;
    }
    
    *longitud = niveles;
    return numeros;
}

void liberarArrayVersion(int*& array) {
    if (array != nullptr) {
        delete[] array;
        array = nullptr;
    }
}


// ============================================
// NAVEGACIÓN DEL ÁRBOL N-ARIO (FASE 2)
// ============================================

Version buscarHijo(Version padre, int numero) {
    // Si el padre es NULL, no tiene hijos
    if (padre == nullptr) return nullptr;
    
    // Empezar desde el primer hijo
    Version actual = padre->primerHijo;
    
    // Recorrer la lista de hermanos buscando el número
    while (actual != nullptr) {
        if (actual->numero == numero) {
            return actual;
        }
        actual = actual->siguienteHermano;
    }
    
    // No se encontró el hijo con ese número
    return nullptr;
}

Version navegarAVersion(Version primeraVersion, int* secuencia, int longitud) {
    // Validaciones básicas
    if (secuencia == nullptr || longitud <= 0) return nullptr;
    
    // Empezar desde la lista de versiones de primer nivel
    Version actual = primeraVersion;
    
    // Buscar el primer número en la lista de primer nivel
    actual = buscarVersionEnLista(actual, secuencia[0]);
    
    // Si no existe la versión de primer nivel, retornar NULL
    if (actual == nullptr) return nullptr;
    
    // Si solo buscábamos un nivel, ya terminamos
    if (longitud == 1) return actual;
    
    // Navegar por los niveles restantes
    for (int i = 1; i < longitud; i++) {
        // Buscar el siguiente número entre los hijos del nodo actual
        actual = buscarHijo(actual, secuencia[i]);
        
        // Si no existe ese hijo, retornar NULL
        if (actual == nullptr) return nullptr;
    }
    
    // Retornar el nodo encontrado
    return actual;
}

