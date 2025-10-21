// Estructuras de Datos y Algoritmos - Obligatorio 2025
// Tecnologo en Informatica FIng - DGETP - UTEC
//
// version.c
// Modulo de implementacion de version.

#include "version.h"
#include <iostream>

using namespace std;

// Crea una nueva versión
Version crearVersion(int numero) {
    Version ver = new nodo_version;
    ver->numero = numero;
    ver->primeraModificacion = nullptr;
    ver->padre = nullptr;
    ver->primerHijo = nullptr;
    ver->siguienteHermano = nullptr;
    return ver;
}

// Libera una versión individual (sin sus hermanos ni hijos)
void liberarVersion(Version& ver) {
    if (ver == nullptr) return;
    
    // Liberar todas las modificaciones de esta versión
    liberarListaModificaciones(ver->primeraModificacion);
    
    delete ver;
    ver = nullptr;
}

// Libera recursivamente todo el árbol de versiones
void liberarArbolVersiones(Version& raiz) {
    if (raiz == nullptr) return;
    
    // Liberar hermanos (siguiente en la lista horizontal)
    liberarArbolVersiones(raiz->siguienteHermano);
    
    // Liberar hijos (subversiones - para Fase 2)
    liberarArbolVersiones(raiz->primerHijo);
    
    // Liberar este nodo
    liberarVersion(raiz);
}

// Busca una versión por número (solo en primer nivel para Fase 1)
Version buscarVersion(Version raiz, int numero) {
    if (raiz == nullptr) return nullptr;
    
    if (raiz->numero == numero) return raiz;
    
    // Buscar en hermanos
    Version encontrada = buscarVersion(raiz->siguienteHermano, numero);
    if (encontrada != nullptr) return encontrada;
    
    // Buscar en hijos (para Fase 2 - ahora retornará nullptr)
    return buscarVersion(raiz->primerHijo, numero);
}
