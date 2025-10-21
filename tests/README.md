# Tests - Fase 1

Archivos de prueba para verificar las funciones implementadas.

## Cómo ejecutar los tests

```bash
# Método 1: Con cat (recomendado)
cat tests/test_minimo.txt | ./main

# Método 2: Con el archivo 'entrada'
cat entrada | ./main
```

## Archivos de test

### `test_minimo.txt`
Test básico para verificar funcionalidad mínima:
- Crear archivo
- Insertar 1 línea
- Mostrar texto
- Salir

**Uso:** Verificación rápida de que todo compila y funciona.

### `test_simple.txt`
Test completo de funcionalidades Fase 1:
- Insertar múltiples líneas en versión 1
- Insertar línea en medio (verifica sistema de deltas)
- Crear versión 2 independiente
- Mostrar ambas versiones (verifica independencia)

**Uso:** Prueba completa de InsertarLinea y MostrarTexto.

### `test_mostrar_texto.txt`
Test específico para MostrarTexto:
- Similar a test_simple.txt
- Enfocado en reconstrucción de texto desde deltas

### `entrada`
Archivo de entrada principal para pruebas manuales.

## Salida esperada

Al ejecutar `test_simple.txt` deberías ver:

```
Archivo: prueba - Version 1

1.  Primera línea del archivo
2.  Línea insertada en medio
3.  Segunda línea agregada
4.  Tercera línea de texto
```

Y versión 2 independiente:
```
Archivo: prueba - Version 2

1.  Versión 2 línea 1
2.  Versión 2 línea 2
```

## Tests pendientes (Fase 1)

- [ ] Test de BorrarLinea
- [ ] Test de BorrarArchivo
- [ ] Test de casos de error (línea inválida, versión inexistente)
- [ ] Test con valgrind (verificar 0 memory leaks)
