// Estructuras de Datos y Algoritmos - Obligatorio 2025
// Tecnologo en Informatica FIng - DGETP - UTEC
//
// mod.c
// Modulo de implementacion de modificacion.

#include "mod.h"
#include <cstring>
#include <iostream>

using namespace std;

// Crea una nueva modificación
Modificacion crearModificacion(TipoMod tipo, unsigned int nroLinea, const char* texto) {
    Modificacion mod = new nodo_modificacion;
    mod->tipo = tipo;
    mod->nroLinea = nroLinea;
    
    // Solo guardamos texto si es INSERCION
    if (tipo == INSERCION && texto != NULL) {
        mod->textoLinea = new char[strlen(texto) + 1];
        strcpy(mod->textoLinea, texto);
    } else {
        mod->textoLinea = NULL;
    }
    
    mod->siguiente = NULL;
    return mod;
}

// Libera una modificación individual
void liberarModificacion(Modificacion& mod) {
    if (mod == NULL) return;
    
    // Liberar texto si existe
    if (mod->textoLinea != NULL) {
        delete[] mod->textoLinea;
        mod->textoLinea = NULL;
    }
    
    delete mod;
    mod = NULL;
}

// Libera toda una lista de modificaciones
void liberarListaModificaciones(Modificacion& lista) {
    while (lista != NULL) {
        Modificacion temp = lista;
        lista = lista->siguiente;
        liberarModificacion(temp);
    }
}
