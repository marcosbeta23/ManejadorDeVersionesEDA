#ifndef UTILS_H
#define UTILS_H

// Estructuras de Datos y Algoritmos - Obligatorio 2025
// Tecnologo en Informatica FIng - DGETP - UTEC
//
// utils.h
// Modulo de especificacion de utilidades.

#include "version.h"

// ============================================
// UTILIDADES DE STRINGS
// ============================================

// Copia un string allocando memoria nueva
// Parámetros:
//   - str: string a copiar
// Retorna: puntero a la copia del string
char* copiarString(const char* str);

// Libera un string y pone el puntero en NULL
// Parámetros:
//   - str: string a liberar (se asigna NULL después)
void liberarString(char*& str);


// ============================================
// UTILIDADES DE RECONSTRUCCIÓN DE TEXTO
// ============================================

// Estructura temporal para lista de líneas (usada en MostrarTexto)
struct nodo_linea {
    char* texto;                    // Texto de la línea
    unsigned int numero;            // Número de línea (1, 2, 3...)
    struct nodo_linea* siguiente;   // Siguiente línea
};

typedef struct nodo_linea* ListaLineas;

// Crea una lista de líneas vacía
// Retorna: NULL (lista vacía)
ListaLineas crearListaLineas();

// Inserta una línea en la posición especificada
// Parámetros:
//   - lista: lista de líneas
//   - nroLinea: posición donde insertar (1 a n+1)
//   - texto: texto de la línea a insertar
void insertarLineaEnLista(ListaLineas& lista, unsigned int nroLinea, const char* texto);

// Borra la línea en la posición especificada
// Parámetros:
//   - lista: lista de líneas
//   - nroLinea: posición de la línea a borrar (1 a n)
void borrarLineaEnLista(ListaLineas& lista, unsigned int nroLinea);

// Imprime todas las líneas numeradas
// Parámetros:
//   - lista: lista de líneas a imprimir
void imprimirListaLineas(ListaLineas lista);

// Libera toda la lista de líneas
// Parámetros:
//   - lista: lista a liberar (se asigna NULL después)
void liberarListaLineas(ListaLineas& lista);

// Cuenta cuántas líneas tiene la lista
// Parámetros:
//   - lista: lista de líneas
// Retorna: número de líneas
unsigned int contarLineas(ListaLineas lista);


// ============================================
// UTILIDADES DE VERSIONES
// ============================================

// Aplica todas las modificaciones a un texto (reconstrucción)
// Parámetros:
//   - texto: lista de líneas donde aplicar las modificaciones
//   - mods: lista de modificaciones a aplicar
void aplicarModificaciones(ListaLineas& texto, Modificacion mods);

// Busca una versión por número en lista de hermanos
// Parámetros:
//   - lista: primera versión de la lista de hermanos
//   - numero: número de versión a buscar
// Retorna: puntero a la versión encontrada o NULL
Version buscarVersionEnLista(Version lista, int numero);

// Agrega una modificación al final de la lista de modificaciones de una versión
// Parámetros:
//   - ver: versión donde agregar la modificación
//   - mod: modificación a agregar
void agregarModificacion(Version ver, Modificacion mod);


// ============================================
// PARSER DE VERSIONES JERÁRQUICAS (FASE 2)
// ============================================

// Parsea un string de versión jerárquica (ej: "1.2.3.4")
// Convierte el string en un array de números para navegar el árbol
// Parámetros:
//   - version: string de versión a parsear (ej: "1", "2.3", "1.2.4.5")
//   - longitud: puntero donde guardar la cantidad de niveles
// Retorna: array dinámico de enteros con los números, o NULL si es inválido
// Nota: El caller debe liberar el array con liberarArrayVersion()
// Ejemplos:
//   "1" → [1], longitud = 1
//   "2.3" → [2, 3], longitud = 2
//   "1.2.4" → [1, 2, 4], longitud = 3
int* parsearVersion(const char* version, int* longitud);

// Libera el array de versión creado por parsearVersion
// Parámetros:
//   - array: array a liberar (se asigna NULL después)
void liberarArrayVersion(int*& array);


// ============================================
// NAVEGACIÓN DEL ÁRBOL N-ARIO (FASE 2)
// ============================================

// Busca un hijo específico por número en la lista de hijos de un padre
// Parámetros:
//   - padre: nodo padre donde buscar (puede ser NULL)
//   - numero: número del hijo a buscar
// Retorna: puntero al hijo encontrado o NULL si no existe
// Nota: Busca en la lista de hermanos comenzando por padre->primerHijo
Version buscarHijo(Version padre, int numero);

// Navega el árbol siguiendo una secuencia de números
// Parámetros:
//   - primeraVersion: raíz del árbol (lista de versiones de primer nivel)
//   - secuencia: array de números que indica el camino (ej: [2, 3, 1])
//   - longitud: cantidad de números en la secuencia
// Retorna: puntero a la versión encontrada o NULL si no existe
// Ejemplos:
//   NavegarAVersion(a, [1], 1) → versión 1
//   NavegarAVersion(a, [2, 3], 2) → versión 2.3
//   NavegarAVersion(a, [1, 2, 3, 4], 4) → versión 1.2.3.4
//   NavegarAVersion(a, [5, 1], 2) → NULL (no existe)
Version navegarAVersion(Version primeraVersion, int* secuencia, int longitud);

#endif // UTILS_H
