#!/bin/bash

echo "=== TEST BORRARVERSION ==="
echo ""
echo "Archivo de entrada: tests/entrada_test_borrar.txt"
echo "Contenido:"
cat tests/entrada_test_borrar.txt
echo ""
echo "==================================="
echo ""

# Ejecutar y capturar solo las partes importantes
echo "EJECUTANDO TEST..."
echo ""

./main < tests/entrada_test_borrar.txt 2>/dev/null | grep -v "CrearVersion\|BorrarVersion\|MostrarVersiones\|InsertarLinea\|BorrarLinea\|MostrarTexto\|MostrarCambios\|Iguales\|VersionIndependiente\|salir\|>" | grep -v "^$" | head -40

echo ""
echo "==================================="
echo "TEST COMPLETADO"
