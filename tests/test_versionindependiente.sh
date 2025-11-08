#!/bin/bash
# Test de VersionIndependiente

echo "Compilando proyecto completo..."

# Compilar el proyecto principal primero
make limpiar > /dev/null 2>&1
make > /dev/null 2>&1

if [ $? -ne 0 ]; then
    echo "❌ Error compilando proyecto principal"
    exit 1
fi

echo "Compilando test de VersionIndependiente..."

g++ -Wall -Wextra -Iinclude \
    -c tests/test_versionindependiente.cpp \
    -o test_versionindependiente.o

if [ $? -ne 0 ]; then
    echo "❌ Error en compilacion del test"
    exit 1
fi

g++ -o test_versionindependiente \
    test_versionindependiente.o \
    archivo.o version.o mod.o utils.o

if [ $? -ne 0 ]; then
    echo "❌ Error en linkeo"
    exit 1
fi

echo "✅ Compilacion exitosa"
echo ""
echo "Ejecutando tests..."
echo ""

./test_versionindependiente

RESULT=$?

# Limpiar
rm -f test_versionindependiente.o test_versionindependiente

exit $RESULT
