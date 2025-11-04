#!/bin/bash
# Test de Validaciones de CrearVersion
# TARJETA 4 - Fase 2

echo "==============================================="
echo "  COMPILANDO TEST DE VALIDACIONES"
echo "==============================================="
echo ""

# Compilar test
g++ -std=c++17 -Wall -Wextra -g \
    test_validaciones.cpp \
    src/utils.c \
    src/version.c \
    src/mod.c \
    -o test_validaciones \
    -I./include

if [ $? -ne 0 ]; then
    echo "❌ Error en compilación"
    exit 1
fi

echo "✓ Compilación exitosa"
echo ""

echo "==============================================="
echo "  EJECUTANDO TESTS"
echo "==============================================="
echo ""

# Ejecutar tests
./test_validaciones

TEST_RESULT=$?

echo ""
echo "==============================================="
echo "  VERIFICANDO MEMORIA CON VALGRIND"
echo "==============================================="
echo ""

# Verificar memoria
valgrind --leak-check=full \
         --show-leak-kinds=all \
         --track-origins=yes \
         --verbose \
         --log-file=valgrind_validaciones.log \
         ./test_validaciones > /dev/null 2>&1

# Mostrar resumen de valgrind
echo "Resumen de Valgrind:"
echo ""
grep "LEAK SUMMARY" valgrind_validaciones.log -A 5
grep "ERROR SUMMARY" valgrind_validaciones.log

# Verificar si hay leaks
if grep -q "All heap blocks were freed -- no leaks are possible" valgrind_validaciones.log; then
    LEAKS=0
    echo ""
    echo "✓ No hay memory leaks"
else
    LEAKS=$(grep "definitely lost" valgrind_validaciones.log | awk '{print $4}' | sed 's/,//')
    echo ""
    echo "⚠️  Memory leaks detectados: $LEAKS bytes"
    echo "Ver valgrind_validaciones.log para detalles"
fi

echo ""
echo "==============================================="
echo "  RESULTADO FINAL"
echo "==============================================="
echo ""

if [ $TEST_RESULT -eq 0 ] && [ "$LEAKS" -eq 0 ]; then
    echo "✅ TARJETA 4 COMPLETADA"
    echo "   - Todos los tests pasaron"
    echo "   - 0 memory leaks"
    exit 0
else
    echo "❌ Hay problemas que corregir"
    if [ $TEST_RESULT -ne 0 ]; then
        echo "   - Algunos tests fallaron"
    fi
    if [ "$LEAKS" -ne 0 ]; then
        echo "   - Hay memory leaks"
    fi
    exit 1
fi
