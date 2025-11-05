#ifndef ARCHIVO_H
#define ARCHIVO_H

// Estructuras de Datos y Algoritmos - Obligatorio 2025
// Tecnologo en Informatica FIng - DGETP - UTEC
//
// archivo.h
// Modulo de especificacion de archivo.

#include "definiciones.h"
#include "version.h"

// Representa un archivo con su nombre y árbol de versiones
struct nodo_archivo {
    char* nombre;
    Version primeraVersion;
};

typedef struct nodo_archivo * Archivo;

// Crea un archivo vacío con el nombre especificado
Archivo CrearArchivo(char * nombre);

// Libera toda la memoria del archivo y asigna NULL al puntero
TipoRet BorrarArchivo(Archivo &a);

// Crea una nueva versión validando que el padre existe y no haya huecos
TipoRet CrearVersion(Archivo &a, char * version, char * error);

// Elimina una versión y sus subversiones, renumerando las siguientes
TipoRet BorrarVersion(Archivo &a, char * version);

// Muestra el árbol de versiones con indentación jerárquica
TipoRet MostrarVersiones(Archivo a);

// Inserta una línea de texto en la posición especificada (1 a n+1)
TipoRet InsertarLinea(Archivo &a, char * version, char * linea, unsigned int nroLinea, char * error);

// Elimina una línea de texto en la posición especificada (1 a n)
TipoRet BorrarLinea(Archivo &a, char * version, unsigned int nroLinea, char * error);

// Muestra el texto completo de una versión incluyendo cambios de ancestros
TipoRet MostrarTexto(Archivo a, char * version);

// Muestra solo los cambios realizados en esta versión (sin ancestros)
TipoRet MostrarCambios(Archivo a, char * version);

// Compara el texto de dos versiones y retorna si son iguales
TipoRet Iguales(Archivo a, char * version1, char * version2, bool &iguales);

// Crea una versión independiente al final del primer nivel con todos los cambios acumulados
TipoRet VersionIndependiente(Archivo &a, char * version);

#endif
