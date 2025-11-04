# 📋 GUÍA RÁPIDA - CÓMO EJECUTAR LOS TESTS

## ❌ Error Común

```bash
cat test_completo | ./main
# Error: No such file or directory
```

**Razón:** Los archivos de test están en la carpeta `tests/`

## ✅ Forma Correcta

### Ejecutar tests individuales:

```bash
# Compilar primero
make limpiar && make

# Archivo principal
cat entrada | ./main

# Tests (están en la carpeta tests/)
cat tests/test_minimo.txt | ./main
cat tests/test_insertar.txt | ./main
cat tests/test_simple.txt | ./main
cat tests/test_borrar_linea.txt | ./main
cat tests/test_completo.txt | ./main
cat tests/test_mostrar_texto.txt | ./main
```

### Ejecutar todos los tests automáticamente:

```bash
bash run_tests.sh
```

O con el script rápido:

```bash
bash test_rapido.sh
```

## 📁 Estructura de Archivos de Test

```
eda-ob2025/
├── entrada                    ← Archivo principal de demostración
├── tests/
│   ├── test_minimo.txt       ← Test más básico
│   ├── test_insertar.txt     ← Test de InsertarLinea
│   ├── test_simple.txt       ← Test completo simple
│   ├── test_borrar_linea.txt ← Test de BorrarLinea
│   ├── test_completo.txt     ← Test de todas las funciones
│   └── test_mostrar_texto.txt← Test de MostrarTexto
```

## 🎯 Ejemplos de Ejecución

### Test más básico (verificar que compila):
```bash
cat tests/test_minimo.txt | ./main
```

**Salida esperada:**
```
Ingrese el nombre del archivo a versionar:
 - Archivo "miarchivo" creado.
...
Archivo: miarchivo - Version 1

1. Hola mundo
 - OK
...
        - CHAUUUUU!!!

 - Archivo "miarchivo" borrado con exito.
```

### Test completo (todas las funciones):
```bash
cat tests/test_completo.txt | ./main
```

### Archivo principal (demostración completa):
```bash
cat entrada | ./main
```

## 🔧 Si los Tests Siguen en Bucle

Si después de ejecutar un test el programa se queda esperando input:

1. **Presiona Ctrl+C** para salir
2. Verifica que el archivo tenga formato Unix (LF):
   ```bash
   cat -A tests/test_completo.txt
   ```
3. Si ves `^M$` al final de las líneas, convierte el archivo:
   ```bash
   sed 's/\r$//' tests/test_completo.txt > temp.txt
   mv temp.txt tests/test_completo.txt
   ```

## 📊 Verificar que Todos los Tests Funcionen

```bash
# Este script prueba todos los tests con timeout
bash test_rapido.sh
```

**Salida esperada:**
```
>>> Test 1: test_minimo.txt
✅ PASÓ
>>> Test 2: test_insertar.txt
✅ PASÓ
...
```

---

**TL;DR:** Usa `cat tests/test_completo.txt | ./main` (con `tests/` al inicio)
