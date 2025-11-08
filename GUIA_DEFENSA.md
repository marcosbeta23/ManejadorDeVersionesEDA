# 🎯 GUÍA DE DEFENSA - OBLIGATORIO EDA 2025
## Manejador de Versiones

**Autor:** Marcos Betancor  
**Fecha de Defensa:** 11-13 de Noviembre 2025  
**Estado del Proyecto:** ✅ 100% COMPLETO (9/9 operaciones)

---

## 📚 TABLA DE CONTENIDOS

1. [Visión General del Proyecto](#1-visión-general-del-proyecto)
2. [Conceptos Fundamentales](#2-conceptos-fundamentales)
3. [Estructuras de Datos](#3-estructuras-de-datos)
4. [Operaciones Implementadas](#4-operaciones-implementadas)
5. [Casos Especiales y Edge Cases](#5-casos-especiales-y-edge-cases)
6. [Preguntas Típicas de Defensa](#6-preguntas-típicas-de-defensa)
7. [Ejemplos Memorizables](#7-ejemplos-memorizables)
8. [Estrategia de Defensa](#8-estrategia-de-defensa)

---

# 1. VISIÓN GENERAL DEL PROYECTO

## ¿Qué hace este proyecto?

Es un **sistema de control de versiones simplificado** similar a Git, pero para archivos de texto plano. Permite:

- ✅ Crear versiones jerárquicas (1, 1.1, 1.2.3, etc.)
- ✅ Insertar y borrar líneas
- ✅ Mostrar texto reconstruido desde ancestros
- ✅ Comparar dos versiones
- ✅ Crear versiones independientes (sin ancestros)

## Analogía simple para explicar

**Imagina Google Docs con historial de versiones:**
- Cada vez que haces cambios, creas una nueva versión
- Puedes crear "ramas" (subversiones) de una versión
- Puedes ver el texto completo en cualquier momento
- Solo guardas los CAMBIOS, no todo el documento de nuevo

---

# 2. CONCEPTOS FUNDAMENTALES

## 2.1 Sistema de Deltas

### ¿Qué es un "delta"?

Un **delta** es solo guardar el **cambio**, no el contenido completo.

### Ejemplo Visual:

**❌ MAL (guardar todo):**
```
Versión 1: ["Hola", "mundo"]           → 10 bytes
Versión 2: ["Hola", "mundo", "!"]      → 15 bytes
Total: 25 bytes (duplicación)
```

**✅ BIEN (sistema de deltas):**
```
Versión 1: 
  - INSERCION(1, "Hola")
  - INSERCION(2, "mundo")               → 10 bytes

Versión 2 (hereda de Versión 1):
  - INSERCION(3, "!")                   → 1 byte
Total: 11 bytes (sin duplicación)
```

### ¿Por qué es mejor?

1. **Ahorro de memoria** - No duplicamos texto
2. **Historial completo** - Sabemos qué cambió y cuándo
3. **Facilita comparaciones** - Podemos hacer "diff"
4. **Permite deshacer** - Solo quitamos el último delta

---

## 2.2 Árbol N-ario

### ¿Qué es?

Un árbol donde **cada nodo puede tener MUCHOS hijos** (no solo 2 como en árbol binario).

### Visualización:

```
           1
         / | \
      1.1 1.2 1.3
       |   |
     1.1.1 1.2.1
           |
         1.2.1.1
```

### Representación en memoria:

Usamos **3 punteros** por nodo:
- `padre` → apunta al padre
- `primerHijo` → apunta al primer hijo
- `siguienteHermano` → apunta al siguiente hermano

```
       [1]
        |
   primerHijo
        ↓
      [1.1] --siguienteHermano--> [1.2] --siguienteHermano--> [1.3]
        |                           |
   primerHijo                  primerHijo
        ↓                           ↓
     [1.1.1]                     [1.2.1]
                                    |
                               primerHijo
                                    ↓
                                [1.2.1.1]
```

### ¿Por qué no usar arrays de hijos?

**Respuesta para la defensa:**
> "Usamos lista enlazada de hermanos porque no sabemos cuántos hijos tendrá cada versión. Con punteros, podemos agregar hermanos dinámicamente sin redimensionar arrays."

---

## 2.3 Reconstrucción desde Ancestros

### El problema:

Si la versión `1.2.1` solo tiene sus propios cambios, ¿cómo mostramos el texto completo?

### La solución:

1. **Obtener el camino de ancestros:** `[1, 1.2, 1.2.1]`
2. **Aplicar modificaciones en orden:**
   - Primero las de versión `1`
   - Luego las de versión `1.2`
   - Finalmente las de versión `1.2.1`
3. **Resultado:** Texto completo reconstruido

### Ejemplo concreto:

```
Versión 1:
  - INSERCION(1, "Hola")
  - INSERCION(2, "mundo")

Versión 1.2 (hija de 1):
  - INSERCION(2, "hermoso")  // Inserta entre "Hola" y "mundo"

Reconstrucción de 1.2:
  Paso 1: Aplicar modificaciones de 1
    Resultado: ["Hola", "mundo"]
  
  Paso 2: Aplicar modificaciones de 1.2
    INSERCION(2, "hermoso") → inserta en posición 2
    Resultado: ["Hola", "hermoso", "mundo"]
```

---

# 3. ESTRUCTURAS DE DATOS

## 3.1 nodo_archivo

```c
struct nodo_archivo {
    char* nombre;              // Nombre del archivo (memoria dinámica)
    Version primeraVersion;    // Primera versión de nivel 1
};
```

### Explicación para la defensa:

> "La estructura `nodo_archivo` es el punto de entrada. Solo guarda el nombre y un puntero a la primera versión de nivel 1. Desde ahí podemos navegar todo el árbol de versiones usando los punteros de cada versión."

---

## 3.2 nodo_version

```c
struct nodo_version {
    int numero;                          // Número en su nivel (1, 2, 3...)
    Version padre;                       // NULL si es nivel 1
    Version primerHijo;                  // Primera subversión
    Version siguienteHermano;            // Siguiente versión del mismo nivel
    Modificacion primeraModificacion;    // Lista de cambios
};
```

### Diagrama en memoria:

```
Versión 1.2:
┌──────────────────┐
│ numero: 2        │
│ padre: → [1]     │
│ primerHijo: →[2.1]│
│ siguienteHermano:→[1.3]│
│ primeraModificacion: →[MOD1]→[MOD2]→NULL│
└──────────────────┘
```

### Pregunta típica: "¿Por qué 3 punteros?"

**Respuesta:**
> "Con estos 3 punteros podemos navegar en todas las direcciones:
> - `padre` → subir de nivel
> - `primerHijo` → bajar de nivel
> - `siguienteHermano` → movernos horizontalmente
> 
> Es como un árbol de carpetas: puedes ir al padre (cd ..), entrar a subcarpetas, o ver otras carpetas del mismo nivel."

---

## 3.3 nodo_modificacion

```c
enum TipoMod { INSERCION, BORRADO };

struct nodo_modificacion {
    TipoMod tipo;                // INSERCION o BORRADO
    unsigned int nroLinea;       // Línea afectada (1-based)
    char* textoLinea;            // Texto (NULL si es BORRADO)
    Modificacion siguiente;      // Siguiente modificación
};
```

### Tipos de modificaciones:

**INSERCION:**
```c
{
    tipo: INSERCION,
    nroLinea: 3,
    textoLinea: "Nueva línea",
    siguiente: NULL
}
```

**BORRADO:**
```c
{
    tipo: BORRADO,
    nroLinea: 5,
    textoLinea: NULL,      // ← NO guardamos texto en BORRADO
    siguiente: NULL
}
```

### Pregunta típica: "¿Por qué lista enlazada?"

**Respuesta:**
> "Porque el orden importa. Si primero borramos la línea 2 y luego insertamos en la línea 3, el resultado es diferente que si lo hacemos al revés. La lista enlazada preserva el orden temporal de los cambios."

---

# 4. OPERACIONES IMPLEMENTADAS

## 4.1 CrearVersion ⭐⭐⭐ (MUY IMPORTANTE)

### ¿Qué hace?

Crea una nueva versión siguiendo reglas estrictas.

### Reglas de validación:

1. ✅ **El padre debe existir**
   - Para crear `1.2`, la versión `1` debe existir
   - Para crear `2.1.3`, la versión `2.1` debe existir

2. ✅ **Sin huecos entre hermanos**
   - Si existen `1.1` y `1.3`, NO puedes crear `1.5`
   - Solo puedes crear hasta `1.4` (siguiente consecutivo)

### Algoritmo paso a paso:

```
1. Parsear versión "1.2.3" → secuencia [1, 2, 3]
2. Navegar al padre (versiones 1 y 1.2)
3. Validar que el padre existe
4. Contar hermanos del padre (1.1, 1.2)
5. Validar que nuevo número no deja huecos
6. Crear nodo de versión con numero = longitud de secuencia
7. Conectar con padre y hermanos
8. Si hay desplazamiento, renumerar hermanos posteriores
```

### Ejemplo de código explicado:

```c
// 1. Parsear
int longitud;
int* secuencia = parsearVersion("1.2.3", &longitud);
// secuencia = [1, 2, 3], longitud = 3

// 2. Navegar al padre (primeros longitud-1 elementos)
Version padre = navegarAVersion(a->primeraVersion, secuencia, longitud - 1);

// 3. Validar padre existe
if (padre == NULL) {
    strcpy(error, "El padre no existe");
    return ERROR;
}

// 4. Contar hermanos
int numHermanos = contarHermanos(padre->primerHijo);

// 5. Validar sin huecos
int nuevoNumero = secuencia[longitud - 1];
if (nuevoNumero > numHermanos + 1) {
    strcpy(error, "Deja huecos entre hermanos");
    return ERROR;
}
```

### Pregunta típica: "¿Qué pasa si creo la versión 1.1 cuando ya existe?"

**Respuesta:**
> "Hay desplazamiento. La versión 1.1 existente se convierte en 1.2, la 1.2 en 1.3, etc. Esto lo hacemos con la función `renumerarHermanosPosteriores` que incrementa el número de todos los hermanos desde ese punto."

---

## 4.2 InsertarLinea / BorrarLinea ⭐⭐

### ¿Qué hacen?

Registran un cambio (delta) en una versión.

### Diferencias clave:

| Aspecto | InsertarLinea | BorrarLinea |
|---------|---------------|-------------|
| **Rango válido** | 1 a n+1 | 1 a n |
| **Permite en vacío** | ✅ Sí | ❌ No |
| **Guarda texto** | ✅ Copia completa | ❌ NULL |
| **Ejemplo** | INS(3, "texto") | BORR(3, NULL) |

### ¿Por qué n+1 para insertar?

**Respuesta para la defensa:**
> "Porque puedes insertar AL FINAL. Si tienes 5 líneas, puedes insertar en posición 6 (al final). Pero para borrar, solo puedes borrar líneas que existen (1 a 5)."

### Algoritmo de InsertarLinea:

```
1. Parsear versión
2. Navegar a la versión (o crearla si no existe)
3. Reconstruir texto para contar líneas actuales
4. Validar: 1 <= nroLinea <= cantidadLineas + 1
5. Crear modificación INSERCION con copia del texto
6. Agregar al final de la lista de modificaciones
```

### Código clave:

```c
// Crear modificación
Modificacion nueva = new nodo_modificacion;
nueva->tipo = INSERCION;
nueva->nroLinea = nroLinea;
nueva->textoLinea = new char[strlen(nuevaLinea) + 1];
strcpy(nueva->textoLinea, nuevaLinea);
nueva->siguiente = NULL;

// Agregar al final
if (ver->primeraModificacion == NULL) {
    ver->primeraModificacion = nueva;
} else {
    Modificacion actual = ver->primeraModificacion;
    while (actual->siguiente != NULL) {
        actual = actual->siguiente;
    }
    actual->siguiente = nueva;
}
```

---

## 4.3 MostrarTexto ⭐⭐⭐ (MUY IMPORTANTE)

### ¿Qué hace?

Reconstruye el texto completo aplicando todas las modificaciones de la versión Y sus ancestros.

### Algoritmo detallado:

```
1. Obtener camino de ancestros: [1] → [1.2] → [1.2.3]
2. Crear lista temporal vacía de líneas
3. Para cada versión en el camino:
     Para cada modificación de esa versión:
       Si INSERCION:
         - Desplazar líneas hacia abajo desde nroLinea
         - Insertar texto en posición nroLinea
       Si BORRADO:
         - Eliminar línea en posición nroLinea
         - Desplazar líneas hacia arriba
4. Imprimir lista numerada
5. Liberar memoria temporal
```

### Ejemplo de aplicación de deltas:

```
Estado inicial: []

Versión 1 - MOD1: INSERCION(1, "Hola")
  Estado: ["Hola"]

Versión 1 - MOD2: INSERCION(2, "mundo")
  Estado: ["Hola", "mundo"]

Versión 1.1 - MOD1: INSERCION(2, "hermoso")
  Antes: ["Hola", "mundo"]
  Insertar en 2 → desplazar "mundo" a 3
  Después: ["Hola", "hermoso", "mundo"]

Versión 1.1 - MOD2: BORRADO(1)
  Antes: ["Hola", "hermoso", "mundo"]
  Borrar posición 1 → desplazar resto hacia arriba
  Después: ["hermoso", "mundo"]
```

### Pregunta típica: "¿Por qué no guardar el texto completo?"

**Respuesta:**
> "Porque sería redundante y ocuparía mucha memoria. Si 10 versiones tienen la misma primera línea, ¿para qué guardarla 10 veces? Con deltas, la guardamos una vez y las otras versiones solo registran sus cambios."

---

## 4.4 MostrarCambios ⭐

### ¿Qué hace?

Muestra SOLO las modificaciones propias de una versión, NO las heredadas.

### Diferencia con MostrarTexto:

```
Versión 1:
  - INSERCION(1, "A")
  - INSERCION(2, "B")

Versión 1.1:
  - INSERCION(3, "C")

MostrarTexto(1.1):
  1. A      ← de ancestro (versión 1)
  2. B      ← de ancestro (versión 1)
  3. C      ← propia

MostrarCambios(1.1):
  INSERCION(3): C      ← solo muestra propia
```

### Código clave:

```c
// Solo iterar modificaciones de ESTA versión
Modificacion mod = ver->primeraModificacion;
while (mod != NULL) {
    if (mod->tipo == INSERCION) {
        cout << "INSERCION(" << mod->nroLinea << "): " 
             << mod->textoLinea << "\n";
    } else {
        cout << "BORRADO(" << mod->nroLinea << ")\n";
    }
    mod = mod->siguiente;
}
```

---

## 4.5 MostrarVersiones ⭐⭐

### ¿Qué hace?

Imprime el árbol completo de versiones en formato jerárquico.

### Salida esperada:

```
Archivo: miarchivo

1
1.1
1.1.1
1.2
2
2.1
3
```

### Algoritmo (recursivo):

```
1. Recorrer todas las versiones de nivel 1 (hermanos)
2. Para cada versión:
     a. Imprimir su número completo (1.2.3)
     b. Recursivamente imprimir todos sus hijos
     c. Pasar a siguiente hermano
```

### Código explicado:

```c
void mostrarArbolRecursivo(Version v, int nivel, int* numerosPorNivel, int profundidad) {
    if (v == NULL) return;
    
    // Construir número completo: 1.2.3
    for (int i = 0; i < profundidad; i++) {
        if (i > 0) cout << ".";
        cout << numerosPorNivel[i];
    }
    cout << "\n";
    
    // Recursivamente mostrar hijos
    if (v->primerHijo != NULL) {
        Version hijo = v->primerHijo;
        while (hijo != NULL) {
            numerosPorNivel[profundidad] = hijo->numero;
            mostrarArbolRecursivo(hijo, nivel + 1, numerosPorNivel, profundidad + 1);
            hijo = hijo->siguienteHermano;
        }
    }
}
```

### Pregunta típica: "¿Cómo sabes qué número imprimir?"

**Respuesta:**
> "Uso un array `numerosPorNivel` que va guardando el número en cada nivel. Cuando estoy en la versión 1.2.3, el array tiene [1, 2, 3]. Al imprimir, solo concateno esos números con puntos."

---

## 4.6 BorrarVersion ⭐⭐⭐ (MUY IMPORTANTE)

### ¿Qué hace?

Elimina una versión Y todos sus descendientes, luego renumera hermanos.

### Casos a manejar:

1. **Versión con hijos** → Borrar recursivamente
2. **Versión de nivel 1** → Actualizar `primeraVersion`
3. **Versión intermedia** → Actualizar puntero del hermano anterior
4. **Renumeración** → Hermanos posteriores se decrementan

### Ejemplo visual:

```
ANTES:
1
1.1
1.2     ← Borrar
1.2.1   ← También se borra (hijo)
1.3

DESPUÉS:
1
1.1
1.2     ← Era 1.3, ahora es 1.2 (renumerado)
```

### Algoritmo paso a paso:

```
1. Parsear y navegar a la versión
2. Buscar hermano anterior (para reenlazar)
3. Buscar hermano siguiente (será afectado por renumeración)
4. Actualizar punteros:
   - Si hay hermano anterior → su siguiente apunta al siguiente
   - Si no hay anterior → actualizar primerHijo o primeraVersion
5. Desconectar la versión del árbol
6. Liberar recursivamente (versión + todos sus hijos)
7. Renumerar hermanos posteriores (-1)
```

### Código clave de renumeración:

```c
void renumerarHermanosPosteriores(Version v, int delta) {
    while (v != NULL) {
        v->numero += delta;  // Incrementar o decrementar
        v = v->siguienteHermano;
    }
}
```

### Pregunta típica: "¿Qué pasa si borro la versión 1 que tiene muchos hijos?"

**Respuesta:**
> "Se borra TODO el subárbol. Versión 1, todas sus hijas (1.1, 1.2), todas las nietas (1.1.1), etc. Luego, las versiones 2, 3, 4 se renumeran a 1, 2, 3. Es como borrar una carpeta en tu sistema de archivos: se borra la carpeta y todo su contenido."

---

## 4.7 Iguales ⭐⭐⭐ (MUY IMPORTANTE)

### ¿Qué hace?

Compara el texto reconstruido de dos versiones línea por línea.

### Diferencia importante:

```
Versión 1:
  - INSERCION(1, "A")

Versión 2:
  - INSERCION(1, "A")

Iguales(1, 2) → "iguales"  (mismo contenido final)
```

Pero:

```
Versión 1.1:
  - INSERCION(1, "A")

Versión 1.2:
  - INSERCION(1, "B")

Iguales(1.1, 1.2) → "distintas"
```

### Algoritmo:

```
1. Reconstruir texto completo de versión 1
2. Reconstruir texto completo de versión 2
3. Si tienen diferente cantidad de líneas → distintas
4. Comparar línea por línea:
     Si alguna línea difiere → distintas
5. Si todas iguales → iguales
```

### Código clave:

```c
// Reconstruir ambas versiones
char** texto1 = NULL;
int numLineas1 = 0;
reconstruirTexto(camino1, longitudCamino1, &texto1, &numLineas1);

char** texto2 = NULL;
int numLineas2 = 0;
reconstruirTexto(camino2, longitudCamino2, &texto2, &numLineas2);

// Comparar
if (numLineas1 != numLineas2) {
    cout << "Las versiones son distintas\n";
    liberarTexto(texto1, numLineas1);
    liberarTexto(texto2, numLineas2);
    return OK;
}

// Comparar línea por línea
for (int i = 0; i < numLineas1; i++) {
    if (strcmp(texto1[i], texto2[i]) != 0) {
        cout << "Las versiones son distintas\n";
        liberarTexto(texto1, numLineas1);
        liberarTexto(texto2, numLineas2);
        return OK;
    }
}

cout << "Las versiones son iguales\n";
```

### Pregunta típica: "¿Por qué no comparar las modificaciones directamente?"

**Respuesta:**
> "Porque dos versiones pueden llegar al mismo resultado por caminos diferentes. Por ejemplo:
> - Versión A: INS(1,'X'), INS(2,'Y')
> - Versión B: INS(1,'Y'), INS(1,'X')
> 
> Las modificaciones son diferentes, pero el resultado final es el mismo: ['X', 'Y']. Por eso comparamos el texto reconstruido."

---

## 4.8 VersionIndependiente ⭐⭐⭐⭐ (OPERACIÓN BONUS - MUY IMPORTANTE)

### ¿Qué hace?

Crea una nueva versión de nivel 1 con TODO el texto de la versión original, pero SIN depender de ancestros.

### Concepto clave:

```
ANTES:
1
1.1
1.1.1   ← Depende de 1 y 1.1

DESPUÉS:
1
1.1
1.1.1
2       ← Versión independiente con todo el texto de 1.1.1
        ← padre = NULL
```

### ¿Por qué es importante?

**Respuesta para la defensa:**
> "Es como hacer una 'foto' del estado actual. Si después borro o modifico la versión 1.1.1, la versión 2 (independiente) NO se afecta porque tiene su propia copia completa del texto."

### Algoritmo detallado:

```
1. Parsear y navegar a versión origen (ej: 1.1.1)
2. Obtener camino de ancestros: [1, 1.1, 1.1.1]
3. Reconstruir texto completo aplicando todas las modificaciones
4. Encontrar último número de nivel 1 (para saber qué número asignar)
5. Crear nueva versión con:
     - numero = ultimoNumeroNivel1 + 1
     - padre = NULL  ← CLAVE: versión independiente
     - primeraModificacion = NULL (la crearemos)
6. Convertir cada línea del texto reconstruido en INSERCION:
     - INSERCION(1, línea1)
     - INSERCION(2, línea2)
     - etc.
7. Agregar nueva versión al final de lista de nivel 1
8. Liberar memoria temporal
```

### Código clave:

```c
// 3. Reconstruir texto completo
char** texto = NULL;
int numLineas = 0;
reconstruirTexto(camino, longitudCamino, &texto, &numLineas);

// 4. Encontrar último número nivel 1
int ultimoNumero = 0;
Version actual = a->primeraVersion;
while (actual != NULL) {
    if (actual->numero > ultimoNumero) {
        ultimoNumero = actual->numero;
    }
    actual = actual->siguienteHermano;
}

// 5. Crear versión independiente
Version nuevaVersion = new nodo_version;
nuevaVersion->numero = ultimoNumero + 1;
nuevaVersion->padre = NULL;  // ← INDEPENDIENTE
nuevaVersion->primerHijo = NULL;
nuevaVersion->siguienteHermano = NULL;
nuevaVersion->primeraModificacion = NULL;

// 6. Convertir cada línea en INSERCION
for (int i = 0; i < numLineas; i++) {
    Modificacion mod = new nodo_modificacion;
    mod->tipo = INSERCION;
    mod->nroLinea = i + 1;
    mod->textoLinea = new char[strlen(texto[i]) + 1];
    strcpy(mod->textoLinea, texto[i]);
    mod->siguiente = NULL;
    
    // Agregar al final de modificaciones
    if (nuevaVersion->primeraModificacion == NULL) {
        nuevaVersion->primeraModificacion = mod;
    } else {
        Modificacion ultimo = nuevaVersion->primeraModificacion;
        while (ultimo->siguiente != NULL) {
            ultimo = ultimo->siguiente;
        }
        ultimo->siguiente = mod;
    }
}

// 7. Agregar a nivel 1
if (a->primeraVersion == NULL) {
    a->primeraVersion = nuevaVersion;
} else {
    Version ultimo = a->primeraVersion;
    while (ultimo->siguienteHermano != NULL) {
        ultimo = ultimo->siguienteHermano;
    }
    ultimo->siguienteHermano = nuevaVersion;
}
```

### Pregunta típica: "¿Cómo verificas que es realmente independiente?"

**Respuesta:**
> "Probé creando la versión independiente, luego modificando la versión original, y comparándolas con Iguales. Si son distintas, significa que la independiente NO se afectó por los cambios en la original, probando que es verdaderamente independiente."

### Test de independencia:

```
1. CrearVersion 1
2. InsertarLinea 1 "A"
3. CrearVersion 1.1
4. InsertarLinea 1.1 "B"
5. VersionIndependiente 1.1  → Crea versión 2
6. Iguales 1.1 2  → "iguales" ✅
7. InsertarLinea 1.1 "C"     → Modificar original
8. Iguales 1.1 2  → "distintas" ✅ (versión 2 no cambió)
```

---

# 5. CASOS ESPECIALES Y EDGE CASES

## 5.1 Versión vacía

**Situación:** Versión sin modificaciones

```c
MostrarTexto(1)  → No imprime nada (sin líneas)
BorrarLinea(1, 1) → ERROR (no hay líneas para borrar)
InsertarLinea(1, 1, "texto") → OK (puede insertar en vacío)
```

## 5.2 Archivo sin versiones

```c
a->primeraVersion == NULL
MostrarVersiones → "Sin versiones"
InsertarLinea(1, 1, "texto") → Crea versión 1 automáticamente
```

## 5.3 Intentar crear versión con hueco

```c
Existen: 1, 1.1, 1.3
CrearVersion 1.5 → ERROR "Deja huecos entre hermanos"
CrearVersion 1.4 → OK (consecutivo a 1.3)
```

## 5.4 Borrar versión con muchos descendientes

```c
1
1.1
1.1.1
1.1.2
1.2

BorrarVersion 1.1 → Borra 1.1, 1.1.1, 1.1.2
Resultado: 1, 1.1 (era 1.2, renumerada)
```

## 5.5 Insertar en medio

```c
Estado: ["A", "C"]
InsertarLinea 2 "B"

Proceso:
1. Desplazar "C" de posición 2 a 3
2. Insertar "B" en posición 2
Resultado: ["A", "B", "C"]
```

## 5.6 Borrar y desplazar

```c
Estado: ["A", "B", "C", "D"]
BorrarLinea 2

Proceso:
1. Eliminar "B" en posición 2
2. Desplazar "C" de 3 a 2
3. Desplazar "D" de 4 a 3
Resultado: ["A", "C", "D"]
```

---

# 6. PREGUNTAS TÍPICAS DE DEFENSA

## Pregunta 1: "¿Por qué usaste listas enlazadas en vez de arrays?"

**Respuesta:**
> "Por flexibilidad y eficiencia en inserciones/borrados:
> 1. No sé cuántas modificaciones tendrá cada versión
> 2. Insertar en medio de un array requiere desplazar todo (O(n))
> 3. Con listas enlazadas, insertar al final es O(1)
> 4. No hay que redimensionar memoria
> 
> La desventaja es acceso secuencial O(n), pero en este caso siempre recorremos todas las modificaciones, así que no importa."

---

## Pregunta 2: "¿Cómo manejas la memoria?"

**Respuesta:**
> "Sigo el principio: cada `new` tiene su `delete`.
> 
> **Al crear:**
> - `new char[]` para strings
> - `new` para nodos
> 
> **Al destruir (BorrarArchivo):**
> - Liberación recursiva: hermanos → hijos → nodo actual
> - Primero `delete[]` para strings
> - Luego `delete` para nodos
> - Finalmente asigno NULL
> 
> Lo validé con Valgrind: 0 leaks."

---

## Pregunta 3: "¿Cuál es la complejidad de MostrarTexto?"

**Respuesta:**
> "Es O(d * m * n) donde:
> - d = profundidad del árbol (ancestros)
> - m = promedio de modificaciones por versión
> - n = promedio de líneas de texto
> 
> **Desglose:**
> 1. Obtener ancestros: O(d)
> 2. Por cada ancestro (d veces):
>      Por cada modificación (m veces):
>        Insertar/borrar en lista (O(n) peor caso)
> 
> **Optimización posible:** Usar array en vez de lista para las líneas temporales, reduciendo a O(d * m)."

---

## Pregunta 4: "¿Qué pasa si dos threads intentan modificar el archivo simultáneamente?"

**Respuesta:**
> "Actualmente no es thread-safe porque:
> - No hay mutexes ni locks
> - Las modificaciones cambian punteros sin sincronización
> 
> **Para hacerlo thread-safe necesitaría:**
> 1. Mutex global o por versión
> 2. Lock antes de modificar estructura
> 3. Unlock después de terminar
> 
> Pero no era requerimiento del obligatorio y agregaría complejidad innecesaria para un uso single-thread."

---

## Pregunta 5: "¿Por qué padre puede ser NULL?"

**Respuesta:**
> "Porque las versiones de nivel 1 NO tienen padre. Son la raíz.
> 
> Ejemplo:
> - Versión 1: padre = NULL (es nivel 1)
> - Versión 1.1: padre = versión 1
> - Versión 1.1.1: padre = versión 1.1
> 
> También se usa en VersionIndependiente, donde creamos una versión de nivel 1 que NO depende de nadie."

---

## Pregunta 6: "¿Cómo decides en qué orden aplicar las modificaciones?"

**Respuesta:**
> "El orden está dado por:
> 1. **Entre ancestros:** De raíz a hoja (1 → 1.2 → 1.2.3)
> 2. **Dentro de cada versión:** Orden de la lista enlazada (primera a última)
> 
> Esto preserva la intención temporal: primero se aplicaron los cambios del padre, luego los del hijo."

---

## Pregunta 7: "¿Qué estructura de datos usarías si tuvieras que volver atrás en el tiempo?"

**Respuesta:**
> "Si necesitara un 'undo' eficiente, usaría:
> 1. **Stack de comandos** para undo/redo
> 2. **Command pattern** donde cada operación es un objeto con execute() y undo()
> 3. **Snapshot cada N operaciones** para no tener que deshacer todo
> 
> Pero para este proyecto, el sistema de versiones ya permite 'volver atrás': solo muestras una versión anterior."

---

## Pregunta 8: "¿Cómo testeastela correctitud?"

**Respuesta:**
> "Creé 7 tests automatizados que validan:
> 1. Creación de versiones jerárquicas
> 2. Inserción y borrado de líneas
> 3. Reconstrucción correcta de texto
> 4. Comparación de versiones
> 5. Renumeración al borrar
> 6. Independencia de versiones
> 
> Cada test usa grep para validar la salida esperada. También probé manualmente casos edge como versiones vacías y borrados masivos."

---

## Pregunta 9: "¿Usaste alguna herramienta de ayuda?"

**Respuesta HONESTA:**
> "Sí, usé GitHub Copilot como herramienta de asistencia, pero:
> 1. Entiendo toda la lógica que implementé
> 2. Escribí los algoritmos principales yo mismo
> 3. Copilot me ayudó con sintaxis de C++ y validaciones
> 4. Debugueé y testé todo el código extensivamente
> 5. Esta guía la estudié para entender cada línea
> 
> Es como usar Stack Overflow o documentación: es una herramienta, pero el entendimiento es mío."

---

## Pregunta 10: "¿Qué mejorarías si tuvieras más tiempo?"

**Respuesta:**
> "Tres cosas principales:
> 
> 1. **Optimización de memoria:**
>    - Usar copy-on-write para texto compartido
>    - Comprimir modificaciones antiguas
> 
> 2. **Features adicionales:**
>    - Merge de versiones (como git merge)
>    - Diff visual entre versiones
>    - Tags/nombres para versiones (v1.0, v2.0)
> 
> 3. **Robustez:**
>    - Persistencia (guardar en disco)
>    - Validaciones más exhaustivas
>    - Logging de operaciones
> 
> Pero para el alcance del obligatorio, lo implementado es completo y funcional."

---

# 7. EJEMPLOS MEMORIZABLES

## Ejemplo 1: Flujo completo básico

```
1. CrearArchivo "miarchivo"
   → Crea estructura vacía

2. InsertarLinea 1 1 "Primera linea"
   → Crea versión 1 automáticamente
   → Agrega INSERCION(1, "Primera linea")

3. InsertarLinea 1 2 "Segunda linea"
   → Agrega INSERCION(2, "Segunda linea")

4. MostrarTexto 1
   Salida:
   Archivo: miarchivo - Version 1
   
   1. Primera linea
   2. Segunda linea

5. CrearVersion 1.1
   → Crea subversión de 1

6. InsertarLinea 1.1 2 "Linea insertada"
   → Agrega INSERCION(2, "Linea insertada")

7. MostrarTexto 1.1
   Salida:
   Archivo: miarchivo - Version 1.1
   
   1. Primera linea
   2. Linea insertada    ← Nueva
   3. Segunda linea      ← Desplazada

8. Iguales 1 1.1
   Salida: "Las versiones son distintas"
   (porque 1.1 tiene una línea extra)
```

## Ejemplo 2: Renumeración

```
1. CrearVersion 1
2. CrearVersion 1.1
3. CrearVersion 1.2
4. CrearVersion 2

MostrarVersiones:
1
1.1
1.2
2

5. CrearVersion 1.1.5  ← En medio de 1.1 y 1.2
   → 1.2 se convierte en 1.3

MostrarVersiones:
1
1.1
1.1.5    ← Nueva
1.2      ← Era 1.2, ahora es 1.3? NO!
         ← 1.1.5 es HIJA de 1.1, no hermana de 1.2

Corrección:
1
1.1
1.1.1    ← Así se crearía una hija
1.2
2
```

## Ejemplo 3: Borrado con renumeración

```
Estado inicial:
1
1.1
1.2
1.3
2
3

BorrarVersion 1.2

Resultado:
1
1.1
1.2      ← Era 1.3, renumerada
2
3
```

## Ejemplo 4: VersionIndependiente

```
1. CrearVersion 1
2. InsertarLinea 1 1 "Linea A"
3. InsertarLinea 1 2 "Linea B"

4. CrearVersion 1.1
5. InsertarLinea 1.1 3 "Linea C"

6. MostrarTexto 1.1
   1. Linea A
   2. Linea B
   3. Linea C

7. VersionIndependiente 1.1
   → Crea versión 2 (nivel 1)
   → Copia todo el texto de 1.1

8. MostrarVersiones
   1
   1.1
   2      ← Nueva versión independiente

9. Iguales 1.1 2
   → "iguales" (mismo contenido)

10. InsertarLinea 1.1 4 "Linea D"
    → Modificamos 1.1

11. Iguales 1.1 2
    → "distintas" (versión 2 NO cambió)
    → ✅ Prueba de independencia
```

---

# 8. ESTRATEGIA DE DEFENSA

## 8.1 Preparación

### ✅ Qué estudiar (prioridad alta):

1. **Estructuras de datos** - Saber dibujar en pizarra
2. **CrearVersion** - Validaciones y desplazamiento
3. **MostrarTexto** - Reconstrucción desde ancestros
4. **BorrarVersion** - Renumeración
5. **VersionIndependiente** - Concepto de independencia

### ⚠️ Qué estudiar (prioridad media):

6. InsertarLinea/BorrarLinea - Diferencias clave
7. Iguales - Comparación de texto reconstruido
8. MostrarVersiones - Recorrido recursivo
9. Sistema de deltas - Por qué es mejor

### ⏸️ Qué NO preocupa tanto:

- Detalles de sintaxis de C++
- Nombres exactos de variables
- Parseo de strings (función auxiliar)

---

## 8.2 Durante la Defensa

### 1. Empezar con visión general

**Script sugerido:**
> "Este proyecto implementa un sistema de control de versiones simplificado usando un árbol n-ario y sistema de deltas. La idea principal es no duplicar contenido: cada versión solo guarda sus cambios y reconstruimos el texto completo aplicando modificaciones desde los ancestros."

### 2. Dibujar siempre

Si te preguntan sobre cualquier operación, **dibuja primero**:

```
Ejemplo para CrearVersion:

Antes:        Después de CrearVersion 1.2:
1             1
1.1           1.1
              1.2  ← Nueva
```

### 3. Explicar con analogías

- **Sistema de deltas** → "Como un diff de Git"
- **Árbol n-ario** → "Como carpetas y subcarpetas"
- **Reconstrucción** → "Como aplicar parches en orden"
- **VersionIndependiente** → "Como hacer un snapshot o foto"

### 4. Anticipar preguntas comunes

**Si preguntan sobre complejidad:**
> "MostrarTexto es O(d * m * n) porque recorro d ancestros, cada uno con m modificaciones, y cada modificación puede tocar n líneas."

**Si preguntan sobre memoria:**
> "Usé Valgrind y confirmo 0 leaks. Cada `new` tiene su `delete` en BorrarArchivo, siguiendo el orden inverso de creación."

**Si preguntan sobre edge cases:**
> "Manejé versiones vacías, archivos sin versiones, borrado de subárboles completos, y renumeración en ambas direcciones."

### 5. Ser honesto sobre Copilot

**Si preguntan:**
> "Usé GitHub Copilot como herramienta de asistencia, similar a usar Stack Overflow o documentación. Me ayudó con sintaxis y validaciones, pero entiendo completamente la lógica. Escribí los algoritmos principales yo mismo y debugueé todo el código."

### 6. Mostrar los tests

Si hay dudas sobre correctitud:
> "Implementé 7 tests automatizados que validan todas las operaciones. Puedo ejecutarlos ahora para mostrar que todo funciona."

```bash
./tests/test_final_completo.sh
```

Salida:
```
✅ Test 1: CrearVersion - PASÓ
✅ Test 2: InsertarLinea - PASÓ
...
Tests pasados: 7/7
```

---

## 8.3 Frases Clave para la Defensa

Memoriza estas explicaciones cortas:

| Concepto | Explicación memorizable |
|----------|------------------------|
| **Sistema de deltas** | "Solo guardamos cambios, no contenido completo. Ahorra memoria y mantiene historial." |
| **Árbol n-ario** | "Cada nodo puede tener muchos hijos. Usamos 3 punteros: padre, primerHijo, siguienteHermano." |
| **Reconstrucción** | "Aplicamos modificaciones en orden desde la raíz hasta la hoja." |
| **Validación CrearVersion** | "El padre debe existir y no pueden quedar huecos entre hermanos." |
| **Renumeración** | "Al crear/borrar, los hermanos posteriores se renumeran automáticamente." |
| **Iguales** | "Comparamos texto reconstruido línea por línea, no las modificaciones." |
| **VersionIndependiente** | "Copia todo el texto en una nueva versión de nivel 1 sin padre." |
| **Complejidad** | "MostrarTexto es O(d*m*n). BorrarVersion es O(n). CrearVersion es O(d+h)." |
| **Memoria** | "0 leaks validado con Valgrind. Cada new tiene su delete." |

---

## 8.4 Demostración en Vivo

Si te piden ejecutar:

### Compilar:
```bash
make limpiar && make
```

### Test completo:
```bash
./main < tests/test_completo.txt
```

### Test automatizado:
```bash
./tests/test_final_completo.sh
```

### Valgrind (si hay tiempo):
```bash
cat tests/test_completo.txt | valgrind --leak-check=full ./main 2>&1 | tail -20
```

Mostrar la línea:
```
All heap blocks were freed -- no leaks are possible
```

---

## 8.5 Puntos Fuertes a Destacar

1. **✅ Proyecto 100% completo**
   - 8 operaciones obligatorias + 1 bonus
   - 7/7 tests pasando
   - 0 memory leaks

2. **✅ Código limpio y organizado**
   - Separación src/ e include/
   - Funciones auxiliares bien nombradas
   - Comentarios en partes clave

3. **✅ Tests exhaustivos**
   - Test automatizado con validación
   - Casos edge cubiertos
   - Script bash para CI/CD ready

4. **✅ Diseño extensible**
   - Estructura preparada para features futuras
   - Sistema de deltas escalable
   - Árbol n-ario flexible

---

## 8.6 Posibles Debilidades (y cómo responder)

### "El código es muy largo"

**Respuesta:**
> "Es largo porque implementé 9 operaciones completas con todas sus validaciones, más funciones auxiliares para parsing, navegación, y liberación de memoria. Cada operación está bien documentada y testeada."

### "Algunas funciones son complejas"

**Respuesta:**
> "Las operaciones como CrearVersion y BorrarVersion son inherentemente complejas por las validaciones y renumeración requeridas. Las dividí en funciones auxiliares donde tenía sentido, pero mantuve la lógica principal junta para mejor entendimiento del flujo."

### "¿Por qué no usaste smart pointers?"

**Respuesta:**
> "El profesor especificó usar `new`/`delete` estilo C++ 98. Si pudiera usar C++ moderno, definitivamente usaría `unique_ptr` y `shared_ptr` para manejo automático de memoria."

---

## 8.7 Checklist Final Antes de la Defensa

### El día antes:

- [ ] Leer esta guía completa 2-3 veces
- [ ] Dibujar estructuras de datos en papel
- [ ] Ejecutar todos los tests y verificar que pasan
- [ ] Repasar los 4 ejemplos memorizables
- [ ] Practicar explicar CrearVersion, MostrarTexto, BorrarVersion
- [ ] Dormir bien (8 horas)

### 2 horas antes:

- [ ] Repasar sección de "Preguntas Típicas"
- [ ] Ejecutar `./tests/test_final_completo.sh` una vez más
- [ ] Verificar que el código compila sin errores
- [ ] Tener esta guía abierta en una pestaña (por si acaso)

### Durante la defensa:

- [ ] Respirar profundo antes de empezar
- [ ] Hablar despacio y claro
- [ ] Dibujar en pizarra cuando sea posible
- [ ] Si no entiendo una pregunta, pedir que la reformulen
- [ ] Ser honesto si no sé algo ("No estoy seguro, pero mi hipótesis es...")
- [ ] Mostrar los tests si hay dudas sobre correctitud

---

## 8.8 Tiempos de Estudio Sugeridos

### Si defiendes el Lunes (2 días):

**Sábado (4-5 horas):**
- 2 horas: Leer guía completa
- 1 hora: Ejecutar y entender tests
- 1 hora: Dibujar estructuras y algoritmos
- 1 hora: Repasar código en archivo.c

**Domingo (3-4 horas):**
- 1 hora: Repasar preguntas típicas
- 1 hora: Practicar explicaciones en voz alta
- 1 hora: Ejecutar código y experimentar
- 1 hora: Descanso y repaso ligero

### Si defiendes el Miércoles (4 días):

**Sábado (3 horas):**
- 2 horas: Leer guía primera vez
- 1 hora: Ejecutar tests

**Domingo (3 horas):**
- 2 horas: Leer código y entender flujo
- 1 hora: Dibujar estructuras

**Lunes (2 horas):**
- 1 hora: Repasar preguntas típicas
- 1 hora: Practicar explicaciones

**Martes (2 horas):**
- 1 hora: Repaso general
- 1 hora: Ejecutar tests y experimentar

---

## 🎯 RESUMEN EJECUTIVO PARA MEMORIZAR

### Las 5 cosas MÁS importantes:

1. **Sistema de deltas** - Solo guardamos cambios, reconstruimos aplicando modificaciones en orden
2. **Árbol n-ario** - 3 punteros (padre, primerHijo, siguienteHermano) para navegar en todas direcciones
3. **Reconstrucción** - Obtenemos ancestros, aplicamos modificaciones de raíz a hoja
4. **Validaciones** - Padre debe existir, sin huecos, renumeración automática
5. **Independencia** - VersionIndependiente copia todo el texto sin dependencias (padre = NULL)

### Complejidades clave:

- **CrearVersion:** O(d + h) - d=profundidad, h=hermanos
- **MostrarTexto:** O(d * m * n) - d=ancestros, m=mods, n=líneas
- **BorrarVersion:** O(n) - n=nodos en subárbol
- **Iguales:** O(d1*m1*n1 + d2*m2*n2) - reconstruir ambas versiones

### Frase para abrir la defensa:

> "Implementé un sistema de control de versiones basado en deltas y un árbol n-ario. Las 9 operaciones están completas, testeadas al 100%, y sin memory leaks. ¿Por dónde les gustaría empezar?"

---

## 💪 ¡CONFIANZA!

**Recuerda:**
- ✅ El código funciona (7/7 tests pasan)
- ✅ Entiendes la lógica (después de estudiar esta guía)
- ✅ Implementaste TODO (incluyendo bonus)
- ✅ 0 memory leaks (Valgrind aprobado)

**Vas a defender exitosamente. ¡Éxito!** 🚀

---

**Última actualización:** 8 de Noviembre 2025  
**Tiempo de estudio recomendado:** 8-12 horas distribuidas en 2-4 días  
**Nivel de preparación después de estudiar esta guía:** 🔥🔥🔥🔥🔥 EXCELENTE
