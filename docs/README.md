# Obligatorio EDA 2025 - Manejador de Versiones

Autores: Marcos Betancor  
Fecha: Octubre 2025  
Curso: Estructuras de Datos y Algoritmos

## 📋 Descripción

Sistema de control de versiones simplificado para archivos de texto plano.

## 🏗️ Estructura del Proyecto

```
eda-ob2025/
├── src/           # Código fuente (.c)
├── include/       # Headers (.h)
├── tests/         # Archivos de prueba
├── docs/          # Documentación
├── Makefile       # Sistema de compilación
└── README.md      # Este archivo
```

## 🔧 Compilación

```bash
# Compilar
make

# Limpiar archivos compilados
make limpiar

# Ejecutar con valgrind
make valgrind
```

## 🚀 Ejecución

```bash
./main
```

## 📊 Estado del Proyecto

### ✅ Estructuras de Datos - COMPLETADO
- [x] `nodo_modificacion` - Sistema de deltas
- [x] `nodo_version` - Árbol n-ario
- [x] `nodo_archivo` - Estructura principal
- [x] Funciones auxiliares completas
- [x] Compila sin errores

### Fase 1 - Control Intermedio 
- [x] CrearArchivo 
- [x] InsertarLinea
- [x] MostrarTexto
- [x] BorrarLinea
- [x] BorrarArchivo

### Fase 2 - Entrega Final 
- [ ] CrearVersion (árbol completo)
- [ ] BorrarVersion
- [ ] MostrarVersiones
- [ ] MostrarCambios
- [ ] Iguales
- [ ] VersionIndependiente

## 📝 Decisiones de Diseño

### Estructuras de Datos

**nodo_archivo:**
- `nombre`: Nombre del archivo
- `primeraVersion`: Puntero a primera versión (lista enlazada simple en Fase 1)

**nodo_version:**
- `numero`: Número de versión (1, 2, 3...)
- `primeraModificacion`: Lista de modificaciones
- `siguienteHermano`: Siguiente versión

**nodo_modificacion:**
- `tipo`: INSERCION o BORRADO
- `nroLinea`: Número de línea afectada
- `textoLinea`: Texto (solo para INSERCION)
- `siguiente`: Siguiente modificación

### Sistema de Deltas

Cada versión guarda solo sus cambios (deltas), no el texto completo.
Para reconstruir el texto se aplican todas las modificaciones en orden.

## 🧪 Testing

Los archivos de prueba están en `tests/`:
- `test_crear_archivo.txt`
- `test_insertar.txt`
- `test_completo.txt`

## 💾 Gestión de Memoria

Se usa `valgrind` para verificar que no hay memory leaks:
```bash
valgrind --leak-check=full ./main
```

## 📚 Recursos

- [Enunciado del obligatorio](docs/)
- [Guía de implementación](docs/)

---

**Última actualización:** 21 de Octubre 2025






