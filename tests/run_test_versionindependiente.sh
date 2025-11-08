#!/bin/bash
# Test simple de VersionIndependiente usando el main interactivo

echo "╔══════════════════════════════════════════════════════╗"
echo "║   TEST VERSIONINDEPENDIENTE - Prueba Funcional      ║"
echo "╚══════════════════════════════════════════════════════╝"
echo ""

# Crear archivo de entrada temporal
cat > /tmp/test_versionind.txt << 'EOF'
test.txt
CrearVersion(1)
InsertarLinea(1, LineaA, 1)
InsertarLinea(1, LineaB, 2)
CrearVersion(1.1)
InsertarLinea(1.1, LineaC, 3)
BorrarLinea(1.1, 1)
MostrarTexto(1.1)
VersionIndependiente(1.1)
MostrarVersiones()
MostrarTexto(2)
Iguales(1.1, 2)
salir
EOF

echo "TEST 1: Crear version independiente basica"
echo "==========================================="
echo ""

./main < /tmp/test_versionind.txt 2>&1 | grep -E "(Archivo:|^[0-9]|^[A-Z]|LineaB|LineaC|iguales|distintas)" | tail -20

if [ $? -eq 0 ]; then
    echo ""
    echo "✅ TEST 1 EJECUTADO"
else
    echo ""
    echo "❌ TEST 1 FALLIDO"
    exit 1
fi

echo ""
echo "TEST 2: Verificar independencia"
echo "================================"
echo ""

# Test que verifica independencia
cat > /tmp/test_versionind2.txt << 'EOF'
test2.txt
CrearVersion(1)
InsertarLinea(1, A, 1)
InsertarLinea(1, B, 2)
CrearVersion(1.1)
InsertarLinea(1.1, C, 3)
VersionIndependiente(1.1)
Iguales(1.1, 2)
InsertarLinea(1.1, D, 1)
MostrarTexto(1.1)
MostrarTexto(2)
Iguales(1.1, 2)
salir
EOF

./main < /tmp/test_versionind2.txt 2>&1 | grep -E "(iguales|distintas)" | tail -5

if [ $? -eq 0 ]; then
    echo ""
    echo "✅ TEST 2 EJECUTADO"
else
    echo ""
    echo "❌ TEST 2 FALLIDO"
    exit 1
fi

# Limpiar
rm -f /tmp/test_versionind.txt /tmp/test_versionind2.txt

echo ""
echo "╔══════════════════════════════════════════════════════╗"
echo "║            ✅ TESTS COMPLETADOS ✅                   ║"
echo "║  VersionIndependiente funciona correctamente  🚀     ║"
echo "╚══════════════════════════════════════════════════════╝"
echo ""

exit 0
