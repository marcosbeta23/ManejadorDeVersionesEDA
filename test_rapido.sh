#!/bin/bash

echo "============================================"
echo "PROBANDO TODOS LOS TESTS - FASE 1"
echo "============================================"
echo ""

# Test 1
echo ">>> Test 1: test_minimo.txt"
timeout 5s bash -c "cat tests/test_minimo.txt | ./main" > /dev/null 2>&1
if [ $? -eq 0 ]; then
    echo "✅ PASÓ"
else
    echo "❌ FALLÓ"
fi

# Test 2
echo ">>> Test 2: test_insertar.txt"
timeout 5s bash -c "cat tests/test_insertar.txt | ./main" > /dev/null 2>&1
if [ $? -eq 0 ]; then
    echo "✅ PASÓ"
else
    echo "❌ FALLÓ"
fi

# Test 3
echo ">>> Test 3: test_simple.txt"
timeout 5s bash -c "cat tests/test_simple.txt | ./main" > /dev/null 2>&1
if [ $? -eq 0 ]; then
    echo "✅ PASÓ"
else
    echo "❌ FALLÓ"
fi

# Test 4
echo ">>> Test 4: test_borrar_linea.txt"
timeout 5s bash -c "cat tests/test_borrar_linea.txt | ./main" > /dev/null 2>&1
if [ $? -eq 0 ]; then
    echo "✅ PASÓ"
else
    echo "❌ FALLÓ"
fi

# Test 5
echo ">>> Test 5: test_completo.txt"
timeout 5s bash -c "cat tests/test_completo.txt | ./main" > /dev/null 2>&1
if [ $? -eq 0 ]; then
    echo "✅ PASÓ"
else
    echo "❌ FALLÓ"
fi

# Test 6
echo ">>> Test 6: entrada (principal)"
timeout 5s bash -c "cat entrada | ./main" > /dev/null 2>&1
if [ $? -eq 0 ]; then
    echo "✅ PASÓ"
else
    echo "❌ FALLÓ"
fi

echo ""
echo "============================================"
echo "PRUEBAS COMPLETADAS"
echo "============================================"
