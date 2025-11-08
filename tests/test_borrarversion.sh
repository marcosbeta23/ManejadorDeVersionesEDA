#!/bin/bash

echo "=== TEST BORRARVERSION COMPLETO ==="
echo ""
echo "Ejecutando: ./main < tests/entrada_test_borrar.txt"
echo "----------------------------------------------"

./main < tests/entrada_test_borrar.txt

echo "----------------------------------------------"
echo ""
echo "=== VALIDACION CON VALGRIND ==="
valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes ./main < tests/entrada_test_borrar.txt 2>&1 | grep -E '(LEAK SUMMARY|definitely lost|indirectly lost|ERROR SUMMARY)'
