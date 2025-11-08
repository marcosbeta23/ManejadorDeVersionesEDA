#!/bin/bash
# Test Final Completo - Todas las operaciones del proyecto

echo ""
echo "╔════════════════════════════════════════════════════════════╗"
echo "║  TEST FINAL COMPLETO - OBLIGATORIO EDA 2025               ║"
echo "║  Validación de TODAS las operaciones implementadas        ║"
echo "╚════════════════════════════════════════════════════════════╝"
echo ""

PASSED=0
FAILED=0

# Colores
GREEN='\033[0;32m'
RED='\033[0;31m'
NC='\033[0m' # No Color

# Test 1: CrearVersion y MostrarVersiones
echo "TEST 1: CrearVersion + MostrarVersiones"
echo "========================================"
cat > /tmp/test_final_1.txt << 'EOF'
archivo1.txt
CrearVersion(1)
CrearVersion(2)
CrearVersion(1.1)
CrearVersion(1.2)
CrearVersion(2.1)
MostrarVersiones()
salir
EOF

OUTPUT=$(../main < /tmp/test_final_1.txt 2>&1 | grep -E "^[0-9]")
if echo "$OUTPUT" | grep -q "^1$" && echo "$OUTPUT" | grep -q "^1.1$" && echo "$OUTPUT" | grep -q "^2.1$"; then
    echo -e "${GREEN}✅ PASSED${NC}"
    ((PASSED++))
else
    echo -e "${RED}❌ FAILED${NC}"
    ((FAILED++))
fi
echo ""

# Test 2: InsertarLinea y MostrarTexto
echo "TEST 2: InsertarLinea + MostrarTexto"
echo "====================================="
cat > /tmp/test_final_2.txt << 'EOF'
archivo2.txt
CrearVersion(1)
InsertarLinea(1, Primera, 1)
InsertarLinea(1, Segunda, 2)
InsertarLinea(1, Tercera, 3)
MostrarTexto(1)
salir
EOF

OUTPUT=$(../main < /tmp/test_final_2.txt 2>&1 | grep -E "^[0-9]\.")
if echo "$OUTPUT" | grep -q "Primera" && echo "$OUTPUT" | grep -q "Segunda" && echo "$OUTPUT" | grep -q "Tercera"; then
    echo -e "${GREEN}✅ PASSED${NC}"
    ((PASSED++))
else
    echo -e "${RED}❌ FAILED${NC}"
    ((FAILED++))
fi
echo ""

# Test 3: BorrarLinea
echo "TEST 3: BorrarLinea"
echo "==================="
cat > /tmp/test_final_3.txt << 'EOF'
archivo3.txt
CrearVersion(1)
InsertarLinea(1, A, 1)
InsertarLinea(1, B, 2)
InsertarLinea(1, C, 3)
BorrarLinea(1, 2)
MostrarTexto(1)
salir
EOF

OUTPUT=$(../main < /tmp/test_final_3.txt 2>&1 | grep -E "^[0-9]\.")
if echo "$OUTPUT" | grep -q "A" && echo "$OUTPUT" | grep -q "C" && ! echo "$OUTPUT" | grep -q "B"; then
    echo -e "${GREEN}✅ PASSED${NC}"
    ((PASSED++))
else
    echo -e "${RED}❌ FAILED${NC}"
    ((FAILED++))
fi
echo ""

# Test 4: MostrarCambios
echo "TEST 4: MostrarCambios"
echo "======================"
cat > /tmp/test_final_4.txt << 'EOF'
archivo4.txt
CrearVersion(1)
InsertarLinea(1, Base, 1)
CrearVersion(1.1)
InsertarLinea(1.1, Nueva, 2)
MostrarCambios(1.1)
salir
EOF

OUTPUT=$(../main < /tmp/test_final_4.txt 2>&1)
if echo "$OUTPUT" | grep -q "INSERCION(2" && echo "$OUTPUT" | grep -q "Nueva"; then
    echo -e "${GREEN}✅ PASSED${NC}"
    ((PASSED++))
else
    echo -e "${RED}❌ FAILED${NC}"
    ((FAILED++))
fi
echo ""

# Test 5: BorrarVersion con renumeración
echo "TEST 5: BorrarVersion + Renumeración"
echo "====================================="
cat > /tmp/test_final_5.txt << 'EOF'
archivo5.txt
CrearVersion(1)
CrearVersion(2)
CrearVersion(3)
BorrarVersion(2)
MostrarVersiones()
salir
EOF

OUTPUT=$(../main < /tmp/test_final_5.txt 2>&1 | grep -E "^[0-9]$")
LINES=$(echo "$OUTPUT" | wc -l)
if [ "$LINES" -eq 2 ] && echo "$OUTPUT" | grep -q "^1$" && echo "$OUTPUT" | grep -q "^2$"; then
    echo -e "${GREEN}✅ PASSED${NC}"
    ((PASSED++))
else
    echo -e "${RED}❌ FAILED${NC}"
    ((FAILED++))
fi
echo ""

# Test 6: Iguales
echo "TEST 6: Iguales"
echo "==============="
cat > /tmp/test_final_6.txt << 'EOF'
archivo6.txt
CrearVersion(1)
InsertarLinea(1, A, 1)
InsertarLinea(1, B, 2)
CrearVersion(2)
InsertarLinea(2, A, 1)
InsertarLinea(2, B, 2)
Iguales(1, 2)
salir
EOF

OUTPUT=$(../main < /tmp/test_final_6.txt 2>&1)
if echo "$OUTPUT" | grep -q "iguales"; then
    echo -e "${GREEN}✅ PASSED${NC}"
    ((PASSED++))
else
    echo -e "${RED}❌ FAILED${NC}"
    ((FAILED++))
fi
echo ""

# Test 7: VersionIndependiente
echo "TEST 7: VersionIndependiente"
echo "============================"
cat > /tmp/test_final_7.txt << 'EOF'
archivo7.txt
CrearVersion(1)
InsertarLinea(1, X, 1)
CrearVersion(1.1)
InsertarLinea(1.1, Y, 2)
VersionIndependiente(1.1)
MostrarVersiones()
Iguales(1.1, 2)
salir
EOF

OUTPUT=$(../main < /tmp/test_final_7.txt 2>&1)
if echo "$OUTPUT" | grep -q "^2$" && echo "$OUTPUT" | grep -q "iguales"; then
    echo -e "${GREEN}✅ PASSED${NC}"
    ((PASSED++))
else
    echo -e "${RED}❌ FAILED${NC}"
    ((FAILED++))
fi
echo ""

# Limpiar archivos temporales
rm -f /tmp/test_final_*.txt

# Resumen
echo "╔════════════════════════════════════════════════════════════╗"
echo "║                    RESUMEN DE TESTS                        ║"
echo "╠════════════════════════════════════════════════════════════╣"
echo -e "║  Tests pasados:  ${GREEN}${PASSED}/7${NC}                                     ║"
echo -e "║  Tests fallidos: ${RED}${FAILED}/7${NC}                                     ║"
echo "╚════════════════════════════════════════════════════════════╝"
echo ""

if [ $FAILED -eq 0 ]; then
    echo -e "${GREEN}"
    echo "╔════════════════════════════════════════════════════════════╗"
    echo "║        ✅✅✅ TODOS LOS TESTS PASARON ✅✅✅              ║"
    echo "║                                                            ║"
    echo "║  PROYECTO COMPLETO Y FUNCIONAL                             ║"
    echo "║  Todas las operaciones implementadas correctamente         ║"
    echo "║  Listo para entrega 🚀                                     ║"
    echo "╚════════════════════════════════════════════════════════════╝"
    echo -e "${NC}"
    exit 0
else
    echo -e "${RED}"
    echo "╔════════════════════════════════════════════════════════════╗"
    echo "║           ❌ ALGUNOS TESTS FALLARON ❌                     ║"
    echo "║  Revisar implementaciones                                  ║"
    echo "╚════════════════════════════════════════════════════════════╝"
    echo -e "${NC}"
    exit 1
fi
