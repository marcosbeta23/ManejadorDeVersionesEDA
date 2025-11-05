# Tests - TARJETAs 6 y 7# Tests : CrearVersion



## Test Automatizado (RECOMENDADO)Test de integración completo con validaciones automáticas y Valgrind.



Test de integración completo con validaciones automáticas y Valgrind.## Ejecutar



### Ejecutar:Desde el directorio tests:

```bash```bash

cd testscd tests

bash test_crearversion.shbash test_crearversion.sh

``````



### Resultado esperado:Desde WSL en Windows:

``````bash

Tests ejecutados: 10wsl bash -c "cd /mnt/d/Escritorio/CODE/EDA/eda-ob2025/tests && bash test_crearversion.sh"

Tests pasados: 10```

0 memory leaks

✅ TARJETA 6 COMPLETADA## Resultado esperado

```

```

### Qué testea:Tests ejecutados: 10

- CrearVersion con versiones jerárquicasTests pasados: 10

- Parser, navegación, validacionesTests fallados: 0

- Desplazamiento y renumeración0 memory leaks

- MostrarVersiones (TARJETA 7)```



---## Qué testea



## Demostración con Main del Profesor- Parser de versiones jerárquicas ("1.2.3" → [1,2,3])

- Navegación del árbol N-ario

Archivo de entrada para demostrar CrearVersion + MostrarVersiones.- Validaciones (padre existe, sin huecos)

- Desplazamiento y renumeración automática

### Ejecutar:- Creación de versiones en todos los niveles (1, 1.1, 1.1.1, etc.)

```bash

make## Archivos

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
