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

// Estructura de modificación
// Representa un cambio (inserción o borrado) realizado en una versión
struct nodo_modificacion {
    TipoMod tipo;                           // Tipo de cambio (INSERCION o BORRADO)
    unsigned int nroLinea;                   // Número de línea afectada
    char* textoLinea;                        // Texto de la línea (NULL para BORRADO)
    struct nodo_modificacion* siguiente;     // Siguiente modificación en la lista
};

typedef struct nodo_modificacion* Modificacion;

// ============================================
// FUNCIONES PARA GESTIÓN DE MODIFICACIONES
// ============================================

// Crea una nueva modificación
// Parámetros:
//   - tipo: INSERCION o BORRADO
//   - nroLinea: número de línea afectada
//   - texto: texto de la línea (solo para INSERCION, NULL para BORRADO)
// Retorna: puntero a la nueva modificación
Modificacion crearModificacion(TipoMod tipo, unsigned int nroLinea, const char* texto);

// Libera una modificación individual
// Parámetros:
//   - mod: modificación a liberar (se asigna NULL después)
void liberarModificacion(Modificacion& mod);

// Libera toda una lista de modificaciones
// Parámetros:
//   - lista: primera modificación de la lista (se asigna NULL después)
void liberarListaModificaciones(Modificacion& lista);

#endif // MOD_H
