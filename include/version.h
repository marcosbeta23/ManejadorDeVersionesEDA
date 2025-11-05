#ifndef VERSION_H
#define VERSION_H

// Estructuras de Datos y Algoritmos - Obligatorio 2025
// Tecnologo en Informatica FIng - DGETP - UTEC
//
// version.h
// Modulo de especificacion de version.

#include "mod.h"

// Representa un nodo en el árbol de versiones
struct nodo_version {
    int numero;
    Modificacion primeraModificacion;
    struct nodo_version* padre;
    struct nodo_version* primerHijo;
    struct nodo_version* siguienteHermano;
};

typedef struct nodo_version* Version;

// Crea un nodo de versión con el número especificado
Version crearVersion(int numero);

// Libera una versión individual y sus modificaciones
void liberarVersion(Version& ver);

// Libera recursivamente todo el árbol de versiones (hermanos e hijos)
void liberarArbolVersiones(Version& raiz);

// Busca recursivamente una versión por número en el árbol
Version buscarVersion(Version raiz, int numero);

#endif // VERSION_H
