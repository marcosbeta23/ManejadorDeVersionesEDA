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
    // Crear nuevo nodo
    nodo_linea* nuevo = new nodo_linea;
    nuevo->texto = copiarString(texto);
    nuevo->numero = nroLinea;
    nuevo->siguiente = nullptr;
    
    // Caso: lista vacía o insertar al inicio
    if (lista == nullptr || nroLinea == 1) {
        nuevo->siguiente = lista;
        lista = nuevo;
        
        // Renumerar líneas siguientes
        nodo_linea* actual = nuevo->siguiente;
        unsigned int num = nroLinea + 1;
        while (actual != nullptr) {
            actual->numero = num++;
            actual = actual->siguiente;
        }
        return;
    }
    
    // Buscar posición de inserción
    nodo_linea* anterior = lista;
    unsigned int pos = 1;
    
    while (anterior->siguiente != nullptr && pos < nroLinea - 1) {
        anterior = anterior->siguiente;
        pos++;
    }
    
    // Insertar
    nuevo->siguiente = anterior->siguiente;
    anterior->siguiente = nuevo;
    
    // Renumerar líneas siguientes
    nodo_linea* actual = nuevo->siguiente;
    unsigned int num = nroLinea + 1;
    while (actual != nullptr) {
        actual->numero = num++;
        actual = actual->siguiente;
    }
}

void borrarLineaEnLista(ListaLineas& lista, unsigned int nroLinea) {
    if (lista == nullptr) return;
    
    // Caso: borrar primera línea
    if (nroLinea == 1) {
        nodo_linea* temp = lista;
        lista = lista->siguiente;
        liberarString(temp->texto);
        delete temp;
        
        // Renumerar líneas restantes
        nodo_linea* actual = lista;
        unsigned int num = 1;
        while (actual != nullptr) {
            actual->numero = num++;
            actual = actual->siguiente;
        }
        return;
    }
    
    // Buscar línea anterior a la que se va a borrar
    nodo_linea* anterior = lista;
    unsigned int pos = 1;
    
    while (anterior->siguiente != nullptr && pos < nroLinea - 1) {
        anterior = anterior->siguiente;
        pos++;
    }
    
    if (anterior->siguiente == nullptr) return;  // Línea no existe
    
    // Borrar
    nodo_linea* temp = anterior->siguiente;
    anterior->siguiente = temp->siguiente;
    liberarString(temp->texto);
    delete temp;
    
    // Renumerar líneas restantes
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
