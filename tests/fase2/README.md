# Tests de Desarrollo - Fase 2

## Descripción
Tests unitarios de desarrollo para verificar funcionalidades de Fase 2 (versiones jerárquicas).

## Tests Disponibles

### 1. test_parser.cpp
**TARJETA 2: Parser de Versiones**
- Convierte strings "1.2.3" en arrays [1,2,3]
- Valida formato, números positivos, sin puntos consecutivos
- **Ejecutar:** `bash test_parser.sh`

### 2. test_navegacion.cpp
**TARJETA 3: Navegación del Árbol**
- Navega por el árbol N-ario de versiones
- Busca versiones por secuencia numérica
- **Ejecutar:** `bash test_navegacion.sh`

### 3. test_validaciones.cpp
**TARJETA 4: Validaciones**
- Valida que padre existe
- Valida que no hay huecos en numeración
- **Ejecutar:** `bash test_validaciones.sh`

### 4. test_desplazamiento.cpp
**TARJETA 5: Desplazamiento y Renumeración**
- Desplaza versiones cuando se inserta número existente
- Renumera subárboles correctamente
- **Ejecutar:** `bash test_desplazamiento.sh`

### 5. test_crearversion.cpp
**TARJETA 6: CrearVersion Completa (INTEGRACIÓN)**
- Test completo de CrearVersion con todos los componentes
- 10 tests exhaustivos: primer nivel, subversiones, desplazamiento
- **Ejecutar:** `bash test_crearversion.sh`
- **ESTE ES EL TEST MÁS IMPORTANTE DE FASE 2**

## Cómo Ejecutar

### Ejecutar un test específico:
```bash
cd /mnt/d/Escritorio/CODE/EDA/eda-ob2025/tests/fase2
bash test_crearversion.sh  # El más completo
```

### Ejecutar todos los tests:
```bash
cd /mnt/d/Escritorio/CODE/EDA/eda-ob2025/tests/fase2
for test in test_*.sh; do
    echo "=========================================="
    echo "Ejecutando $test"
    echo "=========================================="
    bash "$test"
done
```

## Estructura de los Tests

Cada test tiene:
- **`.cpp`**: Código del test (incluye includes, crea estructuras, valida resultados)
- **`.sh`**: Script que compila el `.cpp` + todos los `.c` necesarios, ejecuta y corre Valgrind

## Resultados Esperados

### TARJETA 2: Parser
- 11 tests PASS

### TARJETA 3: Navegación
- 13 tests PASS

### TARJETA 4: Validaciones
- 30 tests PASS

### TARJETA 5: Desplazamiento
- 48 tests PASS

### TARJETA 6: CrearVersion (INTEGRACIÓN)
- **10 tests PASS**
- **0 memory leaks**

**Total Fase 2:** 112 tests, 0 leaks ✅

## Notas

- Los tests NO usan el `main` del profesor
- Llaman directamente a funciones internas
- Son independientes entre sí
- Valgrind verifica memory leaks en cada test
