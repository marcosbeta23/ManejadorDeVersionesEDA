#!/bin/bash
# Script para compilar y ejecutar el test del parser

echo "=========================================="
echo "COMPILANDO TEST DEL PARSER DE VERSIONES"
echo "=========================================="
echo ""

# Compilar
g++ -Wall -Wextra -Iinclude -c src/utils.c -o utils_test.o
g++ -Wall -Wextra -Iinclude -c src/version.c -o version_test.o
g++ -Wall -Wextra -Iinclude -c src/mod.c -o mod_test.o
g++ -Wall -Wextra -Iinclude test_parser.cpp utils_test.o version_test.o mod_test.o -o test_parser

if [ $? -eq 0 ]; then
    echo "✓ Compilación exitosa"
    echo ""
    echo "=========================================="
    echo "EJECUTANDO TESTS"
    echo "=========================================="
    echo ""
    ./test_parser
    echo ""
    echo "=========================================="
    echo "VERIFICANDO MEMORY LEAKS CON VALGRIND"
    echo "=========================================="
    echo ""
    valgrind --leak-check=full --show-leak-kinds=all ./test_parser 2>&1 | tail -20
    
    # Limpiar archivos objeto
    rm -f utils_test.o version_test.o mod_test.o
else
    echo "✗ Error en la compilación"
    exit 1
fi
