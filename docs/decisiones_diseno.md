# Decisiones de Diseño - Obligatorio EDA 2025

## 📐 Estructuras de Datos

### Fase 1 (Control Intermedio)

#### nodo_archivo
```c
struct nodo_archivo {
    char * nombre;              // Nombre del archivo (memoria dinámica)
    Version primeraVersion;     // Lista de versiones de primer nivel
};
```

**Justificación:**
- Simple y suficiente para Fase 1
- `primeraVersion` apunta a lista enlazada de versiones (1, 2, 3...)

#### nodo_version
```c
struct nodo_version {
    int numero;                      // 1, 2, 3, ... (primer nivel)
    Modificacion primeraModificacion; // Lista de cambios
    struct nodo_version * siguienteHermano; // Siguiente versión
};
```

**Justificación:**
- Lista enlazada simple para versiones de primer nivel
- `siguienteHermano` conecta versiones horizontalmente
- En Fase 2 se agregará `padre` y `primerHijo` para árbol n-ario

#### nodo_modificacion
```c
enum TipoMod { INSERCION, BORRADO };

struct nodo_modificacion {
    TipoMod tipo;                    // Tipo de cambio
    unsigned int nroLinea;           // Línea afectada
    char * textoLinea;               // Texto (solo INSERCION)
    struct nodo_modificacion * siguiente; // Siguiente cambio
};
```

**Justificación:**
- Sistema de deltas incrementales (no guardamos texto completo)
- Lista enlazada preserva orden temporal de modificaciones
- `textoLinea` es NULL para BORRADO (ahorro de memoria)

## 🔄 Algoritmos

### Reconstrucción de Texto (MostrarTexto)

```
1. Crear lista temporal de líneas vacía
2. Para cada modificación en orden:
   - Si INSERCION: insertar textoLinea en posición nroLinea
   - Si BORRADO: eliminar línea en posición nroLinea
3. Imprimir todas las líneas numeradas
4. Liberar lista temporal
```

**Complejidad:** O(n*m) donde n=modificaciones, m=líneas promedio

### Validación de nroLinea

Para insertar: `1 <= nroLinea <= cantidadLineasActual + 1`
Para borrar: `1 <= nroLinea <= cantidadLineasActual`

Requiere reconstruir texto mentalmente para saber cuántas líneas hay.

## 💾 Gestión de Memoria

### Orden de Liberación (BorrarArchivo)
```
Para cada versión:
  Para cada modificación:
    1. delete[] textoLinea (si no es NULL)
    2. delete modificacion
  3. delete version
4. delete[] nombre
5. delete archivo
6. archivo = NULL
```

**Importante:** Liberar en orden inverso a la creación.

## 🔮 Extensibilidad para Fase 2

La estructura actual se puede extender fácilmente:
- Agregar `padre` a nodo_version
- Agregar `primerHijo` a nodo_version
- Cambiar parseo de versión para soportar multinivel
- Agregar validaciones de subversiones
- Implementar algoritmo de desplazamiento

---
**Última actualización:** 21 Oct 2025