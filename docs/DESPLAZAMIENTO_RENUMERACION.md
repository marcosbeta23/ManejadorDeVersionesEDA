# TARJETA 5: Desplazamiento y Renumeración

## Descripción
Implementación del comportamiento de desplazamiento y renumeración recursiva cuando se crea una versión con un número que ya existe. Esta funcionalidad es esencial para mantener la integridad del árbol n-ario permitiendo insertar versiones en cualquier posición.

## Funciones Implementadas

### 1. `renumerarSubarbol`
```c
void renumerarSubarbol(Version raiz, int delta);
```

**Propósito:** Renumerar recursivamente un subárbol completo, modificando el número del nodo raíz y de todos sus descendientes.

**Parámetros:**
- `raiz`: Nodo raíz del subárbol a renumerar
- `delta`: Cantidad a sumar/restar a cada número (puede ser positivo o negativo)

**Algoritmo:**
1. Verificar que el nodo no sea NULL (caso base)
2. Sumar `delta` al número del nodo actual
3. Iterar por todos los hijos (comenzando por `primerHijo`)
4. Para cada hijo, llamar recursivamente a `renumerarSubarbol`
5. Avanzar al siguiente hermano usando `siguienteHermano`

**Complejidad:** O(n) donde n es el número total de nodos en el subárbol

**Ejemplos:**

#### Ejemplo 1: Renumeración simple
```
ANTES:
  2 → 2.1
      2.2

renumerarSubarbol(v2, +1)

DESPUÉS:
  3 → 3.1
      3.2
```

#### Ejemplo 2: Renumeración profunda
```
ANTES:
  1 → 1.2 → 1.2.3 → 1.2.3.1

renumerarSubarbol(v1, +1)

DESPUÉS:
  2 → 2.3 → 2.3.4 → 2.3.4.2
```

#### Ejemplo 3: Delta negativo (para BorrarVersion)
```
ANTES:
  5 → 5.3
      5.4

renumerarSubarbol(v5, -2)

DESPUÉS:
  3 → 3.1
      3.2
```

---

### 2. `desplazarYRenumerar`
```c
void desplazarYRenumerar(Version padre, Version& primeraVersion, int numeroInicio, int delta);
```

**Propósito:** Desplazar todas las versiones hermanas desde una posición específica, renumerándolas recursivamente junto con sus descendientes.

**Parámetros:**
- `padre`: Nodo padre (NULL para versiones de primer nivel)
- `primeraVersion`: Referencia a la primera versión del nivel (solo para primer nivel)
- `numeroInicio`: Número desde el cual desplazar (inclusive)
- `delta`: Cantidad a desplazar (+1 para inserción, -1 para borrado)

**Algoritmo:**
1. Determinar la lista de hermanos:
   - Si hay padre: usar `padre->primerHijo`
   - Si no hay padre: usar `primeraVersion`
2. Iterar por todos los hermanos
3. Para cada hermano con `numero >= numeroInicio`:
   - Llamar a `renumerarSubarbol(hermano, delta)`
4. Continuar con el siguiente hermano

**Nota importante:** Los hermanos NO se reordenan numéricamente. Mantienen el orden de creación.

**Ejemplos:**

#### Ejemplo 1: Desplazamiento en primer nivel
```
ESTADO INICIAL:
1
2 → 2.1
    2.2
3

Se va a crear nueva versión 2
↓
desplazarYRenumerar(NULL, primeraVersion, 2, +1)

RESULTADO:
1
[espacio para nueva 2]
3 (antes era 2) → 3.1 (antes era 2.1)
                  3.2 (antes era 2.2)
4 (antes era 3)
```

#### Ejemplo 2: Desplazamiento de hijos
```
ESTADO INICIAL:
v1 con hijos:
  1.1
  1.2 → 1.2.1
  1.3

Se va a crear nuevo hijo 1.2
↓
desplazarYRenumerar(v1, dummy, 2, +1)

RESULTADO:
v1 con hijos:
  1.1
  [espacio para nuevo 1.2]
  1.3 (antes era 1.2) → 1.3.1 (antes era 1.2.1)
  1.4 (antes era 1.3)
```

#### Ejemplo 3: Desplazamiento para BorrarVersion
```
ESTADO INICIAL:
1
2 (será borrada)
3
4

Después de borrar 2:
↓
desplazarYRenumerar(NULL, primeraVersion, 3, -1)

RESULTADO:
1
[2 fue borrada]
2 (antes era 3)
3 (antes era 4)
```

---

## Integración con CrearVersion

El desplazamiento se usa en `CrearVersion` cuando se detecta que el número ya existe:

```c
Ret CrearVersion(Archivo& archivo, Cadena nombreVersion) {
    // 1. Parsear versión
    int* secuencia = nullptr;
    int longitud = 0;
    parsearVersion(nombreVersion, secuencia, longitud);
    
    // 2. Validar padre existe
    if (!validarPadreExiste(archivo->primeraVersion, secuencia, longitud)) {
        return ERROR;
    }
    
    // 3. Validar sin huecos
    int numeroNuevo = secuencia[longitud - 1];
    Version padre = (longitud > 1) ? 
        navegarAVersion(archivo->primeraVersion, secuencia, longitud-1) : 
        nullptr;
    
    if (!validarSinHuecos(padre, archivo->primeraVersion, numeroNuevo)) {
        return ERROR;
    }
    
    // 4. Verificar si número existe → DESPLAZAMIENTO
    Version hermanos = (padre != nullptr) ? padre->primerHijo : archivo->primeraVersion;
    Version existente = buscarHijo(padre, numeroNuevo);
    
    if (existente != nullptr) {
        // El número existe, desplazar desde esa posición
        desplazarYRenumerar(padre, archivo->primeraVersion, numeroNuevo, +1);
    }
    
    // 5. Crear nueva versión con el número deseado
    Version nueva = crearVersion(numeroNuevo);
    
    // 6. Insertar en la posición correcta...
    // ... resto de la lógica ...
}
```

---

## Test Suite

### Archivo: `test_desplazamiento.cpp`

**Tests de `renumerarSubarbol`:** 6 tests
- Nodo único sin hijos
- Nodo con un hijo
- Nodo con múltiples hijos hermanos
- Árbol profundo (3 niveles)
- Delta negativo
- NULL no crashea

**Tests de `desplazarYRenumerar`:** 7 tests
- Desplazar versiones de primer nivel
- Desplazar con hijos (caso complejo)
- Desplazar hijos de un padre específico
- Desplazar todos desde 1
- Desplazar solo el último
- Desplazar con delta negativo
- Lista vacía no crashea

**Tests de escenarios complejos:** 1 test
- Árbol completo con múltiples niveles

### Resultados
```
Tests ejecutados: 14
Tests pasados:    14
Tests fallados:   0

✓ TODOS LOS TESTS PASARON
```

### Valgrind
```
==703== HEAP SUMMARY:
==703==     in use at exit: 0 bytes in 0 blocks
==703==   
==703== All heap blocks were freed -- no leaks are possible
==703== 
==703== ERROR SUMMARY: 0 errors from 0 contexts (suppressed: 0 from 0)
```

---

## Casos de Uso

### Caso 1: Crear versión en medio
```
Comando: CrearVersion "2"

ANTES:
1
2 → 2.1
    2.2
3

PROCESO:
1. validarPadreExiste([2], 1) → true (primer nivel)
2. validarSinHuecos(NULL, primeraVersion, 2) → true (existe, será desplazamiento)
3. desplazarYRenumerar(NULL, primeraVersion, 2, +1)
4. Crear nueva versión 2

DESPUÉS:
1
2 (nueva, vacía)
3 (antes 2) → 3.1 (antes 2.1)
              3.2 (antes 2.2)
4 (antes 3)
```

### Caso 2: Crear subversión con desplazamiento
```
Comando: CrearVersion "1.2"

ANTES:
1 → 1.1
    1.2 → 1.2.1
    1.3

PROCESO:
1. validarPadreExiste([1,2], 2) → true (1 existe)
2. validarSinHuecos(v1, NULL, 2) → true (existe, será desplazamiento)
3. desplazarYRenumerar(v1, dummy, 2, +1)
4. Crear nueva versión 1.2

DESPUÉS:
1 → 1.1
    1.2 (nueva, vacía)
    1.3 (antes 1.2) → 1.3.1 (antes 1.2.1)
    1.4 (antes 1.3)
```

### Caso 3: BorrarVersion con desplazamiento
```
Comando: BorrarVersion "2"

ANTES:
1
2 → 2.1
    2.2
3
4

PROCESO:
1. Localizar versión 2
2. Liberar recursivamente 2, 2.1, 2.2
3. Eliminar 2 de la lista de hermanos
4. desplazarYRenumerar(NULL, primeraVersion, 3, -1)

DESPUÉS:
1
[2 fue borrada]
2 (antes 3)
3 (antes 4)
```

---

## Propiedades Importantes

### Invariantes Mantenidos
1. **No hay huecos en numeración**: Después del desplazamiento, los números siguen siendo consecutivos
2. **Orden de creación preservado**: Los hermanos mantienen su orden relativo en la lista
3. **Propagación completa**: El cambio de número se propaga a TODOS los niveles de descendientes
4. **Sin fugas de memoria**: Todas las estructuras existentes se mantienen intactas

### Complejidad
- **renumerarSubarbol**: O(n) donde n = nodos en subárbol
- **desplazarYRenumerar**: O(h×m) donde:
  - h = número de hermanos afectados
  - m = promedio de nodos por subárbol afectado

---

## Archivos Modificados

### `include/utils.h`
- Agregadas declaraciones de funciones
- Documentación completa con ejemplos

### `src/utils.c`
- Implementación de `renumerarSubarbol`
- Implementación de `desplazarYRenumerar`

### Tests y scripts
- `test_desplazamiento.cpp`: Suite completa de tests
- `test_desplazamiento.sh`: Script de compilación y ejecución

---

## Próximos Pasos

**TARJETA 6:** CrearVersion Completa
- Integrar parser + navegación + validaciones + desplazamiento
- Insertar nueva versión en posición correcta
- Heredar texto del padre
- Manejar todos los casos especiales

---

## Estado
✅ **COMPLETADA**
- Fecha: 4 de noviembre de 2025
- Tests: 14/14 pasando (100%)
- Memory leaks: 0
- Funcionalidad lista para integración en CrearVersion
