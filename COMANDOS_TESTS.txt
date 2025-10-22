## 🚀 COMANDOS PARA EJECUTAR TESTS

### Compilar:
```bash
make limpiar && make
```

### Ejecutar tests (la ruta correcta es tests/):

```bash
# Test básico
cat tests/test_minimo.txt | ./main

# Test de insertar
cat tests/test_insertar.txt | ./main

# Test completo ← TU ERROR ESTABA AQUÍ
cat tests/test_completo.txt | ./main

# Test de borrar
cat tests/test_borrar_linea.txt | ./main

# Archivo principal
cat entrada | ./main
```

### ❌ ERROR:
```bash
cat test_completo | ./main
# No such file or directory
```

### ✅ CORRECTO:
```bash
cat tests/test_completo.txt | ./main
#   ^^^^^              ^^^^
#   carpeta           extensión
```

### Script automático:
```bash
bash run_tests.sh
```
