# TARJETA 4: Validaciones de CrearVersion

## Descripción
Implementación de las validaciones necesarias para la operación `CrearVersion`, garantizando la integridad del árbol n-ario de versiones.

## Funciones Implementadas

### 1. `validarPadreExiste`
```c
bool validarPadreExiste(Version primeraVersion, int* secuencia, int longitud);
```

**Propósito:** Verificar que el padre de una nueva versión exista antes de crearla.

**Lógica:**
- Si `longitud == 1`: Es versión de primer nivel (sin padre) → retorna `true`
- Si `longitud > 1`: Navega hasta el padre usando `navegarAVersion(secuencia, longitud-1)`
- Retorna `true` si el padre existe, `false` si no existe

**Ejemplos:**
```
Árbol: [1, 2, 3]
       ├─1: [1.1, 1.2, 1.3]

✓ [2,1] → padre [2] existe
✓ [1,4] → padre [1] existe  
✗ [5,1] → padre [5] NO existe
✓ [1,2,1] → padre [1,2] existe
✗ [1,5,1] → padre [1,5] NO existe
```

---

### 2. `validarSinHuecos`
```c
bool validarSinHuecos(Version padre, Version primeraVersion, int numeroNuevo);
```

**Propósito:** Validar que un nuevo número de versión no cree huecos en la numeración de hermanos.

**Parámetros:**
- `padre`: Versión padre (puede ser `nullptr` para primer nivel)
- `primeraVersion`: Primera versión del nivel (para primer nivel)
- `numeroNuevo`: Número que se desea asignar a la nueva versión

**Algoritmo:**
1. Determinar lista de hermanos:
   - Si hay padre: `padre->primerHijo`
   - Si no: usar `primeraVersion`

2. Recorrer hermanos y calcular:
   - `contador`: cantidad de hermanos
   - `maximo`: número más alto
   - `existe`: si `numeroNuevo` ya está usado

3. Validar según casos:

#### **CASO 1: Número ya existe**
```
Hermanos: [1, 2, 3]
Nuevo: 2
Resultado: ✓ VÁLIDO (desplazamiento permitido)
```

#### **CASO 2: Siguiente consecutivo**
```
Hermanos: [1, 2, 3]
Nuevo: 4
Resultado: ✓ VÁLIDO (4 == 3+1)
```

#### **CASO 3: Llenar hueco existente**
```
Hermanos: [1, 3]
Nuevo: 2
Resultado: ✓ VÁLIDO (llena hueco entre 1 y 3)
```

#### **CASO 4: Crear hueco**
```
Hermanos: [1, 2]
Nuevo: 5
Resultado: ✗ INVÁLIDO (crearía huecos en 3 y 4)
```

#### **CASO 5: Número inválido**
```
Hermanos: [1, 2, 3]
Nuevo: 0 o -1
Resultado: ✗ INVÁLIDO (números deben ser > 0)
```

---

## Test Suite

### Archivo: `test_validaciones.cpp`

**Tests de `validarPadreExiste`:** 9 tests
- 2 tests de primer nivel (sin padre)
- 4 tests de segundo nivel (con/sin padre)
- 3 tests de tercer nivel (con/sin padre)

**Tests de `validarSinHuecos`:** 17 tests
- Casos consecutivos
- Casos de desplazamiento
- Casos de llenar huecos
- Casos de crear huecos (error)
- Casos de números inválidos
- Casos con diferentes padres

**Tests de casos límite:** 3 tests
- Árbol vacío
- Parámetros NULL
- Longitudes inválidas

### Resultados
```
Tests ejecutados: 29
Tests pasados:    29
Tests fallados:   0

✓ TODOS LOS TESTS PASARON
```

### Valgrind
```
==691== HEAP SUMMARY:
==691==     in use at exit: 0 bytes in 0 blocks
==691==   total heap usage: 14 allocs, 14 frees, 77,280 bytes allocated
==691== 
==691== All heap blocks were freed -- no leaks are possible
```

---

## Integración con CrearVersion

Estas validaciones serán utilizadas por `CrearVersion` en TARJETA 6:

```c
Ret CrearVersion(Archivo& archivo, Cadena nombreVersion) {
    // 1. Parsear versión
    int* secuencia = nullptr;
    int longitud = 0;
    if (!parsearVersion(nombreVersion, secuencia, longitud)) {
        return ERROR_FORMATO;
    }
    
    // 2. VALIDAR PADRE EXISTE
    if (!validarPadreExiste(archivo->primeraVersion, secuencia, longitud)) {
        liberarArrayVersion(secuencia, longitud);
        return ERROR_VERSION_NO_EXISTE;  // Padre no existe
    }
    
    // 3. Obtener número nuevo
    int numeroNuevo = secuencia[longitud - 1];
    
    // 4. VALIDAR SIN HUECOS
    Version padre = (longitud > 1) ? 
        navegarAVersion(archivo->primeraVersion, secuencia, longitud-1) : 
        nullptr;
    
    if (!validarSinHuecos(padre, archivo->primeraVersion, numeroNuevo)) {
        liberarArrayVersion(secuencia, longitud);
        return ERROR;  // Crearía hueco
    }
    
    // 5. Crear versión (con posible desplazamiento)
    // ... resto de lógica ...
}
```

---

## Archivos Modificados

### `include/utils.h`
- Agregadas declaraciones de funciones
- Documentación completa con ejemplos

### `src/utils.c`
- Implementación de `validarPadreExiste`
- Implementación de `validarSinHuecos`

### Tests y scripts
- `test_validaciones.cpp`: Suite completa de tests
- `test_validaciones.sh`: Script de compilación y ejecución
- `valgrind_validaciones.log`: Verificación de memoria

---

## Próximos Pasos

**TARJETA 5:** Desplazamiento y Renumeración
- Implementar lógica para desplazar versiones cuando se inserta con número existente
- Renumerar hijos recursivamente

**TARJETA 6:** CrearVersion Completa
- Integrar parser + navegación + validaciones + desplazamiento
- Crear nueva versión en el árbol
- Heredar texto del padre

---

## Estado
✅ **COMPLETADA**
- Fecha: 4 de noviembre de 2025
- Tests: 29/29 pasando (100%)
- Memory leaks: 0
- Validaciones listas para integración
