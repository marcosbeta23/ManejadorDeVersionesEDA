// Estructuras de Datos y Algoritmos - Obligatorio 2025
// Tecnologo en Informatica FIng - DGETP - UTEC
//
// utils.c
// Modulo de implementacion de utilidades.

#include "utils.h"
#include <cstring>
#include <iostream>

using namespace std;

// ============================================
// UTILIDADES DE STRINGS
// ============================================

char* copiarString(const char* str) {
    if (str == nullptr) return nullptr;
    
    char* copia = new char[strlen(str) + 1];
    strcpy(copia, str);
    return copia;
}

void liberarString(char*& str) {
    if (str != nullptr) {
        delete[] str;
        str = nullptr;
    }
}

// ============================================
// UTILIDADES DE RECONSTRUCCIÓN DE TEXTO
// ============================================

ListaLineas crearListaLineas() {
    return nullptr;
}

void insertarLineaEnLista(ListaLineas& lista, unsigned int nroLinea, const char* texto) {
    nodo_linea* nuevo = new nodo_linea;
    nuevo->texto = copiarString(texto);
    nuevo->numero = nroLinea;
    nuevo->siguiente = nullptr;
    
    if (lista == nullptr || nroLinea == 1) {
        nuevo->siguiente = lista;
        lista = nuevo;
        
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
    
    nodo_linea* actual = nuevo->siguiente;
    unsigned int num = nroLinea + 1;
    while (actual != nullptr) {
        actual->numero = num++;
        actual = actual->siguiente;
    }
}

void borrarLineaEnLista(ListaLineas& lista, unsigned int nroLinea) {
    if (lista == nullptr) return;
    
    if (nroLinea == 1) {
        nodo_linea* temp = lista;
        lista = lista->siguiente;
        liberarString(temp->texto);
        delete temp;
        
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
    
    nodo_linea* actual = anterior->siguiente;
    unsigned int num = nroLinea;
    while (actual != nullptr) {
        actual->numero = num++;
        actual = actual->siguiente;
    }
}

void imprimirListaLineas(ListaLineas lista) {
    nodo_linea* actual = lista;
    while (actual != nullptr) {
        cout << actual->numero << ". " << actual->texto << endl;
        actual = actual->siguiente;
    }
}

void liberarListaLineas(ListaLineas& lista) {
    while (lista != nullptr) {
        nodo_linea* temp = lista;
        lista = lista->siguiente;
        liberarString(temp->texto);
        delete temp;
    }
}

unsigned int contarLineas(ListaLineas lista) {
    unsigned int count = 0;
    nodo_linea* actual = lista;
    while (actual != nullptr) {
        count++;
        actual = actual->siguiente;
    }
    return count;
}

// ============================================
// UTILIDADES DE VERSIONES
// ============================================

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
    
    // Si la versión no tiene modificaciones, esta es la primera
    if (ver->primeraModificacion == nullptr) {
        ver->primeraModificacion = mod;
        return;
    }
    
    // Buscar el último nodo de la lista de modificaciones
    Modificacion actual = ver->primeraModificacion;
    while (actual->siguiente != nullptr) {
        actual = actual->siguiente;
    }
    
    // Agregar al final
    actual->siguiente = mod;
}


// ============================================
// PARSER DE VERSIONES JERÁRQUICAS (FASE 2)
// ============================================

int* parsearVersion(const char* version, int* longitud) {
    if (version == nullptr || longitud == nullptr) return nullptr;
    
    *longitud = 0;
    
    // Validar que el string no esté vacío
    if (strlen(version) == 0) return nullptr;
    
    // Validar que no empiece ni termine con punto
    if (version[0] == '.' || version[strlen(version) - 1] == '.') {
        return nullptr;
    }
    
    // Hacer una copia del string para no modificar el original
    char* copia = copiarString(version);
    
    // Contar cuántos niveles hay (cantidad de números separados por puntos)
    int niveles = 1;
    for (size_t i = 0; i < strlen(copia); i++) {
        if (copia[i] == '.') {
            niveles++;
            // Validar que no haya puntos consecutivos
            if (i > 0 && copia[i-1] == '.') {
                liberarString(copia);
                return nullptr;
            }
        } else if (copia[i] < '0' || copia[i] > '9') {
            // Validar que solo haya dígitos y puntos
            liberarString(copia);
            return nullptr;
        }
    }
    
    // Alocar array para los números
    int* numeros = new int[niveles];
    
    // Parsear los números
    char* token = strtok(copia, ".");
    int indice = 0;
    
    while (token != nullptr && indice < niveles) {
        // Validar que el token no esté vacío
        if (strlen(token) == 0) {
            delete[] numeros;
            liberarString(copia);
            return nullptr;
        }
        
        // Convertir a entero
        int numero = atoi(token);
        
        // Validar que sea un número válido (mayor que 0)
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
    
    // Verificar que se parsearon todos los niveles esperados
    if (indice != niveles) {
        delete[] numeros;
        return nullptr;
    }
    
    *longitud = niveles;
    return numeros;
}

void liberarArrayVersion(int*& array) {
    if (array != nullptr) {
        delete[] array;
        array = nullptr;
    }
}


// ============================================
// NAVEGACIÓN DEL ÁRBOL N-ARIO (FASE 2)
// ============================================

Version buscarHijo(Version padre, int numero) {
    // Si el padre es NULL, no tiene hijos
    if (padre == nullptr) return nullptr;
    
    // Empezar desde el primer hijo
    Version actual = padre->primerHijo;
    
    // Recorrer la lista de hermanos buscando el número
    while (actual != nullptr) {
        if (actual->numero == numero) {
            return actual;
        }
        actual = actual->siguienteHermano;
    }
    
    // No se encontró el hijo con ese número
    return nullptr;
}

Version navegarAVersion(Version primeraVersion, int* secuencia, int longitud) {
    // Validaciones básicas
    if (secuencia == nullptr || longitud <= 0) return nullptr;
    
    // Empezar desde la lista de versiones de primer nivel
    Version actual = primeraVersion;
    
    // Buscar el primer número en la lista de primer nivel
    actual = buscarVersionEnLista(actual, secuencia[0]);
    
    // Si no existe la versión de primer nivel, retornar NULL
    if (actual == nullptr) return nullptr;
    
    // Si solo buscábamos un nivel, ya terminamos
    if (longitud == 1) return actual;
    
    // Navegar por los niveles restantes
    for (int i = 1; i < longitud; i++) {
        // Buscar el siguiente número entre los hijos del nodo actual
        actual = buscarHijo(actual, secuencia[i]);
        
        // Si no existe ese hijo, retornar NULL
        if (actual == nullptr) return nullptr;
    }
    
    // Retornar el nodo encontrado
    return actual;
}


// ============================================
// VALIDACIONES PARA CREARVERSION (FASE 2)
// ============================================

bool validarPadreExiste(Version primeraVersion, int* secuencia, int longitud) {
    // Validaciones básicas
    if (secuencia == nullptr || longitud <= 0) return false;
    
    // Si es versión de primer nivel (longitud == 1), no tiene padre, siempre válido
    if (longitud == 1) return true;
    
    // Para versiones con padre, navegar hasta el padre
    // El padre es todo menos el último número
    // Ej: para "1.2.3", el padre es "1.2" (secuencia[0..longitud-2])
    Version padre = navegarAVersion(primeraVersion, secuencia, longitud - 1);
    
    // Si el padre no existe, retornar false
    return (padre != nullptr);
}

bool validarSinHuecos(Version padre, Version primeraVersion, int numeroNuevo) {
    // Obtener la lista de hermanos
    Version hermanos = nullptr;
    
    if (padre == nullptr) {
        // Versiones de primer nivel
        hermanos = primeraVersion;
    } else {
        // Hijos del padre
        hermanos = padre->primerHijo;
    }
    
    // Si no hay hermanos, cualquier número es válido (será el primero)
    if (hermanos == nullptr) {
        return (numeroNuevo == 1);  // El primer hijo debe ser 1
    }
    
    // Recorrer la lista de hermanos y contar cuántos hay
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
    
    // Caso 1: El número ya existe → válido (será desplazamiento)
    if (numeroExiste) {
        return true;
    }
    
    // Caso 2: El número nuevo es el siguiente consecutivo → válido
    if (numeroNuevo == maxNumero + 1) {
        return true;
    }
    
    // Caso 3: El número nuevo llena un hueco existente → válido
    // Verificar si hay huecos entre 1 y maxNumero
    // Si el nuevo número está en un hueco, es válido
    if (numeroNuevo > 0 && numeroNuevo < maxNumero) {
        // Verificar si este número específicamente llena un hueco
        return true;  // Si no existe y está en el rango, llena un hueco
    }
    
    // Caso 4: El número nuevo crea un hueco → inválido
    // Esto ocurre si numeroNuevo > maxNumero + 1
    if (numeroNuevo > maxNumero + 1) {
        return false;  // Crearía huecos en maxNumero+1, maxNumero+2, ...
    }
    
    // Caso 5: numeroNuevo <= 0 → inválido
    if (numeroNuevo <= 0) {
        return false;
    }
    
    // Por defecto, aceptar
    return true;
}


// ============================================
// DESPLAZAMIENTO Y RENUMERACIÓN (FASE 2)
// ============================================

void renumerarSubarbol(Version raiz, int delta) {
    // Caso base: si el nodo es NULL, no hay nada que renumerar
    if (raiz == nullptr) {
        return;
    }
    
    // Paso 1: Cambiar el número del nodo actual
    raiz->numero += delta;
    
    // Paso 2: Recursivamente renumerar todos los hijos
    // Comenzar por el primer hijo
    Version hijo = raiz->primerHijo;
    
    // Recorrer todos los hermanos (hijos del nodo actual)
    while (hijo != nullptr) {
        // Llamada recursiva para renumerar todo el subárbol de este hijo
        renumerarSubarbol(hijo, delta);
        
        // Avanzar al siguiente hermano
        hijo = hijo->siguienteHermano;
    }
}

void desplazarYRenumerar(Version padre, Version& primeraVersion, int numeroInicio, int delta) {
    // Determinar la lista de hermanos a procesar
    Version hermanos = nullptr;
    
    if (padre != nullptr) {
        // Si hay padre, trabajar con sus hijos
        hermanos = padre->primerHijo;
    } else {
        // Si no hay padre, trabajar con versiones de primer nivel
        hermanos = primeraVersion;
    }
    
    // Si no hay hermanos, no hay nada que desplazar
    if (hermanos == nullptr) {
        return;
    }
    
    // Recorrer la lista de hermanos y desplazar los que cumplen la condición
    Version actual = hermanos;
    
    while (actual != nullptr) {
        // Si el número del nodo actual es >= numeroInicio, desplazarlo
        if (actual->numero >= numeroInicio) {
            // Renumerar este nodo y todo su subárbol
            renumerarSubarbol(actual, delta);
        }
        
        // Avanzar al siguiente hermano
        actual = actual->siguienteHermano;
    }
    
    // IMPORTANTE: Si estamos en el primer nivel y cambiamos números,
    // puede que necesitemos reordenar la lista de hermanos
    // Sin embargo, según la especificación, los hermanos NO necesitan
    // estar ordenados numéricamente, solo deben mantener el orden
    // de creación. Por lo tanto, NO reordenamos.
}

