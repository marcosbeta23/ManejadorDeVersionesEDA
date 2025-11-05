# TARJETA 6: CrearVersion Completa - Integración Total

## Fecha de Completación
**4 de noviembre de 2025**

## Objetivo
Implementar la función `CrearVersion()` completa que integra todas las funcionalidades desarrolladas en las tarjetas anteriores: parser, navegación, validaciones y desplazamiento.

## Funcionalidad Implementada

### Función Principal: `CrearVersion()`
**Archivo:** `src/archivo.c` (líneas 80-175)

**Firma:**
```c
TipoRet CrearVersion(Archivo &a, char * version)
```

### Algoritmo de 10 Pasos

#### PASO 1: Parser de Versión
```c
secuencia = parsearVersion(version, &longitud);
```
- Convierte string "1.2.3" en array [1,2,3] con longitud 3
- Valida formato, números positivos, sin puntos consecutivos
- Retorna ERROR si formato inválido

#### PASO 2: Validar Padre Existe
```c
if (!validarPadreExiste(a->primeraVersion, secuencia, longitud)) {
    return ERROR;
}
```
- Para "1.2.3", verifica que "1.2" existe
- Para versiones de primer nivel (longitud==1), siempre válido

#### PASO 3: Extraer Número Nuevo
```c
int numeroNuevo = secuencia[longitud - 1];
```
- Extrae el último número de la secuencia
- Ejemplo: "1.2.3" → numeroNuevo = 3

#### PASO 4: Navegar al Padre
```c
if (longitud > 1) {
    padre = navegarAVersion(a->primeraVersion, secuencia, longitud - 1);
}
```
- Navega hasta el nodo padre
- Si es primer nivel, padre = nullptr

#### PASO 5: Validar Sin Huecos
```c
if (!validarSinHuecos(padre, a->primeraVersion, numeroNuevo)) {
    return ERROR;
}
```
- Verifica que no se creen huecos en la numeración
- Permite: consecutivo, llenar hueco, o número existente (desplazamiento)
- Rechaza: números que creen huecos

#### PASO 6: Verificar Existencia (Desplazamiento)
```c
Version hermanos = (padre != nullptr) ? padre->primerHijo : a->primeraVersion;
Version existente = (padre != nullptr) ? buscarHijo(padre, numeroNuevo) 
                                       : buscarVersionEnLista(hermanos, numeroNuevo);

if (existente != nullptr) {
    desplazarYRenumerar(padre, a->primeraVersion, numeroNuevo, 1);
}
```
- Busca si el número ya existe entre hermanos
- Si existe: desplaza todos los hermanos >= numeroNuevo en +1
- **Importante:** Usa diferentes funciones para primer nivel vs subversiones

#### PASO 7: Crear Nuevo Nodo
```c
Version nuevaVersion = crearVersion(numeroNuevo);
```
- Crea el nodo de versión con malloc
- Inicializa todos los punteros en nullptr

#### PASO 8: Establecer Puntero al Padre
```c
nuevaVersion->padre = padre;
```
- Conecta el hijo con su padre
- Para primer nivel, padre = nullptr

#### PASO 9: Insertar en Lista de Hermanos (Orden Numérico)
```c
// Caso 1: Subversiones (padre != nullptr)
if (padre != nullptr) {
    if (padre->primerHijo == nullptr) {
        padre->primerHijo = nuevaVersion;
    } else {
        // Insertar en orden numérico
        while (actual != nullptr && actual->numero < numeroNuevo) {
            anterior = actual;
            actual = actual->siguienteHermano;
        }
        // Insertar en posición correcta
    }
}
// Caso 2: Primer nivel (padre == nullptr)
else {
    if (a->primeraVersion == nullptr) {
        a->primeraVersion = nuevaVersion;
    } else {
        // Insertar en orden numérico
    }
}
```
- Inserta el nuevo nodo en la lista de hermanos
- Mantiene el orden numérico ascendente
- Dos casos diferentes según si es primer nivel o subversión

#### PASO 10: Liberar Memoria Temporal
```c
liberarArrayVersion(secuencia);
return OK;
```
- Libera el array temporal del parser
- Retorna éxito

## Función Auxiliar Clave: `renumerarSubarbol()`

### Problema Encontrado y Corregido
**Archivo:** `src/utils.c`

**Versión INCORRECTA (inicial):**
```c
void renumerarSubarbol(Version raiz, int delta) {
    if (raiz == nullptr) return;
    
    raiz->numero += delta;
    
    // ❌ ERROR: Renumeraba recursivamente los hijos
    Version hijo = raiz->primerHijo;
    while (hijo != nullptr) {
        renumerarSubarbol(hijo, delta);  // Recursión incorrecta
        hijo = hijo->siguienteHermano;
    }
}
```

**Problema:**
- Si 2.1 se desplazaba, se convertía en 3.2 (renumeraba padre +1 y hijo +1)
- Los hijos perdían su numeración relativa al padre
- TEST 9 fallaba: versiones 3.1 y 3.2 no existían después del desplazamiento

**Versión CORRECTA (final):**
```c
void renumerarSubarbol(Version raiz, int delta) {
    if (raiz == nullptr) return;
    
    raiz->numero += delta;
    
    // ✅ CORRECTO: NO renumeramos los hijos
    // Los hijos mantienen su numeración relativa al padre
    // Ejemplo: Si 2.1 se desplaza a 3, se convierte en 3.1
}
```

**Solución:**
- Solo se renumera el nodo padre, NO sus hijos
- Los hijos mantienen su número relativo
- 2.1 → 3.1 (solo padre +1, hijo permanece .1)

### Análisis del Debug
**Comportamiento INCORRECTO:**
```
[DEBUG] Renumerando nodo 2 -> 3        (padre)
[DEBUG] Procesando hijo numero 1       
[DEBUG] Renumerando nodo 1 -> 2        (hijo 1→2) ❌
[DEBUG] Procesando hijo numero 2       
[DEBUG] Renumerando nodo 2 -> 3        (hijo 2→3) ❌
Resultado: 3.2 y 3.3 en lugar de 3.1 y 3.2
```

**Comportamiento CORRECTO:**
```
[DEBUG] Renumerando nodo 2 -> 3        (padre)
[DEBUG] Renumerando nodo 3 -> 4        (siguiente hermano)
Resultado: 3.1 y 3.2 ✅
```

## Suite de Tests

### Tests Implementados (10 totales)

#### 1. Tests de Primer Nivel (3 tests)
1. **Crear primera versión "1"**
   - Estructura vacía → crear "1"
   - Verifica: OK, 1 versión existe

2. **Crear versiones consecutivas "1", "2", "3"**
   - Creación secuencial
   - Verifica: OK, 3 versiones de primer nivel

3. **Error: Crear "3" con solo [1] (hueco)**
   - Intenta crear hueco
   - Verifica: ERROR, mensaje correcto

#### 2. Tests de Subversiones (4 tests)
4. **Crear subversión "1.1"**
   - Requiere que "1" exista
   - Verifica: OK, subversión creada

5. **Crear múltiples subversiones "1.1", "1.2", "1.3"**
   - Creación consecutiva de hijos
   - Verifica: OK, 3 subversiones existen

6. **Error: Crear "2.1" sin padre "2"**
   - Padre no existe
   - Verifica: ERROR, mensaje correcto

7. **Crear versión profunda "1.2.3.4"**
   - Navegación multi-nivel
   - Requiere: 1 → 1.2 → 1.2.3 existen
   - Verifica: OK, 4 niveles de profundidad

#### 3. Tests de Desplazamiento (2 tests)
8. **Desplazamiento simple: [1,2,3] crear "2"**
   - Estructura: 1, 2, 3
   - Crear nuevo "2" → desplaza 2→3, 3→4
   - Resultado: 1, 2(nuevo), 3, 4
   - Verifica: OK, 4 versiones de primer nivel

9. **Desplazamiento con hijos: 2→3, 2.1→3.1, 2.2→3.2**
   - Estructura: 1, 2→[2.1,2.2], 3
   - Crear nuevo "2" → desplaza todo
   - Resultado: 1, 2(nuevo), 3→[3.1,3.2], 4
   - Verifica: OK, versiones 3.1 y 3.2 existen
   - **ESTE TEST INICIALMENTE FALLÓ** → corregido con fix de renumerarSubarbol()

#### 4. Tests de Casos Complejos (1 test)
10. **Estructura compleja múltiples niveles**
    - Crea estructura con varios niveles
    - Combina: creación, desplazamiento, validaciones
    - Verifica: OK, estructura completa correcta

### Resultados Finales
```
Tests ejecutados: 10
Tests pasados:    10
Tests fallados:   0
✓ TODOS LOS TESTS PASARON
```

### Verificación de Memoria
```
Valgrind: 0 errors, 0 contexts
✓ No hay memory leaks
```

## Integración de Componentes

### Módulos Utilizados

1. **Parser (TARJETA 2)**
   - `parsearVersion()` - convierte string a array
   - `liberarArrayVersion()` - libera memoria temporal

2. **Navegación (TARJETA 3)**
   - `navegarAVersion()` - busca nodo en árbol
   - `buscarHijo()` - busca hijo específico
   - `buscarVersionEnLista()` - busca en lista de hermanos

3. **Validaciones (TARJETA 4)**
   - `validarPadreExiste()` - verifica existencia de padre
   - `validarSinHuecos()` - previene huecos en numeración

4. **Desplazamiento (TARJETA 5)**
   - `desplazarYRenumerar()` - desplaza hermanos
   - `renumerarSubarbol()` - cambia número de nodo padre
   - **FIX APLICADO:** No renumera hijos recursivamente

5. **Versión (TARJETA 1)**
   - `crearVersion()` - crea nodo de versión

### Flujo de Datos
```
Input: "1.2.3"
    ↓
parsearVersion() → [1,2,3], longitud=3
    ↓
validarPadreExiste() → verifica que "1.2" existe
    ↓
navegarAVersion() → obtiene puntero al padre "1.2"
    ↓
validarSinHuecos() → verifica que no crea huecos
    ↓
buscarHijo() → ¿ya existe "3" en hijos de "1.2"?
    ↓ (si existe)
desplazarYRenumerar() → desplaza hermanos >= 3
    ↓
crearVersion(3) → crea nuevo nodo
    ↓
Insertar en lista de hermanos (orden numérico)
    ↓
liberarArrayVersion() → libera memoria temporal
    ↓
Return OK
```

## Casos de Borde Manejados

### 1. Primera Versión del Archivo
- Array vacío → crear "1"
- `a->primeraVersion == nullptr`
- Asigna directamente: `a->primeraVersion = nuevaVersion`

### 2. Primer Hijo de un Padre
- Padre sin hijos → crear ".1"
- `padre->primerHijo == nullptr`
- Asigna: `padre->primerHijo = nuevaVersion`

### 3. Desplazamiento en Primer Nivel
- Usa `buscarVersionEnLista()` en lugar de `buscarHijo()`
- Maneja correctamente `padre == nullptr`

### 4. Desplazamiento con Hijos
- Los hijos mantienen su número relativo
- 2.1.5 → 3.1.5 (solo cambia el padre)

### 5. Inserción en Posición Intermedia
- [1, 3, 5] crear "4" → [1, 3, 4, 5]
- Encuentra posición correcta en lista ordenada

### 6. Versiones Multi-Nivel
- "1.2.3.4.5.6" → navega 5 niveles
- Validaciones en cada nivel

## Problemas Encontrados y Soluciones

### Problema 1: Memory Leaks en Tests
**Síntoma:** Valgrind detectó 8 bytes leaked
**Causa:** `new int[1]{1}` en líneas 177 y 204 de test_crearversion.cpp
**Solución:** Cambiar a stack allocation `int seq[] = {1}`
**Resultado:** 0 leaks ✅

### Problema 2: Desplazamiento con Hijos Fallaba (TEST 9)
**Síntoma:** 
- Versiones 3.1 y 3.2 no existían después de desplazar 2→3
- Test mostraba estructura: 3 → [2, 3] en lugar de 3 → [1, 2]

**Causa:** 
- `renumerarSubarbol()` renumeraba recursivamente padre e hijos
- 2.1 se convertía en 3.2 en lugar de 3.1

**Solución:** 
- Eliminar recursión en `renumerarSubarbol()`
- Solo cambiar número del nodo padre
- Los hijos mantienen su numeración relativa

**Código Corregido:**
```c
void renumerarSubarbol(Version raiz, int delta) {
    if (raiz == nullptr) return;
    raiz->numero += delta;
    // NO llamar recursivamente en hijos
}
```

**Resultado:** TEST 9 pasa ✅

### Problema 3: BuscarHijo() No Funciona en Primer Nivel
**Síntoma:** Desplazamiento fallaba en primer nivel
**Causa:** `buscarHijo(nullptr, numero)` retorna nullptr siempre
**Solución:** 
```c
Version existente = (padre != nullptr) ? buscarHijo(padre, numeroNuevo) 
                                       : buscarVersionEnLista(hermanos, numeroNuevo);
```
**Resultado:** Desplazamiento funciona en ambos niveles ✅

## Archivos Modificados

### 1. src/archivo.c
- Función `CrearVersion()` completa (líneas 80-175)
- Integra 10 pasos del algoritmo
- Fix: Búsqueda correcta para primer nivel vs subversiones

### 2. src/utils.c
- Fix en `renumerarSubarbol()` (líneas 437-448)
- Eliminada recursión incorrecta en hijos
- Documentación del comportamiento correcto

### 3. test_crearversion.cpp (nuevo)
- Suite de 10 tests exhaustivos
- Tests de primer nivel, subversiones, desplazamiento
- Fix: Memory leaks en líneas 177 y 204

### 4. test_crearversion.sh (nuevo)
- Script de compilación y testing
- Integración con Valgrind
- Reporte de resultados

### 5. test_debug_desplazamiento.cpp (nuevo)
- Test de debugging especializado
- Ayudó a identificar el bug en renumerarSubarbol()
- Visualización de estructura del árbol

## Métricas

### Complejidad
- **Temporal:** O(n·h) donde n=hermanos, h=profundidad
  - Parser: O(m) donde m = longitud del string
  - Navegación: O(h) donde h = profundidad
  - Validaciones: O(n) donde n = número de hermanos
  - Desplazamiento: O(n) donde n = hermanos a desplazar
  - Inserción: O(n) para encontrar posición + O(1) para insertar

- **Espacial:** O(h) donde h=profundidad
  - Parser: O(h) para el array temporal
  - Stack de recursión: O(h) para navegación

### Líneas de Código
- `CrearVersion()`: ~95 líneas
- Tests: ~250 líneas
- Documentación: ~600 líneas

### Cobertura de Tests
- **10/10 escenarios cubiertos** (100%)
- Primer nivel: 3 tests
- Subversiones: 4 tests
- Desplazamiento: 2 tests
- Casos complejos: 1 test

## Conclusiones

### Logros
✅ Integración exitosa de todos los componentes de Fase 2  
✅ 10/10 tests pasando sin errores  
✅ 0 memory leaks verificado con Valgrind  
✅ Manejo robusto de casos de borde  
✅ Código documentado y maintainable  
✅ Bug crítico identificado y corregido (renumerarSubarbol)  

### Aprendizajes Clave
1. **Jerarquía de versiones:** Los hijos mantienen numeración relativa al padre
2. **Desplazamiento:** Solo el padre cambia número, hijos permanecen iguales
3. **Testing:** Tests exhaustivos revelan bugs sutiles
4. **Debugging:** Herramientas de visualización son cruciales
5. **Memory management:** Stack allocation preferible cuando sea posible

### Próximos Pasos
- **TARJETA 7:** MostrarVersiones - visualización del árbol
- **TARJETA 8:** MostrarTexto - reconstrucción con ancestros
- **TARJETA 9:** MostrarCambios - diferencias sin ancestros
- **TARJETAS 10-12:** BorrarVersion - eliminación con renumeración inversa

---

**Estado:** ✅ COMPLETADA  
**Fecha:** 4 de noviembre de 2025  
**Tests:** 10/10 PASS  
**Memory Leaks:** 0  
**Autor:** Marcos Beta
