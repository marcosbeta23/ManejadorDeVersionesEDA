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
    ver->primeraModificacion = NULL;
    ver->padre = NULL;
    ver->primerHijo = NULL;
    ver->siguienteHermano = NULL;
    return ver;
}

// Libera una versión individual y sus modificaciones
void liberarVersion(Version& ver) {
    if (ver == NULL) return;
    
    liberarListaModificaciones(ver->primeraModificacion);
    
    delete ver;
    ver = NULL;
}

// Libera recursivamente todo el árbol de versiones (hermanos e hijos)
void liberarArbolVersiones(Version& raiz) {
    if (raiz == NULL) return;
    
    liberarArbolVersiones(raiz->siguienteHermano);
    liberarArbolVersiones(raiz->primerHijo);
    
    liberarVersion(raiz);
}

// Busca recursivamente una versión por número en el árbol
Version buscarVersion(Version raiz, int numero) {
    if (raiz == NULL) return NULL;
    
    if (raiz->numero == numero) return raiz;
    
    Version encontrada = buscarVersion(raiz->siguienteHermano, numero);
    if (encontrada != NULL) return encontrada;
    
    return buscarVersion(raiz->primerHijo, numero);
}
