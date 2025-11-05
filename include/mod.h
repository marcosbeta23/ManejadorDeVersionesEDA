#ifndef MOD_H
#define MOD_H

// Estructuras de Datos y Algoritmos - Obligatorio 2025
// Tecnologo en Informatica FIng - DGETP - UTEC
//
// mod.h
// Modulo de especificacion de modificacion.

// Tipo de modificación
enum TipoMod {
    INSERCION,
    BORRADO
};

// Representa un cambio (inserción o borrado) en una versión
struct nodo_modificacion {
    TipoMod tipo;
    unsigned int nroLinea;
    char* textoLinea;
    struct nodo_modificacion* siguiente;
};

typedef struct nodo_modificacion* Modificacion;

// Crea una nueva modificación (inserción o borrado)
Modificacion crearModificacion(TipoMod tipo, unsigned int nroLinea, const char* texto);

// Libera una modificación individual
void liberarModificacion(Modificacion& mod);

// Libera toda la lista de modificaciones
void liberarListaModificaciones(Modificacion& lista);

#endif // MOD_H
