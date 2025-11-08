// Estructuras de Datos y Algoritmos - Obligatorio 2025
// Tecnologo en Informatica FIng - DGETP - UTEC
//
// utils.c
// Modulo de implementacion de utilidades.

#include "utils.h"
#include <cstring>
#include <iostream>

using namespace std;

// Crea una copia dinámica del string
char* copiarString(const char* str) {
    if (str == nullptr) return nullptr;
    
    char* copia = new char[strlen(str) + 1];
    strcpy(copia, str);
    return copia;
}

// Libera memoria de un string dinámico
void liberarString(char*& str) {
    if (str != nullptr) {
        delete[] str;
        str = nullptr;
    }
}

// Crea una lista de líneas vacía
ListaLineas crearListaLineas() {
    return nullptr;
}

// Inserta una línea en la posición especificada y renumera las siguientes
void insertarLineaEnLista(ListaLineas& lista, unsigned int nroLinea, const char* texto) {
    nodo_linea* nuevo = new nodo_linea;
    nuevo->texto = copiarString(texto);
    nuevo->numero = nroLinea;
    nuevo->siguiente = nullptr;
    
    if (lista == nullptr || nroLinea == 1) {
        nuevo->siguiente = lista;
        lista = nuevo;
        
        // Renumerar las que siguen
        nodo_linea* actual = nuevo->siguiente;
        unsigned int num = nroLinea + 1;
        while (actual != nullptr) {
            actual->numero = num++;
            actual = actual->siguiente;
        }
        return;
    }
    
    nodo_linea* anterior = lista;
    unsigned int pos = 1;
    
    while (anterior->siguiente != nullptr && pos < nroLinea - 1) {
        anterior = anterior->siguiente;
        pos++;
    }
    
    nuevo->siguiente = anterior->siguiente;
    anterior->siguiente = nuevo;
    
    // Renumerar resto de la lista
    nodo_linea* actual = nuevo->siguiente;
    unsigned int num = nroLinea + 1;
    while (actual != nullptr) {
        actual->numero = num++;
        actual = actual->siguiente;
    }
}

// Borra una línea de la posición especificada y renumera las siguientes
void borrarLineaEnLista(ListaLineas& lista, unsigned int nroLinea) {
    if (lista == nullptr) return;
    
    if (nroLinea == 1) {
        nodo_linea* temp = lista;
        lista = lista->siguiente;
        liberarString(temp->texto);
        delete temp;
        
        // Renumerar desde 1
        nodo_linea* actual = lista;
        unsigned int num = 1;
        while (actual != nullptr) {
            actual->numero = num++;
            actual = actual->siguiente;
        }
        return;
    }
    
    nodo_linea* anterior = lista;
    unsigned int pos = 1;
    
    while (anterior->siguiente != nullptr && pos < nroLinea - 1) {
        anterior = anterior->siguiente;
        pos++;
    }
    
    if (anterior->siguiente == nullptr) return;
    
    nodo_linea* temp = anterior->siguiente;
    anterior->siguiente = temp->siguiente;
    liberarString(temp->texto);
    delete temp;
    
    // Renumerar el resto
    nodo_linea* actual = anterior->siguiente;
    unsigned int num = nroLinea;
    while (actual != nullptr) {
        actual->numero = num++;
        actual = actual->siguiente;
    }
}

// Imprime todas las líneas con su numeración
void imprimirListaLineas(ListaLineas lista) {
    nodo_linea* actual = lista;
    while (actual != nullptr) {
        cout << actual->numero << ". " << actual->texto << endl;
        actual = actual->siguiente;
    }
}

// Libera toda la memoria de la lista de líneas
void liberarListaLineas(ListaLineas& lista) {
    while (lista != nullptr) {
        nodo_linea* temp = lista;
        lista = lista->siguiente;
        liberarString(temp->texto);
        delete temp;
    }
}

// Cuenta el número total de líneas en la lista
unsigned int contarLineas(ListaLineas lista) {
    unsigned int count = 0;
    nodo_linea* actual = lista;
    while (actual != nullptr) {
        count++;
        actual = actual->siguiente;
    }
    return count;
}

// Aplica una lista de modificaciones (inserciones/borrados) sobre el texto
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

// Busca una versión por número en una lista de hermanos
Version buscarVersionEnLista(Version lista, int numero) {
    Version actual = lista;
    while (actual != nullptr) {
        if (actual->numero == numero) return actual;
        actual = actual->siguienteHermano;
    }
    return nullptr;
}

// Agrega una modificación al final de la lista de modificaciones de una versión
void agregarModificacion(Version ver, Modificacion mod) {
    if (ver == nullptr || mod == nullptr) return;
    
    if (ver->primeraModificacion == nullptr) {
        ver->primeraModificacion = mod;
        return;
    }
    
    Modificacion actual = ver->primeraModificacion;
    while (actual->siguiente != nullptr) {
        actual = actual->siguiente;
    }
    
    actual->siguiente = mod;
}

// Parsea un string de versión (ej: "1.2.3") y retorna un array de enteros
int* parsearVersion(const char* version, int* longitud) {
    if (version == nullptr || longitud == nullptr) return nullptr;
    
    *longitud = 0;
    
    if (strlen(version) == 0) return nullptr;
    
    if (version[0] == '.' || version[strlen(version) - 1] == '.') {
        return nullptr;
    }
    
    char* copia = copiarString(version);
    
    // Contar niveles
    int niveles = 1;
    for (size_t i = 0; i < strlen(copia); i++) {
        if (copia[i] == '.') {
            niveles++;
            if (i > 0 && copia[i-1] == '.') {
                liberarString(copia);
                return nullptr;
            }
        } else if (copia[i] < '0' || copia[i] > '9') {
            liberarString(copia);
            return nullptr;
        }
    }
    
    int* numeros = new int[niveles];
    
    char* token = strtok(copia, ".");
    int indice = 0;
    
    while (token != nullptr && indice < niveles) {
        if (strlen(token) == 0) {
            delete[] numeros;
            liberarString(copia);
            return nullptr;
        }
        
        int numero = atoi(token);
        
        if (numero <= 0) {
            delete[] numeros;
            liberarString(copia);
            return nullptr;
        }
        
        numeros[indice] = numero;
        indice++;
        token = strtok(nullptr, ".");
    }
    
    liberarString(copia);
    
    if (indice != niveles) {
        delete[] numeros;
        return nullptr;
    }
    
    *longitud = niveles;
    return numeros;
}

// Libera el array dinámico de una versión parseada
void liberarArrayVersion(int*& array) {
    if (array != nullptr) {
        delete[] array;
        array = nullptr;
    }
}

// Busca un hijo específico por número entre los hijos de un padre
Version buscarHijo(Version padre, int numero) {
    if (padre == nullptr) return nullptr;
    
    Version actual = padre->primerHijo;
    
    while (actual != nullptr) {
        if (actual->numero == numero) {
            return actual;
        }
        actual = actual->siguienteHermano;
    }
    
    return nullptr;
}

// Navega por el árbol siguiendo una secuencia de números (ej: [1,2,3] → 1.2.3)
Version navegarAVersion(Version primeraVersion, int* secuencia, int longitud) {
    if (secuencia == nullptr || longitud <= 0) return nullptr;
    
    Version actual = primeraVersion;
    
    actual = buscarVersionEnLista(actual, secuencia[0]);
    
    if (actual == nullptr) return nullptr;
    
    if (longitud == 1) return actual;
    
    for (int i = 1; i < longitud; i++) {
        actual = buscarHijo(actual, secuencia[i]);
        
        if (actual == nullptr) return nullptr;
    }
    
    return actual;
}

// Construye un camino desde la versión hasta la raíz (en orden inverso: hoja→raíz)
void obtenerCaminoAncestros(Version v, Version* camino, int* longitud) {
    if (v == nullptr || camino == nullptr || longitud == nullptr) {
        if (longitud != nullptr) *longitud = 0;
        return;
    }
    
    *longitud = 0;
    Version actual = v;
    
    // Subir desde la versión hasta la raíz siguiendo punteros padre
    while (actual != nullptr) {
        camino[*longitud] = actual;
        (*longitud)++;
        actual = actual->padre;
    }
    
    // Invertir el camino para que quede raíz→hoja
    for (int i = 0; i < *longitud / 2; i++) {
        Version temp = camino[i];
        camino[i] = camino[*longitud - 1 - i];
        camino[*longitud - 1 - i] = temp;
    }
}

// Valida que el padre de una versión existe en el árbol
bool validarPadreExiste(Version primeraVersion, int* secuencia, int longitud) {
    if (secuencia == nullptr || longitud <= 0) return false;
    
    if (longitud == 1) return true;
    
    // El padre es todo menos el último número
    Version padre = navegarAVersion(primeraVersion, secuencia, longitud - 1);
    
    return (padre != nullptr);
}

// Valida que crear una versión con este número no genere huecos en la numeración
bool validarSinHuecos(Version padre, Version primeraVersion, int numeroNuevo) {
    Version hermanos = (padre == nullptr) ? primeraVersion : padre->primerHijo;
    
    if (hermanos == nullptr) {
        return (numeroNuevo == 1);
    }
    
    int count = 0;
    int maxNumero = 0;
    bool numeroExiste = false;
    
    Version actual = hermanos;
    while (actual != nullptr) {
        count++;
        if (actual->numero > maxNumero) {
            maxNumero = actual->numero;
        }
        if (actual->numero == numeroNuevo) {
            numeroExiste = true;
        }
        actual = actual->siguienteHermano;
    }
    
    if (numeroExiste) {
        return true;  // Desplazamiento
    }
    
    if (numeroNuevo == maxNumero + 1) {
        return true;  // Consecutivo
    }
    
    if (numeroNuevo > 0 && numeroNuevo < maxNumero) {
        return true;  // Llena un hueco
    }
    
    if (numeroNuevo > maxNumero + 1) {
        return false;  // Crearía huecos
    }
    
    if (numeroNuevo <= 0) {
        return false;
    }
    
    return true;
}

// Renumera un nodo (los hijos mantienen su numeración relativa al padre)
void renumerarSubarbol(Version raiz, int delta) {
    if (raiz == nullptr) {
        return;
    }
    
    // 1. Renumerar este nodo
    raiz->numero += delta;
    
    // 2. Renumerar RECURSIVAMENTE todos los hijos
    Version hijo = raiz->primerHijo;
    while (hijo != nullptr) {
        renumerarSubarbol(hijo, delta);
        hijo = hijo->siguienteHermano;
    }
}

// Desplaza y renumera versiones hermanas desde numeroInicio en adelante
void desplazarYRenumerar(Version padre, Version& primeraVersion, int numeroInicio, int delta) {
    Version hermanos = (padre != nullptr) ? padre->primerHijo : primeraVersion;
    
    if (hermanos == nullptr) {
        return;
    }
    
    Version actual = hermanos;
    
    while (actual != nullptr) {
        if (actual->numero >= numeroInicio) {
            renumerarSubarbol(actual, delta);
        }
        
        actual = actual->siguienteHermano;
    }
}

// Renumera todos los hermanos posteriores aplicando un delta
// Usado en BorrarVersion para decrementar números después de eliminar un nodo
// IMPORTANTE: Solo renumera el nodo mismo, NO sus hijos (los hijos mantienen su numeración relativa)
void renumerarHermanosPosteriores(Version primerHermano, int delta) {
    Version actual = primerHermano;
    
    while (actual != nullptr) {
        // Solo renumerar el nodo actual, no sus hijos
        actual->numero += delta;
        actual = actual->siguienteHermano;
    }
}

