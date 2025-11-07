#ifndef UTILS_H
#define UTILS_H

// Estructuras de Datos y Algoritmos - Obligatorio 2025
// Tecnologo en Informatica FIng - DGETP - UTEC
//
// utils.h
// Modulo de especificacion de utilidades.

#include "version.h"

// Crea una copia dinámica del string
char* copiarString(const char* str);

// Libera un string y asigna NULL al puntero
void liberarString(char*& str);

// Estructura para reconstruir el texto aplicando modificaciones
struct nodo_linea {
    char* texto;
    unsigned int numero;
    struct nodo_linea* siguiente;
};

typedef struct nodo_linea* ListaLineas;

// Crea una lista de líneas vacía
ListaLineas crearListaLineas();

// Inserta una línea en la posición especificada
void insertarLineaEnLista(ListaLineas& lista, unsigned int nroLinea, const char* texto);

// Elimina la línea en la posición especificada
void borrarLineaEnLista(ListaLineas& lista, unsigned int nroLinea);

// Imprime todas las líneas numeradas
void imprimirListaLineas(ListaLineas lista);

// Libera toda la lista de líneas
void liberarListaLineas(ListaLineas& lista);

// Retorna la cantidad de líneas de la lista
unsigned int contarLineas(ListaLineas lista);


// Aplica todas las modificaciones a un texto
void aplicarModificaciones(ListaLineas& texto, Modificacion mods);

// Busca una versión por número en lista de hermanos
Version buscarVersionEnLista(Version lista, int numero);

// Agrega una modificación al final de la lista
void agregarModificacion(Version ver, Modificacion mod);

// Parsea un string de versión (ej: "1.2.3") y retorna un array de enteros
int* parsearVersion(const char* version, int* longitud);

// Libera el array creado por parsearVersion
void liberarArrayVersion(int*& array);


// Busca un hijo específico por número en la lista de hijos
Version buscarHijo(Version padre, int numero);

// Navega el árbol siguiendo una secuencia de números
Version navegarAVersion(Version primeraVersion, int* secuencia, int longitud);

// Construye un camino desde la versión hasta la raíz (en orden inverso)
void obtenerCaminoAncestros(Version v, Version* camino, int* longitud);


// Valida que el padre de una versión existe
bool validarPadreExiste(Version primeraVersion, int* secuencia, int longitud);

// Valida que no hay huecos entre versiones hermanas
bool validarSinHuecos(Version padre, Version primeraVersion, int numeroNuevo);


// Renumera recursivamente un subárbol completo sumando delta
void renumerarSubarbol(Version raiz, int delta);

// Desplaza y renumera versiones hermanas desde una posición
void desplazarYRenumerar(Version padre, Version& primeraVersion, int numeroInicio, int delta);

#endif // UTILS_H
