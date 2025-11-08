# Tests - Obligatorio EDA 2025# Tests - TARJETAs 6 y 7# Tests : CrearVersion



## 🎯 PROYECTO COMPLETO - TODAS LAS OPERACIONES IMPLEMENTADAS



---## Test Automatizado (RECOMENDADO)Test de integración completo con validaciones automáticas y Valgrind.



## 📋 Test Principal: `test_completo.txt`



**Archivo de entrada exhaustivo que prueba TODAS las operaciones en un solo flujo**Test de integración completo con validaciones automáticas y Valgrind.## Ejecutar



### Operaciones validadas:

- ✅ CrearVersion (jerárquico: 1, 2, 1.1, 1.2, 1.1.1)

- ✅ InsertarLinea### Ejecutar:Desde el directorio tests:

- ✅ BorrarLinea

- ✅ MostrarTexto (con ancestros)```bash```bash

- ✅ MostrarCambios (solo modificaciones propias)

- ✅ MostrarVersiones (árbol completo)cd testscd tests

- ✅ BorrarVersion (con renumeración)

- ✅ Iguales (comparación texto reconstruido)bash test_crearversion.shbash test_crearversion.sh

- ✅ VersionIndependiente (versión independiente en nivel 1)

``

### Ejecutar:

```bash

./main < tests/test_completo.txt

```### Resultado esperado:Desde WSL en Windows:



---``````bash



## 🚀 Suite Automatizada: `test_final_completo.sh`Tests ejecutados: 10wsl bash -c "cd /mnt/d/Escritorio/CODE/EDA/eda-ob2025/tests && bash test_crearversion.sh"



**7 tests automáticos con validaciones**Tests pasados: 10```



### Ejecutar:0 memory leaks

```bash

./tests/test_final_completo.sh✅ TARJETA 6 COMPLETADA## Resultado esperado

```

```

### Resultado esperado:

``````

Tests pasados: 7/7

Tests fallados: 0/7### Qué testea:Tests ejecutados: 10

✅ TODOS LOS TESTS PASARON

```- CrearVersion con versiones jerárquicasTests pasados: 10



---- Parser, navegación, validacionesTests fallados: 0



## 📊 Estado del Proyecto- Desplazamiento y renumeración0 memory leaks



**✅ COMPLETO - Listo para entrega**- MostrarVersiones (TARJETA 7)```



- Fecha: 8 Noviembre 2025

- Tests: 7/7 pasando (100%)

- Commits: 4 principales---## Qué testea

- Memory leaks: 0



**Proyecto listo para defensa ✅**

## Demostración con Main del Profesor- Parser de versiones jerárquicas ("1.2.3" → [1,2,3])

- Navegación del árbol N-ario

Archivo de entrada para demostrar CrearVersion + MostrarVersiones.- Validaciones (padre existe, sin huecos)

- Desplazamiento y renumeración automática

### Ejecutar:- Creación de versiones en todos los niveles (1, 1.1, 1.1.1, etc.)

```bash

make

## Archivos

cat tests/entrada_completa.txt | ./main 2>&1 | grep -E "Archivo:|^[0-9]|OK"

```- `test_crearversion.cpp` - Código del test (10 casos)

- `test_crearversion.sh` - Script de compilación y ejecución

### Output esperado:- `test_crearversion` - Ejecutable (generado automáticamente)

```
Archivo: miarchivo
1
2
3
OK

Archivo: miarchivo
1
1.1
1.2
2
3
OK
```

**Nota:** El main es interactivo, el filtro grep muestra solo lo importante.

---

## Archivos

- `test_crearversion.cpp/.sh` - Test automatizado
- `entrada_completa.txt` - Demo para el main del profesor
