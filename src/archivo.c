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


Archivo CrearArchivo(char * nombre){
// Crea el archivo con el nombre especificado y lo inicializa sin contenido (vacío).
// El archivo creado es retornado.
// Esta operación se ejecuta al inicio de una sesión de trabajo con un archivo.
	// Validación: nombre no puede ser NULL o vacío
	if (nombre == nullptr || strlen(nombre) == 0) {
        return nullptr;  // O manejar error de otra forma
    }
	Archivo a = new nodo_archivo;
	a->nombre = new char[strlen(nombre) + 1];
	strcpy(a->nombre, nombre);
	a->primeraVersion = nullptr;  // Archivo inicialmente sin versiones
	
	return a;
}


TipoRet BorrarArchivo(Archivo &a){
// Elimina toda la memoria utilizada por el archivo y asigna NULL al puntero a.
// Se asume como precondición que a referencia a un archivo (en particular a es distinto a NULL).
// Esta operación se ejecuta al final de una sesión de trabajo con un archivo.
	
	// Validar que el archivo existe
	if (a == nullptr) {
		return ERROR;
	}
	
	// Liberar todas las versiones (y recursivamente todas las modificaciones)
	// Esta función libera:
	// - Todas las versiones en la lista de hermanos
	// - Para cada versión: todas sus modificaciones
	// - Para cada modificación: el texto si es INSERCION
	liberarArbolVersiones(a->primeraVersion);
	
	// Liberar el nombre del archivo
	delete[] a->nombre;
	
	// Liberar la estructura del archivo
	delete a;
	
	// Asignar NULL al puntero
	a = nullptr;
	
	return OK;
}


TipoRet CrearVersion(Archivo &a, char * version, char * error){
// Crea una nueva versión del archivo si la versión especificada cumple con las siguientes reglas:
// - El padre de la nueva versión a crear ya debe existir. Por ejemplo, si creo la versión 2.15.1, la versión 2.15 ya debe existir.
// Las versiones del primer nivel no siguen esta regla, ya que no tienen versión padre.
// - No pueden quedar "huecos" entre versiones hermanas. Por ejemplo, si creamos la versión 2.15.3, las versiones 2.15.1 y 2.15.2 ya deben existir.
// Ver ejemplo en la letra.

	// Validación: archivo debe existir
	if (a == nullptr) {
		strcpy(error, "Archivo no existe");
		return ERROR;
	}
	
	// Validación: string de versión debe ser válido
	if (version == nullptr || strlen(version) == 0) {
		strcpy(error, "Version invalida");
		return ERROR;
	}
	
	// PASO 1: PARSEAR STRING DE VERSIÓN
	int* secuencia = nullptr;
	int longitud = 0;
	secuencia = parsearVersion(version, &longitud);
	
	if (secuencia == nullptr) {
		strcpy(error, "Formato de version invalido");
		return ERROR;
	}
	
	// PASO 2: VALIDAR QUE EL PADRE EXISTE
	if (!validarPadreExiste(a->primeraVersion, secuencia, longitud)) {
		strcpy(error, "El padre de la version no existe");
		liberarArrayVersion(secuencia);
		return ERROR;
	}
	
	// PASO 3: OBTENER NÚMERO DE LA NUEVA VERSIÓN
	int numeroNuevo = secuencia[longitud - 1];
	
	// PASO 4: DETERMINAR EL PADRE
	Version padre = nullptr;
	if (longitud > 1) {
		// Es una subversión, navegar al padre
		padre = navegarAVersion(a->primeraVersion, secuencia, longitud - 1);
		if (padre == nullptr) {
			strcpy(error, "Error al navegar al padre");
			liberarArrayVersion(secuencia);
			return ERROR;
		}
	}
	// Si longitud == 1, padre = nullptr (es versión de primer nivel)
	
	// PASO 5: VALIDAR SIN HUECOS
	if (!validarSinHuecos(padre, a->primeraVersion, numeroNuevo)) {
		strcpy(error, "Crear la version generaria huecos en la numeracion");
		liberarArrayVersion(secuencia);
		return ERROR;
	}
	
	// PASO 6: VERIFICAR SI EL NÚMERO YA EXISTE (DESPLAZAMIENTO)
	Version hermanos = (padre != nullptr) ? padre->primerHijo : a->primeraVersion;
	Version existente = (padre != nullptr) ? buscarHijo(padre, numeroNuevo) : buscarVersionEnLista(hermanos, numeroNuevo);
	
	if (existente != nullptr) {
		// El número ya existe, desplazar hermanos posteriores hacia la derecha
		desplazarYRenumerar(padre, a->primeraVersion, numeroNuevo, 1);
	}
	
	// PASO 7: CREAR NUEVO NODO DE VERSIÓN
	Version nuevaVersion = crearVersion(numeroNuevo);
	if (nuevaVersion == nullptr) {
		strcpy(error, "Error al crear nodo de version");
		liberarArrayVersion(secuencia);
		return ERROR;
	}
	
	// PASO 8: ESTABLECER PUNTERO AL PADRE
	nuevaVersion->padre = padre;
	
	// PASO 9: INSERTAR EN LA LISTA DE HERMANOS
	// Necesitamos insertar en orden numérico para mantener la lista ordenada
	if (padre != nullptr) {
		// Es una subversión, insertar en la lista de hijos del padre
		if (padre->primerHijo == nullptr) {
			// No hay hijos, este es el primero
			padre->primerHijo = nuevaVersion;
		} else {
			// Hay hijos, insertar en orden numérico
			Version actual = padre->primerHijo;
			Version anterior = nullptr;
			
			// Buscar posición correcta
			while (actual != nullptr && actual->numero < numeroNuevo) {
				anterior = actual;
				actual = actual->siguienteHermano;
			}
			
			// Insertar en la posición encontrada
			if (anterior == nullptr) {
				// Insertar al principio
				nuevaVersion->siguienteHermano = padre->primerHijo;
				padre->primerHijo = nuevaVersion;
			} else {
				// Insertar después de anterior
				nuevaVersion->siguienteHermano = anterior->siguienteHermano;
				anterior->siguienteHermano = nuevaVersion;
			}
		}
	} else {
		// Es versión de primer nivel, insertar en a->primeraVersion
		if (a->primeraVersion == nullptr) {
			// No hay versiones, esta es la primera
			a->primeraVersion = nuevaVersion;
		} else {
			// Hay versiones, insertar en orden numérico
			Version actual = a->primeraVersion;
			Version anterior = nullptr;
			
			// Buscar posición correcta
			while (actual != nullptr && actual->numero < numeroNuevo) {
				anterior = actual;
				actual = actual->siguienteHermano;
			}
			
			// Insertar en la posición encontrada
			if (anterior == nullptr) {
				// Insertar al principio
				nuevaVersion->siguienteHermano = a->primeraVersion;
				a->primeraVersion = nuevaVersion;
			} else {
				// Insertar después de anterior
				nuevaVersion->siguienteHermano = anterior->siguienteHermano;
				anterior->siguienteHermano = nuevaVersion;
			}
		}
	}
	
	// PASO 10: LIBERAR MEMORIA TEMPORAL
	liberarArrayVersion(secuencia);
	
	// Éxito
	return OK;
}

TipoRet BorrarVersion(Archivo &a, char * version){
// Elimina una versión del archivo si la version pasada por parámetro existe. En otro caso la operación quedará sin efecto.
// Si la versión a eliminar posee subversiones, éstas deberán ser eliminadas también, así como el texto asociado a cada una de las versiones.
// No deben quedar números de versiones libres sin usar. Por lo tanto cuando se elimina una versión, las versiones hermanas que le siguen deben decrementar su numeración (así también sus subversiones dependientes). Por ejemplo, si existen las versiones 2.15.1, 2.15.2 y 2.15.3, y elimino la 2.15.1, la versión 2.15.2 y la 2.15.3 pasan a ser 2.15.1 y 2.15.2 respectivamente, esto incluye a todas las subversiones de estas versiones.

	return NO_IMPLEMENTADA;
}

TipoRet MostrarVersiones(Archivo a){
// FORMATO: En primer lugar muestra el nombre del archivo. Después de una línea en blanco lista todos las versiones del archivo
// ordenadas por nivel jerárquico e indentadas según muestra el ejemplo publicado en la letra (cada nivel está indentado por un tabulador).

	return NO_IMPLEMENTADA;
}

TipoRet InsertarLinea(Archivo &a, char * version, char * linea, unsigned int nroLinea, char * error){
// Esta función inserta una linea de texto a la version parámetro en la posición nroLinea.
// El número de línea debe estar entre 1 y n+1, siendo n la cantidad de líneas del archivo. Por ejemplo, si el texto tiene 7 líneas, se podráinsertar en las posiciones 1 (al comienzo) a 8 (al final).
// Si se inserta en un número de línea existente, ésta y las siguientes líneas se correrán hacia adelante (abajo) dejando el espacio para la nueva línea.
// No se puede insertar una línea en una versión que tenga subversiones.
// Notar que el crear un archivo, éste no es editable hasta que no se crea al menos una versión del mismo. Sólo las versiones de un archivo son editables (se pueden insertar o suprimir líneas), siempre que no tengan subversiones creadas.
// En caso que TipoRet sea ERROR, en error se debe cargar cuál es el mismo.

	// Validación: archivo debe existir
	if (a == nullptr) {
		strcpy(error, "Archivo no existe");
		return ERROR;
	}
	
	// Validación: versión y linea no pueden ser NULL
	if (version == nullptr || linea == nullptr) {
		strcpy(error, "Version o linea son NULL");
		return ERROR;
	}
	
	// Validación: nroLinea debe ser al menos 1
	if (nroLinea < 1) {
		strcpy(error, "Numero de linea invalido (debe ser >= 1)");
		return ERROR;
	}
	
	// FASE 1: Solo versiones de primer nivel (solo números: "1", "2", "3"...)
	// Parsear número de versión
	int numVersion = atoi(version);
	
	if (numVersion <= 0) {
		strcpy(error, "Version invalida (debe ser numero positivo)");
		return ERROR;
	}
	
	// Buscar la versión en el archivo
	Version ver = buscarVersionEnLista(a->primeraVersion, numVersion);
	
	// Si la versión NO existe, crearla
	if (ver == nullptr) {
		// Crear nueva versión
		ver = crearVersion(numVersion);
		
		// Agregar a la lista de versiones del archivo
		if (a->primeraVersion == nullptr) {
			// Es la primera versión
			a->primeraVersion = ver;
		} else {
			// Agregar al final de la lista
			Version actual = a->primeraVersion;
			while (actual->siguienteHermano != nullptr) {
				actual = actual->siguienteHermano;
			}
			actual->siguienteHermano = ver;
		}
	}
	
	// Validar que nroLinea sea válido (1 a n+1 donde n = líneas actuales)
	// Para saber cuántas líneas tiene, necesitamos reconstruir el texto
	ListaLineas texto = crearListaLineas();
	aplicarModificaciones(texto, ver->primeraModificacion);
	unsigned int numLineas = contarLineas(texto);
	
	// nroLinea debe estar entre 1 y numLineas+1
	if (nroLinea > numLineas + 1) {
		liberarListaLineas(texto);
		strcpy(error, "Numero de linea fuera de rango (muy grande)");
		return ERROR;
	}
	
	// Liberar texto temporal (ya no lo necesitamos)
	liberarListaLineas(texto);
	
	// Crear modificación de tipo INSERCION
	Modificacion mod = crearModificacion(INSERCION, nroLinea, linea);
	
	// Agregar modificación a la versión
	agregarModificacion(ver, mod);
	
	return OK;
}

TipoRet BorrarLinea(Archivo &a, char * version, unsigned int nroLinea, char * error){
// Esta función elimina una línea de texto de la version del archivo a en la posición nroLinea.
// El número de línea debe estar entre 1 y n, siendo n la cantidad de líneas del archivo. Por ejemplo, si el texto tiene 7 líneas, se podrán eliminar líneas de las posiciones 1 a 7.
// Cuando se elimina una línea, las siguientes líneas se corren, decrementando en una unidad sus posiciones para ocupar el lugar de la línea borrada.
// No se puede borrar una línea de una versión que tenga subversiones creadas.
// En caso que TipoRet sea ERROR, en error se debe cargar cuál es el mismo.

	// Validación: archivo debe existir
	if (a == nullptr) {
		strcpy(error, "Archivo no existe");
		return ERROR;
	}
	
	// Validación: versión no puede ser NULL
	if (version == nullptr) {
		strcpy(error, "Version es NULL");
		return ERROR;
	}
	
	// Validación: nroLinea debe ser al menos 1
	if (nroLinea < 1) {
		strcpy(error, "Numero de linea invalido (debe ser >= 1)");
		return ERROR;
	}
	
	// FASE 1: Solo versiones de primer nivel
	// Parsear número de versión
	int numVersion = atoi(version);
	
	if (numVersion <= 0) {
		strcpy(error, "Version invalida (debe ser numero positivo)");
		return ERROR;
	}
	
	// Buscar la versión en el archivo
	Version ver = buscarVersionEnLista(a->primeraVersion, numVersion);
	
	// Validar que la versión existe
	if (ver == nullptr) {
		strcpy(error, "Version no existe");
		return ERROR;
	}
	
	// Reconstruir texto actual para saber cuántas líneas hay
	ListaLineas texto = crearListaLineas();
	aplicarModificaciones(texto, ver->primeraModificacion);
	unsigned int numLineas = contarLineas(texto);
	
	// Validar que hay líneas para borrar
	if (numLineas == 0) {
		liberarListaLineas(texto);
		strcpy(error, "No hay lineas para borrar (version vacia)");
		return ERROR;
	}
	
	// Validar que nroLinea está entre 1 y n (NO se puede borrar en n+1)
	if (nroLinea > numLineas) {
		liberarListaLineas(texto);
		strcpy(error, "Numero de linea fuera de rango (no existe)");
		return ERROR;
	}
	
	// Liberar texto temporal (ya no lo necesitamos)
	liberarListaLineas(texto);
	
	// Crear modificación de tipo BORRADO
	// Para BORRADO, el texto es NULL (no se necesita guardar texto)
	Modificacion mod = crearModificacion(BORRADO, nroLinea, nullptr);
	
	// Agregar modificación a la versión
	agregarModificacion(ver, mod);
	
	return OK;
}

TipoRet MostrarTexto(Archivo a, char * version){
// Esta función muestra el texto completo de la version, teniendo en cuenta los cambios realizados en dicha versión y en las versiones ancestras, de la cual ella depende.

	// Validar archivo
	if (a == nullptr) {
		return ERROR;
	}

	// Parsear versión (Fase 1: solo número simple)
	int numVersion = atoi(version);
	
	// Buscar la versión
	Version ver = buscarVersionEnLista(a->primeraVersion, numVersion);
	
	// Validar que la versión existe
	if (ver == nullptr) {
		return ERROR;
	}

	// Crear lista temporal para reconstruir el texto
	ListaLineas texto = crearListaLineas();
	
	// Aplicar todas las modificaciones en orden
	aplicarModificaciones(texto, ver->primeraModificacion);
	
	// Imprimir formato: "Archivo: nombre - Version N"
	cout << "Archivo: " << a->nombre << " - Version " << numVersion << endl << endl;
	
	// Verificar si hay líneas
	if (texto == nullptr) {
		// Versión sin líneas (todas fueron borradas o nunca se insertaron)
		cout << "(versión sin líneas)" << endl;
	} else {
		// Imprimir todas las líneas numeradas
		imprimirListaLineas(texto);
	}
	
	// Liberar memoria temporal
	liberarListaLineas(texto);
	
	return OK;
}

TipoRet MostrarCambios(Archivo a, char * version){
// Esta función muestra los cambios que se realizaron en el texto de la version parámetro, sin incluir los cambios realizados en las versiones ancestras de la cual dicha versión depende.

	return NO_IMPLEMENTADA;
}

TipoRet Iguales(Archivo a, char * version1, char * version2, bool &iguales){
// Esta función asigna al parámetro booleano (iguales) el valor true si ambas versiones (version1 y version2) del archivo tienen exactamente el mismo texto, y false en caso contrario.

	return NO_IMPLEMENTADA;
}

TipoRet VersionIndependiente(Archivo &a, char * version){
// Esta función crea una nueva versión al final del primer nivel con todos los cambios de la version especificada y de sus versiones ancestras. La versión que se crea debe ser independiente de cualquier otra versión.
// Por ejemplo, si creamos una versión independiente a partir de la 2.11.3, entonces se crea una nueva versión al final del primer nivel (si existen las versiones 1, 2, 3 y 4, entonces se crea la 5) con los cambios realizados a las versiones 2, 2.11 y 2.11.3.

	return NO_IMPLEMENTADA;
}
