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
	if (nombre == NULL || strlen(nombre) == 0) {
        return NULL;
    }
	Archivo a = new nodo_archivo;
	a->nombre = new char[strlen(nombre) + 1];
	strcpy(a->nombre, nombre);
	a->primeraVersion = NULL;
	
	return a;
}

// Elimina toda la memoria utilizada por el archivo y asigna NULL al puntero a.
TipoRet BorrarArchivo(Archivo &a){
	if (a == NULL) {
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
	if (a == NULL) {
		strcpy(error, "Archivo no existe");
		return ERROR;
	}
	
	if (version == NULL || strlen(version) == 0) {
		strcpy(error, "Version invalida");
		return ERROR;
	}
	
	// Parsear versión
	int* secuencia = NULL;
	int longitud = 0;
	secuencia = parsearVersion(version, &longitud);
	
	if (secuencia == NULL) {
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
	Version padre = NULL;
	if (longitud > 1) {
		padre = navegarAVersion(a->primeraVersion, secuencia, longitud - 1);
		if (padre == NULL) {
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
	Version hermanos = (padre != NULL) ? padre->primerHijo : a->primeraVersion;
	Version existente = (padre != NULL) ? buscarHijo(padre, numeroNuevo) : buscarVersionEnLista(hermanos, numeroNuevo);
	
	if (existente != NULL) {
		desplazarYRenumerar(padre, a->primeraVersion, numeroNuevo, 1);
	}
	
	// Crear nodo
	Version nuevaVersion = crearVersion(numeroNuevo);
	if (nuevaVersion == NULL) {
		strcpy(error, "Error al crear nodo de version");
		liberarArrayVersion(secuencia);
		return ERROR;
	}
	
	nuevaVersion->padre = padre;
	
	// Insertar en lista ordenada
	if (padre != NULL) {
		if (padre->primerHijo == NULL) {
			padre->primerHijo = nuevaVersion;
		} else {
			Version actual = padre->primerHijo;
			Version anterior = NULL;
			
			while (actual != NULL && actual->numero < numeroNuevo) {
				anterior = actual;
				actual = actual->siguienteHermano;
			}
			
			if (anterior == NULL) {
				nuevaVersion->siguienteHermano = padre->primerHijo;
				padre->primerHijo = nuevaVersion;
			} else {
				nuevaVersion->siguienteHermano = anterior->siguienteHermano;
				anterior->siguienteHermano = nuevaVersion;
			}
		}
	} else {
		if (a->primeraVersion == NULL) {
			a->primeraVersion = nuevaVersion;
		} else {
			Version actual = a->primeraVersion;
			Version anterior = NULL;
			
			while (actual != NULL && actual->numero < numeroNuevo) {
				anterior = actual;
				actual = actual->siguienteHermano;
			}
			
			if (anterior == NULL) {
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
	if (a == NULL || version == NULL) {
		return ERROR;
	}
	
	// 1. Parsear la versión
	int* secuencia = NULL;
	int longitud = 0;
	secuencia = parsearVersion(version, &longitud);
	
	if (secuencia == NULL) {
		return ERROR;
	}
	
	// 2. Caso especial: Única versión "1"
	if (longitud == 1 && secuencia[0] == 1) {
		// Verificar si es la única versión
		if (a->primeraVersion != NULL && 
		    a->primeraVersion->numero == 1 && 
		    a->primeraVersion->siguienteHermano == NULL) {
			// Es la única versión, liberar todo y dejar archivo vacío
			liberarArbolVersiones(a->primeraVersion);
			a->primeraVersion = NULL;
			liberarArrayVersion(secuencia);
			return OK;
		}
	}
	
	// 3. Navegar a la versión a borrar
	Version ver = navegarAVersion(a->primeraVersion, secuencia, longitud);
	
	if (ver == NULL) {
		liberarArrayVersion(secuencia);
		return ERROR;
	}
	
	// 4. Encontrar padre y hermano anterior
	Version padre = ver->padre;
	Version hermanoAnterior = NULL;
	Version hermanoSiguiente = ver->siguienteHermano;
	
	// Buscar hermano anterior
	if (padre != NULL) {
		// No es de primer nivel, buscar en hijos del padre
		Version actual = padre->primerHijo;
		while (actual != NULL && actual->siguienteHermano != ver) {
			actual = actual->siguienteHermano;
		}
		hermanoAnterior = actual;
	} else {
		// Es de primer nivel, buscar en primeraVersion
		if (a->primeraVersion == ver) {
			// Es el primero, no hay hermano anterior
			hermanoAnterior = NULL;
		} else {
			Version actual = a->primeraVersion;
			while (actual != NULL && actual->siguienteHermano != ver) {
				actual = actual->siguienteHermano;
			}
			hermanoAnterior = actual;
		}
	}
	
	// 5. Actualizar punteros
	if (hermanoAnterior != NULL) {
		// Hay hermano anterior, actualizar su siguiente
		hermanoAnterior->siguienteHermano = hermanoSiguiente;
	} else {
		// No hay hermano anterior, es el primero
		if (padre != NULL) {
			// Actualizar primerHijo del padre
			padre->primerHijo = hermanoSiguiente;
		} else {
			// Actualizar primeraVersion del archivo
			a->primeraVersion = hermanoSiguiente;
		}
	}
	
	// 6. Desconectar el nodo antes de liberar
	ver->siguienteHermano = NULL;
	
	// 7. Liberar el nodo y sus hijos (NO hermanos porque ya los desconectamos)
	liberarArbolVersiones(ver);
	
	// 8. Renumerar hermanos posteriores (decrementar en 1)
	if (hermanoSiguiente != NULL) {
		renumerarHermanosPosteriores(hermanoSiguiente, -1);
	}
	
	// 9. Liberar memoria del parser
	liberarArrayVersion(secuencia);
	
	return OK;
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
	if (a == NULL) {
		strcpy(error, "Archivo no existe");
		return ERROR;
	}
	
	if (version == NULL || linea == NULL) {
		strcpy(error, "Version o linea son NULL");
		return ERROR;
	}
	
	if (nroLinea < 1) {
		strcpy(error, "Numero de linea invalido (debe ser >= 1)");
		return ERROR;
	}
	
	// Parsear la versión jerárquica
	int* secuencia = NULL;
	int longitud = 0;
	secuencia = parsearVersion(version, &longitud);
	
	if (secuencia == NULL) {
		strcpy(error, "Version invalida");
		return ERROR;
	}
	
	// Navegar al nodo de la versión
	Version ver = navegarAVersion(a->primeraVersion, secuencia, longitud);
	
	if (ver == NULL) {
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
	if (a == NULL) {
		strcpy(error, "Archivo no existe");
		return ERROR;
	}
	
	if (version == NULL) {
		strcpy(error, "Version es NULL");
		return ERROR;
	}
	
	if (nroLinea < 1) {
		strcpy(error, "Numero de linea invalido (debe ser >= 1)");
		return ERROR;
	}
	
	// Parsear la versión jerárquica
	int* secuencia = NULL;
	int longitud = 0;
	secuencia = parsearVersion(version, &longitud);
	
	if (secuencia == NULL) {
		strcpy(error, "Version invalida");
		return ERROR;
	}
	
	// Navegar al nodo de la versión
	Version ver = navegarAVersion(a->primeraVersion, secuencia, longitud);
	
	if (ver == NULL) {
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
	
	Modificacion mod = crearModificacion(BORRADO, nroLinea, NULL);
	agregarModificacion(ver, mod);
	
	return OK;
}

// Esta función muestra el texto completo de la version, teniendo en cuenta los cambios realizados en dicha versión y en las versiones ancestras, de la cual ella depende.
TipoRet MostrarTexto(Archivo a, char * version){
	if (a == NULL) {
		return ERROR;
	}

	// Parsear la versión jerárquica (ej: "1.2.3")
	int* secuencia = NULL;
	int longitud = 0;
	secuencia = parsearVersion(version, &longitud);
	
	if (secuencia == NULL) {
		return ERROR;
	}
	
	// Navegar al nodo de la versión
	Version ver = navegarAVersion(a->primeraVersion, secuencia, longitud);
	
	if (ver == NULL) {
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
	
	if (texto == NULL || contarLineas(texto) == 0) {
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
	if (a == NULL) {
		return ERROR;
	}
	
	// Parsear la versión jerárquica
	int* secuencia = NULL;
	int longitud = 0;
	secuencia = parsearVersion(version, &longitud);
	
	if (secuencia == NULL) {
		return ERROR;
	}
	
	// Navegar al nodo de la versión
	Version ver = navegarAVersion(a->primeraVersion, secuencia, longitud);
	
	if (ver == NULL) {
		liberarArrayVersion(secuencia);
		return ERROR;
	}
	
	// Mostrar encabezado
	cout << "Cambios de la version " << version << ":\n\n";
	
	// Verificar si hay modificaciones
	if (ver->primeraModificacion == NULL) {
		cout << "(sin modificaciones propias)\n";
		liberarArrayVersion(secuencia);
		return OK;
	}
	
	// Recorrer SOLO las modificaciones de esta versión (NO ancestros)
	Modificacion actual = ver->primeraModificacion;
	while (actual != NULL) {
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
    // Validaciones iniciales
    if (a == NULL || version1 == NULL || version2 == NULL) {
        return ERROR;
    }
    
    // 1. Parsear y navegar a la primera versión
    int longitud1 = 0;
    int* secuencia1 = parsearVersion(version1, &longitud1);
    if (secuencia1 == NULL) {
        return ERROR;
    }
    
    Version v1 = navegarAVersion(a->primeraVersion, secuencia1, longitud1);
    if (v1 == NULL) {
        liberarArrayVersion(secuencia1);
        return ERROR;
    }
    
    // 2. Parsear y navegar a la segunda versión
    int longitud2 = 0;
    int* secuencia2 = parsearVersion(version2, &longitud2);
    if (secuencia2 == NULL) {
        liberarArrayVersion(secuencia1);
        return ERROR;
    }
    
    Version v2 = navegarAVersion(a->primeraVersion, secuencia2, longitud2);
    if (v2 == NULL) {
        liberarArrayVersion(secuencia1);
        liberarArrayVersion(secuencia2);
        return ERROR;
    }
    
    // 3. Obtener camino de ancestros para v1
    int longitudCamino1 = 0;
    Version* camino1 = new Version[100];
    obtenerCaminoAncestros(v1, camino1, &longitudCamino1);
    
    // 4. Obtener camino de ancestros para v2
    int longitudCamino2 = 0;
    Version* camino2 = new Version[100];
    obtenerCaminoAncestros(v2, camino2, &longitudCamino2);
    
    // 5. Reconstruir textos usando arrays dinámicos simples
    // Usaremos arrays de strings para evitar complejidad con listas enlazadas
    const int MAX_LINEAS = 1000;
    char** texto1 = new char*[MAX_LINEAS];
    int cantLineas1 = 0;
    for (int i = 0; i < MAX_LINEAS; i++) texto1[i] = NULL;
    
    char** texto2 = new char*[MAX_LINEAS];
    int cantLineas2 = 0;
    for (int i = 0; i < MAX_LINEAS; i++) texto2[i] = NULL;
    
    // Aplicar modificaciones de ancestros de v1
    for (int i = 0; i < longitudCamino1; i++) {
        Version ancestro = camino1[i];
        Modificacion mod = ancestro->primeraModificacion;
        
        while (mod != NULL) {
            if (mod->tipo == INSERCION) {
                // Inserción: desplazar líneas hacia abajo e insertar
                unsigned int pos = mod->nroLinea;
                if (pos >= 1 && pos <= (unsigned int)(cantLineas1 + 1) && cantLineas1 < MAX_LINEAS) {
                    // Desplazar hacia abajo
                    for (int j = cantLineas1; j >= (int)pos; j--) {
                        texto1[j] = texto1[j-1];
                    }
                    // Insertar nueva línea
                    if (mod->textoLinea != NULL) {
                        texto1[pos-1] = new char[strlen(mod->textoLinea) + 1];
                        strcpy(texto1[pos-1], mod->textoLinea);
                    } else {
                        texto1[pos-1] = new char[1];
                        texto1[pos-1][0] = '\0';
                    }
                    cantLineas1++;
                }
            } else {
                // Borrado: eliminar línea y desplazar hacia arriba
                unsigned int pos = mod->nroLinea;
                if (pos >= 1 && pos <= (unsigned int)cantLineas1) {
                    if (texto1[pos-1] != NULL) {
                        delete[] texto1[pos-1];
                    }
                    // Desplazar hacia arriba
                    for (int j = pos-1; j < cantLineas1-1; j++) {
                        texto1[j] = texto1[j+1];
                    }
                    texto1[cantLineas1-1] = NULL;
                    cantLineas1--;
                }
            }
            mod = mod->siguiente;
        }
    }
    
    // Aplicar modificaciones de ancestros de v2
    for (int i = 0; i < longitudCamino2; i++) {
        Version ancestro = camino2[i];
        Modificacion mod = ancestro->primeraModificacion;
        
        while (mod != NULL) {
            if (mod->tipo == INSERCION) {
                unsigned int pos = mod->nroLinea;
                if (pos >= 1 && pos <= (unsigned int)(cantLineas2 + 1) && cantLineas2 < MAX_LINEAS) {
                    for (int j = cantLineas2; j >= (int)pos; j--) {
                        texto2[j] = texto2[j-1];
                    }
                    if (mod->textoLinea != NULL) {
                        texto2[pos-1] = new char[strlen(mod->textoLinea) + 1];
                        strcpy(texto2[pos-1], mod->textoLinea);
                    } else {
                        texto2[pos-1] = new char[1];
                        texto2[pos-1][0] = '\0';
                    }
                    cantLineas2++;
                }
            } else {
                unsigned int pos = mod->nroLinea;
                if (pos >= 1 && pos <= (unsigned int)cantLineas2) {
                    if (texto2[pos-1] != NULL) {
                        delete[] texto2[pos-1];
                    }
                    for (int j = pos-1; j < cantLineas2-1; j++) {
                        texto2[j] = texto2[j+1];
                    }
                    texto2[cantLineas2-1] = NULL;
                    cantLineas2--;
                }
            }
            mod = mod->siguiente;
        }
    }
    
    // 6. Comparar
    if (cantLineas1 != cantLineas2) {
        iguales = false;
    } else {
        iguales = true;
        for (int i = 0; i < cantLineas1 && iguales; i++) {
            if (texto1[i] == NULL || texto2[i] == NULL) {
                iguales = false;
            } else if (strcmp(texto1[i], texto2[i]) != 0) {
                iguales = false;
            }
        }
    }
    
    // 7. Liberar memoria
    for (int i = 0; i < cantLineas1; i++) {
        if (texto1[i] != NULL) {
            delete[] texto1[i];
        }
    }
    delete[] texto1;
    
    for (int i = 0; i < cantLineas2; i++) {
        if (texto2[i] != NULL) {
            delete[] texto2[i];
        }
    }
    delete[] texto2;
    
    delete[] camino1;
    delete[] camino2;
    liberarArrayVersion(secuencia1);
    liberarArrayVersion(secuencia2);
    
    return OK;
}

// Esta función crea una nueva versión al final del primer nivel con todos los cambios de la version especificada y de sus versiones ancestras. La versión que se crea debe ser independiente de cualquier otra versión.
TipoRet VersionIndependiente(Archivo &a, char * version){
	if (a == NULL || version == NULL) {
		return ERROR;
	}
	
	// 1. Parsear y navegar a la versión origen
	int longitud = 0;
	int* secuencia = parsearVersion(version, &longitud);
	if (secuencia == NULL) {
		return ERROR;
	}
	
	Version versionOrigen = navegarAVersion(a->primeraVersion, secuencia, longitud);
	if (versionOrigen == NULL) {
		liberarArrayVersion(secuencia);
		return ERROR;
	}
	
	// 2. Obtener camino de ancestros (igual que en Iguales)
	int longitudCamino = 0;
	Version* camino = new Version[100];
	obtenerCaminoAncestros(versionOrigen, camino, &longitudCamino);
	
	// 3. Reconstruir texto completo aplicando todas las modificaciones (igual que en Iguales)
	const int MAX_LINEAS = 1000;
	char** textoCompleto = new char*[MAX_LINEAS];
	int cantLineas = 0;
	for (int i = 0; i < MAX_LINEAS; i++) textoCompleto[i] = NULL;
	
	// Aplicar modificaciones de todos los ancestros en orden
	for (int i = 0; i < longitudCamino; i++) {
		Version ancestro = camino[i];
		Modificacion mod = ancestro->primeraModificacion;
		
		while (mod != NULL) {
			if (mod->tipo == INSERCION) {
				unsigned int pos = mod->nroLinea;
				if (pos >= 1 && pos <= (unsigned int)(cantLineas + 1) && cantLineas < MAX_LINEAS) {
					// Desplazar líneas hacia abajo
					for (int j = cantLineas; j >= (int)pos; j--) {
						textoCompleto[j] = textoCompleto[j-1];
					}
					// Insertar nueva línea
					if (mod->textoLinea != NULL) {
						textoCompleto[pos-1] = new char[strlen(mod->textoLinea) + 1];
						strcpy(textoCompleto[pos-1], mod->textoLinea);
					} else {
						textoCompleto[pos-1] = new char[1];
						textoCompleto[pos-1][0] = '\0';
					}
					cantLineas++;
				}
			} else {  // BORRADO
				unsigned int pos = mod->nroLinea;
				if (pos >= 1 && pos <= (unsigned int)cantLineas) {
					if (textoCompleto[pos-1] != NULL) {
						delete[] textoCompleto[pos-1];
					}
					// Desplazar líneas hacia arriba
					for (int j = pos-1; j < cantLineas-1; j++) {
						textoCompleto[j] = textoCompleto[j+1];
					}
					textoCompleto[cantLineas-1] = NULL;
					cantLineas--;
				}
			}
			mod = mod->siguiente;
		}
	}
	
	// 4. Encontrar último número de nivel 1
	int ultimoNumero = 0;
	Version actual = a->primeraVersion;
	while (actual != NULL) {
		if (actual->numero > ultimoNumero) {
			ultimoNumero = actual->numero;
		}
		actual = actual->siguienteHermano;
	}
	
	int numeroNuevo = ultimoNumero + 1;
	
	// 5. Crear nueva versión de nivel 1 (sin padre, independiente)
	Version nuevaVersion = crearVersion(numeroNuevo);
	if (nuevaVersion == NULL) {
		// Liberar memoria temporal
		for (int i = 0; i < cantLineas; i++) {
			if (textoCompleto[i] != NULL) {
				delete[] textoCompleto[i];
			}
		}
		delete[] textoCompleto;
		delete[] camino;
		liberarArrayVersion(secuencia);
		return ERROR;
	}
	
	nuevaVersion->padre = NULL;  // ¡CLAVE! Es independiente, sin padre
	
	// 6. Agregar nueva versión al final de la lista de nivel 1
	if (a->primeraVersion == NULL) {
		a->primeraVersion = nuevaVersion;
	} else {
		Version ultimo = a->primeraVersion;
		while (ultimo->siguienteHermano != NULL) {
			ultimo = ultimo->siguienteHermano;
		}
		ultimo->siguienteHermano = nuevaVersion;
	}
	
	// 7. Convertir cada línea del texto reconstruido en una INSERCION nueva
	// Esto hace que la nueva versión sea independiente - no necesita ancestros
	for (int i = 0; i < cantLineas; i++) {
		if (textoCompleto[i] != NULL) {
			Modificacion mod = crearModificacion(INSERCION, i + 1, textoCompleto[i]);
			if (mod != NULL) {
				agregarModificacion(nuevaVersion, mod);
			}
		}
	}
	
	// 8. Liberar memoria temporal
	for (int i = 0; i < cantLineas; i++) {
		if (textoCompleto[i] != NULL) {
			delete[] textoCompleto[i];
		}
	}
	delete[] textoCompleto;
	delete[] camino;
	liberarArrayVersion(secuencia);
	
	return OK;
}
