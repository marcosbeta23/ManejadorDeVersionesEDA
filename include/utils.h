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

#endif // UTILS_H
