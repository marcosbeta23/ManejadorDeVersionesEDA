// Estructuras de Datos y Algoritmos - Obligatorio 2025
// Tecnologo en Informatica FIng - DGETP - UTEC
//
// archivo.c
// Modulo de implementacion de archivo.

#include "archivo.h"
#include "utils.h"
#include <string.h>
#include <iostream>

using namespace std;

struct nodo_archivo {
    char* nombre;
    Version primeraVersion;
};

// Crea el archivo con el nombre especificado y lo inicializa sin contenido (vacío).
Archivo CrearArchivo(char * nombre){
	if (nombre == nullptr || strlen(nombre) == 0) {
        return nullptr;
    }
	Archivo a = new nodo_archivo;
	a->nombre = new char[strlen(nombre) + 1];
	strcpy(a->nombre, nombre);
	a->primeraVersion = nullptr;
	
	return a;
}

// Elimina toda la memoria utilizada por el archivo y asigna NULL al puntero a.
TipoRet BorrarArchivo(Archivo &a){
	if (a == nullptr) {
		return ERROR;
	}
	
	// Liberar versiones y modificaciones
	liberarArbolVersiones(a->primeraVersion);
	
	delete[] a->nombre;
	delete a;
	a = NULL;
	
	return OK;
}

// Crea una nueva versión del archivo si la versión especificada cumple con las siguientes reglas:
// - El padre de la nueva versión a crear ya debe existir.
// - No pueden quedar "huecos" entre versiones hermanas. 
TipoRet CrearVersion(Archivo &a, char * version, char * error){
	if (a == nullptr) {
		strcpy(error, "Archivo no existe");
		return ERROR;
	}
	
	if (version == nullptr || strlen(version) == 0) {
		strcpy(error, "Version invalida");
		return ERROR;
	}
	
	// Parsear versión
	int* secuencia = nullptr;
	int longitud = 0;
	secuencia = parsearVersion(version, &longitud);
	
	if (secuencia == nullptr) {
		strcpy(error, "Formato de version invalido");
		return ERROR;
	}
	
	// Validar padre
	if (!validarPadreExiste(a->primeraVersion, secuencia, longitud)) {
		strcpy(error, "El padre de la version no existe");
		liberarArrayVersion(secuencia);
		return ERROR;
	}
	
	int numeroNuevo = secuencia[longitud - 1];
	
	// Determinar padre
	Version padre = nullptr;
	if (longitud > 1) {
		padre = navegarAVersion(a->primeraVersion, secuencia, longitud - 1);
		if (padre == nullptr) {
			strcpy(error, "Error al navegar al padre");
			liberarArrayVersion(secuencia);
			return ERROR;
		}
	}
	
	// Validar sin huecos
	if (!validarSinHuecos(padre, a->primeraVersion, numeroNuevo)) {
		strcpy(error, "Crear la version generaria huecos en la numeracion");
		liberarArrayVersion(secuencia);
		return ERROR;
	}
	
	// Verificar si existe (desplazamiento)
	Version hermanos = (padre != nullptr) ? padre->primerHijo : a->primeraVersion;
	Version existente = (padre != nullptr) ? buscarHijo(padre, numeroNuevo) : buscarVersionEnLista(hermanos, numeroNuevo);
	
	if (existente != nullptr) {
		desplazarYRenumerar(padre, a->primeraVersion, numeroNuevo, 1);
	}
	
	// Crear nodo
	Version nuevaVersion = crearVersion(numeroNuevo);
	if (nuevaVersion == nullptr) {
		strcpy(error, "Error al crear nodo de version");
		liberarArrayVersion(secuencia);
		return ERROR;
	}
	
	nuevaVersion->padre = padre;
	
	// Insertar en lista ordenada
	if (padre != nullptr) {
		if (padre->primerHijo == nullptr) {
			padre->primerHijo = nuevaVersion;
		} else {
			Version actual = padre->primerHijo;
			Version anterior = nullptr;
			
			while (actual != nullptr && actual->numero < numeroNuevo) {
				anterior = actual;
				actual = actual->siguienteHermano;
			}
			
			if (anterior == nullptr) {
				nuevaVersion->siguienteHermano = padre->primerHijo;
				padre->primerHijo = nuevaVersion;
			} else {
				nuevaVersion->siguienteHermano = anterior->siguienteHermano;
				anterior->siguienteHermano = nuevaVersion;
			}
		}
	} else {
		if (a->primeraVersion == nullptr) {
			a->primeraVersion = nuevaVersion;
		} else {
			Version actual = a->primeraVersion;
			Version anterior = nullptr;
			
			while (actual != nullptr && actual->numero < numeroNuevo) {
				anterior = actual;
				actual = actual->siguienteHermano;
			}
			
			if (anterior == nullptr) {
				nuevaVersion->siguienteHermano = a->primeraVersion;
				a->primeraVersion = nuevaVersion;
			} else {
				nuevaVersion->siguienteHermano = anterior->siguienteHermano;
				anterior->siguienteHermano = nuevaVersion;
			}
		}
	}
	
	liberarArrayVersion(secuencia);
	
	return OK;
}

// Elimina una versión del archivo si la version pasada por parámetro existe. En otro caso la operación quedará sin efecto.
TipoRet BorrarVersion(Archivo &a, char * version){
	return NO_IMPLEMENTADA;
}

// Función auxiliar recursiva para mostrar el árbol con indentación
void mostrarArbolRecursivo(Version v, int nivel, int* numerosPorNivel, int profundidad) {
    if (v == NULL) return;
    
    // Construir el número de versión completo (ej: 1.2.3)
    for (int i = 0; i < profundidad; i++) {
        if (i > 0) cout << ".";
        cout << numerosPorNivel[i];
    }
    cout << "\n";
    
    // Recursivamente mostrar hijos (incrementar profundidad)
    if (v->primerHijo != NULL) {
        Version hijo = v->primerHijo;
        while (hijo != NULL) {
            numerosPorNivel[profundidad] = hijo->numero;
            mostrarArbolRecursivo(hijo, nivel + 1, numerosPorNivel, profundidad + 1);
            hijo = hijo->siguienteHermano;
        }
    }
}

TipoRet MostrarVersiones(Archivo a){
    if (a == NULL) {
        cout << "Error: Archivo no existe\n";
        return ERROR;
    }
    
    cout << "Archivo: " << a->nombre << "\n\n";
    
    if (a->primeraVersion == NULL) {
        cout << "Sin versiones\n";
        return OK;
    }
    
    // Array para construir números de versión jerárquicos
    int numerosPorNivel[100];  // Asumiendo máximo 100 niveles de profundidad
    
    // Recorrer todas las versiones de primer nivel
    Version v = a->primeraVersion;
    while (v != NULL) {
        numerosPorNivel[0] = v->numero;
        mostrarArbolRecursivo(v, 0, numerosPorNivel, 1);
        v = v->siguienteHermano;
    }
    
    return OK;
}

// Esta función inserta una linea de texto a la version parámetro en la posición nroLinea.
TipoRet InsertarLinea(Archivo &a, char * version, char * linea, unsigned int nroLinea, char * error){
	if (a == nullptr) {
		strcpy(error, "Archivo no existe");
		return ERROR;
	}
	
	if (version == nullptr || linea == nullptr) {
		strcpy(error, "Version o linea son NULL");
		return ERROR;
	}
	
	if (nroLinea < 1) {
		strcpy(error, "Numero de linea invalido (debe ser >= 1)");
		return ERROR;
	}
	
	// Parsear la versión jerárquica
	int* secuencia = nullptr;
	int longitud = 0;
	secuencia = parsearVersion(version, &longitud);
	
	if (secuencia == nullptr) {
		strcpy(error, "Version invalida");
		return ERROR;
	}
	
	// Navegar al nodo de la versión
	Version ver = navegarAVersion(a->primeraVersion, secuencia, longitud);
	
	if (ver == nullptr) {
		liberarArrayVersion(secuencia);
		strcpy(error, "Version no existe");
		return ERROR;
	}
	
	// Validar rango de línea - necesitamos el texto CON ancestros
	Version camino[100];
	int longitudCamino = 0;
	obtenerCaminoAncestros(ver, camino, &longitudCamino);
	
	ListaLineas texto = crearListaLineas();
	for (int i = 0; i < longitudCamino; i++) {
		aplicarModificaciones(texto, camino[i]->primeraModificacion);
	}
	
	unsigned int numLineas = contarLineas(texto);
	
	// nroLinea debe estar entre 1 y numLineas+1
	if (nroLinea > numLineas + 1) {
		liberarListaLineas(texto);
		liberarArrayVersion(secuencia);
		strcpy(error, "Numero de linea fuera de rango (muy grande)");
		return ERROR;
	}
	
	liberarListaLineas(texto);
	liberarArrayVersion(secuencia);
	
	Modificacion mod = crearModificacion(INSERCION, nroLinea, linea);
	agregarModificacion(ver, mod);
	
	return OK;
}

// Esta función elimina una línea de texto de la version del archivo a en la posición nroLinea.
TipoRet BorrarLinea(Archivo &a, char * version, unsigned int nroLinea, char * error){
	if (a == nullptr) {
		strcpy(error, "Archivo no existe");
		return ERROR;
	}
	
	if (version == nullptr) {
		strcpy(error, "Version es NULL");
		return ERROR;
	}
	
	if (nroLinea < 1) {
		strcpy(error, "Numero de linea invalido (debe ser >= 1)");
		return ERROR;
	}
	
	// Parsear la versión jerárquica
	int* secuencia = nullptr;
	int longitud = 0;
	secuencia = parsearVersion(version, &longitud);
	
	if (secuencia == nullptr) {
		strcpy(error, "Version invalida");
		return ERROR;
	}
	
	// Navegar al nodo de la versión
	Version ver = navegarAVersion(a->primeraVersion, secuencia, longitud);
	
	if (ver == nullptr) {
		liberarArrayVersion(secuencia);
		strcpy(error, "Version no existe");
		return ERROR;
	}
	
	// Validar rango - necesitamos el texto CON ancestros
	Version camino[100];
	int longitudCamino = 0;
	obtenerCaminoAncestros(ver, camino, &longitudCamino);
	
	ListaLineas texto = crearListaLineas();
	for (int i = 0; i < longitudCamino; i++) {
		aplicarModificaciones(texto, camino[i]->primeraModificacion);
	}
	
	unsigned int numLineas = contarLineas(texto);
	
	if (numLineas == 0) {
		liberarListaLineas(texto);
		liberarArrayVersion(secuencia);
		strcpy(error, "No hay lineas para borrar (version vacia)");
		return ERROR;
	}
	
	if (nroLinea > numLineas) {
		liberarListaLineas(texto);
		liberarArrayVersion(secuencia);
		strcpy(error, "Numero de linea fuera de rango (no existe)");
		return ERROR;
	}
	
	liberarListaLineas(texto);
	liberarArrayVersion(secuencia);
	
	Modificacion mod = crearModificacion(BORRADO, nroLinea, nullptr);
	agregarModificacion(ver, mod);
	
	return OK;
}

// Esta función muestra el texto completo de la version, teniendo en cuenta los cambios realizados en dicha versión y en las versiones ancestras, de la cual ella depende.
TipoRet MostrarTexto(Archivo a, char * version){
	if (a == nullptr) {
		return ERROR;
	}

	// Parsear la versión jerárquica (ej: "1.2.3")
	int* secuencia = nullptr;
	int longitud = 0;
	secuencia = parsearVersion(version, &longitud);
	
	if (secuencia == nullptr) {
		return ERROR;
	}
	
	// Navegar al nodo de la versión
	Version ver = navegarAVersion(a->primeraVersion, secuencia, longitud);
	
	if (ver == nullptr) {
		liberarArrayVersion(secuencia);
		return ERROR;
	}
	
	// Obtener camino de ancestros (raíz → versión objetivo)
	Version camino[100];  // Máximo 100 niveles de profundidad
	int longitudCamino = 0;
	obtenerCaminoAncestros(ver, camino, &longitudCamino);
	
	// Crear texto vacío
	ListaLineas texto = crearListaLineas();
	
	// Aplicar modificaciones de todos los ancestros en orden (raíz → hoja)
	for (int i = 0; i < longitudCamino; i++) {
		aplicarModificaciones(texto, camino[i]->primeraModificacion);
	}
	
	// Mostrar el texto reconstruido
	cout << "Archivo: " << a->nombre << " - Version " << version << endl << endl;
	
	if (texto == nullptr || contarLineas(texto) == 0) {
		cout << "(versión sin líneas)" << endl;
	} else {
		imprimirListaLineas(texto);
	}
	
	// Liberar memoria
	liberarListaLineas(texto);
	liberarArrayVersion(secuencia);
	
	return OK;
}

// Esta función muestra los cambios que se realizaron en el texto de la version parámetro, sin incluir los cambios realizados en las versiones ancestras de la cual dicha versión depende.
TipoRet MostrarCambios(Archivo a, char * version){
	if (a == nullptr) {
		return ERROR;
	}
	
	// Parsear la versión jerárquica
	int* secuencia = nullptr;
	int longitud = 0;
	secuencia = parsearVersion(version, &longitud);
	
	if (secuencia == nullptr) {
		return ERROR;
	}
	
	// Navegar al nodo de la versión
	Version ver = navegarAVersion(a->primeraVersion, secuencia, longitud);
	
	if (ver == nullptr) {
		liberarArrayVersion(secuencia);
		return ERROR;
	}
	
	// Mostrar encabezado
	cout << "Cambios de la version " << version << ":\n\n";
	
	// Verificar si hay modificaciones
	if (ver->primeraModificacion == nullptr) {
		cout << "(sin modificaciones propias)\n";
		liberarArrayVersion(secuencia);
		return OK;
	}
	
	// Recorrer SOLO las modificaciones de esta versión (NO ancestros)
	Modificacion actual = ver->primeraModificacion;
	while (actual != nullptr) {
		if (actual->tipo == INSERCION) {
			cout << "INSERCION(" << actual->nroLinea << ", \"" 
			     << actual->textoLinea << "\")\n";
		} else {  // BORRADO
			cout << "BORRADO(" << actual->nroLinea << ")\n";
		}
		actual = actual->siguiente;
	}
	
	// Liberar memoria
	liberarArrayVersion(secuencia);
	
	return OK;
}

// Esta función asigna al parámetro booleano (iguales) el valor true si ambas versiones (version1 y version2) del archivo tienen exactamente el mismo texto, y false en caso contrario.
TipoRet Iguales(Archivo a, char * version1, char * version2, bool &iguales){
	return NO_IMPLEMENTADA;
}

// Esta función crea una nueva versión al final del primer nivel con todos los cambios de la version especificada y de sus versiones ancestras. La versión que se crea debe ser independiente de cualquier otra versión.
TipoRet VersionIndependiente(Archivo &a, char * version){
	return NO_IMPLEMENTADA;
}
