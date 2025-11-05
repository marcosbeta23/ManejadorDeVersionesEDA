#!/bin/bash

# Script para compilar y ejecutar tests de navegación del árbol
# TARJETA 3: Navegación del Árbol N-ario

echo "================================================"
echo "  COMPILANDO TEST DE NAVEGACIÓN DEL ÁRBOL"
echo "================================================"
echo ""

# Compilar el test
g++ -Wall -Wextra -g \
    test_navegacion.cpp \
    src/version.c \
    src/utils.c \
    src/mod.c \
    -o test_navegacion \
    -I include

if [ $? -ne 0 ]; then
    echo "❌ ERROR: Falló la compilación"
    exit 1
fi

echo "✓ Compilación exitosa"
echo ""

echo "================================================"
echo "  EJECUTANDO TESTS"
echo "================================================"
echo ""

./test_navegacion

TEST_RESULT=$?

echo ""
echo "================================================"
echo "  VERIFICACIÓN CON VALGRIND"
echo "================================================"
echo ""

valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes ./test_navegacion 2>&1 | grep -A 10 "HEAP SUMMARY"

echo ""
echo "================================================"

# Limpiar
rm -f test_navegacion

exit $TEST_RESULT
