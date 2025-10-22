# Tests - Fase 1 ✅

Archivos de prueba para verificar las funciones implementadas.

## 🚀 Ejecución Rápida

### Opción 1: Script automatizado (RECOMENDADO)
```bash
# En WSL
cd /mnt/d/Escritorio/CODE/EDA/eda-ob2025
bash run_tests.sh
```

### Opción 2: Tests individuales
```bash
# Compilar primero
make limpiar && make

# Ejecutar test específico
cat tests/test_minimo.txt | ./main
cat tests/test_simple.txt | ./main
cat entrada | ./main
```

### Opción 3: Con Valgrind (verificar memoria)
```bash
cat entrada | valgrind --leak-check=full ./main
```

## 📁 Archivos de Test

### ✅ `test_crear_archivo.txt`
**Descripción:** Test más básico - solo crea archivo
```
miarchivo
salir
```
**Uso:** Verificar que el proyecto compila y ejecuta.

---

### ✅ `test_minimo.txt`
**Descripción:** Test mínimo funcional
- Crear archivo
- Insertar 1 línea
- Mostrar texto
- Salir

**Comandos:**
```
miarchivo
InsertarLinea(1, Hola mundo, 1)
MostrarTexto(1)
salir
```

**Salida esperada:**
```
Archivo: miarchivo - Version 1

1. Hola mundo
```

---

### ✅ `test_insertar.txt`
**Descripción:** Test de InsertarLinea
- Insertar 3 líneas
- Mostrar resultado

**Comandos:**
```
miarchivo
InsertarLinea(1, Primera linea, 1)
InsertarLinea(1, Segunda linea, 2)
InsertarLinea(1, Tercera linea, 3)
MostrarTexto(1)
salir
```

---

### ✅ `test_simple.txt`
**Descripción:** Test completo de InsertarLinea y MostrarTexto
- Insertar 3 líneas en versión 1
- **Insertar en medio** (posición 2) → verifica sistema de deltas
- Crear versión 2 independiente
- Mostrar ambas versiones → verifica independencia

**Salida clave:**
```
Archivo: prueba - Version 1

1. Primera línea del archivo
2. Línea insertada en medio       ← Insertada después
3. Segunda línea agregada          ← Se corrió de 2 a 3
4. Tercera línea de texto
```

---

### ✅ `test_mostrar_texto.txt`
**Descripción:** Igual que test_simple.txt
- Enfocado en reconstrucción de texto desde deltas

---

### ✅ `test_borrar_linea.txt`
**Descripción:** Test de BorrarLinea
- Insertar 5 líneas
- Borrar línea 3
- Borrar línea 1
- Borrar línea 3 (ahora es otra línea)
- Mostrar resultado después de cada borrado

**Comandos clave:**
```
InsertarLinea(1, Primera linea, 1)
...
InsertarLinea(1, Quinta linea, 5)
MostrarTexto(1)        # 5 líneas

BorrarLinea(1, 3)      # Borra "Tercera linea"
MostrarTexto(1)        # 4 líneas

BorrarLinea(1, 1)      # Borra "Primera linea"
MostrarTexto(1)        # 3 líneas
```

**Demuestra:** Sistema de deltas funciona para borrado

---

### ✅ `test_completo.txt`
**Descripción:** Test completo de todas las operaciones
- InsertarLinea en versión 1
- BorrarLinea
- Crear versión 2
- Mostrar ambas versiones

**Uso:** Prueba de integración completa

---

### ✅ `entrada`
**Descripción:** Archivo principal de demostración - INCLUYE TODAS LAS FUNCIONES
- InsertarLinea (múltiples veces)
- MostrarTexto (múltiples versiones)
- BorrarLinea (múltiples veces)
- Crear versión 2
- **BorrarArchivo** (prueba liberación de memoria)

**Comandos:** 16 operaciones + salir

**IMPORTANTE:** Este archivo ahora incluye `BorrarArchivo` antes de `salir` para probar liberación de memoria.

---

## 📊 Resultados Esperados

### Test exitoso se ve así:
```
$ cat tests/test_minimo.txt | ./main

Ingrese el nombre del archivo a versionar:
Ingrese el nombre de la version:
Ingrese la linea:
Ingrese el numero de linea de la version 1:

Archivo: miarchivo - Version 1

1. Hola mundo
```

### Test con bucle infinito (ERROR):
```
$ cat tests/test_sin_salir.txt | ./main
...
(se queda esperando input y no termina)
```

**Solución:** TODOS los archivos ahora tienen `salir` al final ✅

---

## ✅ Status de los Tests

| Archivo | Estado | Descripción |
|---------|--------|-------------|
| `test_crear_archivo.txt` | ✅ ARREGLADO | Tenía contenido vacío → Ahora tiene `salir` |
| `test_minimo.txt` | ✅ OK | Ya tenía `salir` |
| `test_insertar.txt` | ✅ ARREGLADO | Estaba vacío → Ahora tiene contenido + `salir` |
| `test_simple.txt` | ✅ OK | Ya tenía `salir` |
| `test_mostrar_texto.txt` | ✅ OK | Ya tenía `salir` |
| `test_borrar_linea.txt` | ✅ OK | Ya tenía `salir` |
| `test_completo.txt` | ✅ OK | Ya tenía `salir` |
| `entrada` | ✅ MEJORADO | Ahora incluye BorrarArchivo + `salir` |

---

## 🔧 Troubleshooting

### Problema: "El test se queda en bucle infinito"
**Causa:** Falta el comando `salir` al final del archivo  
**Solución:** Todos los archivos ahora tienen `salir` ✅

### Problema: "No compila"
**Solución:**
```bash
make limpiar
make
```

### Problema: "Warnings de funciones sin implementar"
**Respuesta:** Normal. Son funciones de Fase 2 (CrearVersion, BorrarVersion, etc.)

### Problema: "Memory leaks"
**Verificar:**
```bash
cat entrada | valgrind --leak-check=full ./main 2>&1 | tail -20
```

**Resultado esperado:** 
```
All heap blocks were freed -- no leaks are possible ✅
```

---

## 🎯 Cómo Ejecutar para la Defensa

```bash
# 1. Compilar
make limpiar && make

# 2. Test rápido
cat tests/test_minimo.txt | ./main

# 3. Test completo (todas las funciones)
cat entrada | ./main

# 4. Verificar memoria (IMPORTANTE)
cat entrada | valgrind --leak-check=full ./main
```

---

**Última actualización:** Octubre 21, 2025  
**Todos los tests verificados y funcionando** ✅
