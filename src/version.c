// Estructuras de Datos y Algoritmos - Obligatorio 2025
// Tecnologo en Informatica FIng - DGETP - UTEC
//
// version.c
// Modulo de implementacion de version.

#include "version.h"
#include <iostream>

using namespace std;

// Crea un nodo de versión con el número especificado
Version crearVersion(int numero) {
    Version ver = new nodo_version;
    ver->numero = numero;
    ver->primeraModificacion = nullptr;
    ver->padre = nullptr;
    ver->primerHijo = nullptr;
    ver->siguienteHermano = nullptr;
    return ver;
}

// Libera una versión individual y sus modificaciones
void liberarVersion(Version& ver) {
    if (ver == nullptr) return;
    
    liberarListaModificaciones(ver->primeraModificacion);
    
    delete ver;
    ver = nullptr;
}

// Libera recursivamente todo el árbol de versiones (hermanos e hijos)
void liberarArbolVersiones(Version& raiz) {
    if (raiz == nullptr) return;
    
    liberarArbolVersiones(raiz->siguienteHermano);
    liberarArbolVersiones(raiz->primerHijo);
    
    liberarVersion(raiz);
}

// Busca recursivamente una versión por número en el árbol
Version buscarVersion(Version raiz, int numero) {
    if (raiz == nullptr) return nullptr;
    
    if (raiz->numero == numero) return raiz;
    
    Version encontrada = buscarVersion(raiz->siguienteHermano, numero);
    if (encontrada != nullptr) return encontrada;
    
    return buscarVersion(raiz->primerHijo, numero);
}
