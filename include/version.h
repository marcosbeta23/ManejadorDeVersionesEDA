#ifndef VERSION_H
#define VERSION_H

// Estructuras de Datos y Algoritmos - Obligatorio 2025
// Tecnologo en Informatica FIng - DGETP - UTEC
//
// version.h
// Modulo de especificacion de version.

#include "mod.h"

// Estructura de versión
// Representa un nodo en el árbol de versiones
struct nodo_version {
    int numero;                              // Número de versión (1, 2, 3... para primer nivel)
    Modificacion primeraModificacion;        // Lista de modificaciones de esta versión
    struct nodo_version* padre;              // Padre (NULL para versiones de primer nivel)
    struct nodo_version* primerHijo;         // Primer hijo (NULL en Fase 1, se usa en Fase 2)
    struct nodo_version* siguienteHermano;   // Siguiente versión del mismo nivel
};

typedef struct nodo_version* Version;

// ============================================
// FUNCIONES PARA GESTIÓN DE VERSIONES
// ============================================

// Crea una nueva versión con el número especificado
// Parámetros:
//   - numero: número de versión a crear
// Retorna: puntero a la nueva versión inicializada
Version crearVersion(int numero);

// Libera una versión individual (sin sus hermanos ni hijos)
// Parámetros:
//   - ver: versión a liberar (se asigna NULL después)
void liberarVersion(Version& ver);

// Libera recursivamente todo el árbol de versiones
// Parámetros:
//   - raiz: raíz del árbol a liberar (se asigna NULL después)
void liberarArbolVersiones(Version& raiz);

// Busca una versión por número (solo en primer nivel para Fase 1)
// Parámetros:
//   - raiz: raíz del árbol donde buscar
//   - numero: número de versión a buscar
// Retorna: puntero a la versión encontrada o NULL si no existe
Version buscarVersion(Version raiz, int numero);

#endif // VERSION_H
