#!/bin/bash
# Test de Desplazamiento y Renumeración
# TARJETA 5 - Fase 2

echo "==============================================="
echo "  COMPILANDO TEST DE DESPLAZAMIENTO"
echo "==============================================="
echo ""

# Compilar test
g++ -std=c++17 -Wall -Wextra -g \
    test_desplazamiento.cpp \
    src/utils.c \
    src/version.c \
    src/mod.c \
    -o test_desplazamiento \
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
./test_desplazamiento

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
         --log-file=valgrind_desplazamiento.log \
         ./test_desplazamiento > /dev/null 2>&1

# Mostrar resumen de valgrind
echo "Resumen de Valgrind:"
echo ""
grep "LEAK SUMMARY" valgrind_desplazamiento.log -A 5
grep "ERROR SUMMARY" valgrind_desplazamiento.log

# Verificar si hay leaks
if grep -q "All heap blocks were freed -- no leaks are possible" valgrind_desplazamiento.log; then
    LEAKS=0
    echo ""
    echo "✓ No hay memory leaks"
else
    LEAKS=$(grep "definitely lost" valgrind_desplazamiento.log | awk '{print $4}' | sed 's/,//')
    echo ""
    echo "⚠️  Memory leaks detectados: $LEAKS bytes"
    echo "Ver valgrind_desplazamiento.log para detalles"
fi

echo ""
echo "==============================================="
echo "  RESULTADO FINAL"
echo "==============================================="
echo ""

if [ $TEST_RESULT -eq 0 ] && [ "$LEAKS" -eq 0 ]; then
    echo "✅ TARJETA 5 COMPLETADA"
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
