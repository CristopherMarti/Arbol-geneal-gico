//===================================================================
//  Gestor de Personas (Árbol Genealógico)
//-------------------------------------------------------------------
//  EQUIPO: Grupo 04
//  Descripción:
//  Este programa demuestra una estructura de datos mixta para modelar
//  un árbol genealógico. Las personas se almacenan en un árbol binario
//  de búsqueda (ordenado por ID) y, adicionalmente, cada persona
//  mantiene una lista enlazada simple con sus hijos para poder recorrer
//  los descendientes directos.  
//  
//  Funcionalidades principales:
//  - Insertar personas en el árbol (con validaciones de ID único y datos
//    obligatorios).
//  - Asignar padre-hijo (por ID) mediante una lista de hijos.
//  - Búsqueda de personas por ID y por nombre.
//  - Recorridos clásicos (preorden, inorden, postorden) del árbol BST.
//  - Mostrar ancestros (camino hacia la raíz) y descendientes recursivos.
//  - Menú interactivo en consola.
//===================================================================

#include <iostream>
#include <string>
using namespace std;

//--------------------------------------------------------------------
//  Adelanto de la estructura Persona para que pueda ser referenciada
//  dentro de la estructura Hijo.
//--------------------------------------------------------------------
struct Persona;

//--------------------------------------------------------------------
//  Estructura Hijo: nodo de la lista enlazada simple que almacena los
//  hijos de una persona.
//--------------------------------------------------------------------
struct Hijo {
    Persona* persona;   // Puntero al objeto Persona (hijo)
    Hijo* siguiente;    // Puntero al siguiente nodo en la lista
};

//--------------------------------------------------------------------
//  Estructura Persona: nodo del árbol binario de búsqueda.
//--------------------------------------------------------------------
struct Persona {
    int id;                     // Identificador único y clave del BST
    string nombre;              // Nombre completo
    string fechaNacimiento;     // Fecha con formato AAAA-MM-DD
    string genero;              // "M", "F" u otra designación

    // Punteros del árbol binario de búsqueda
    Persona* izq;               // Hijo izquierdo (IDs menores)
    Persona* der;               // Hijo derecho (IDs mayores)
    Persona* padre;             // Puntero al padre en el BST

    // Lista de hijos (descendencia directa)
    Hijo* hijos;                // Cabeza de la lista de hijos
};

//--------------------------------------------------------------------
//  crearPersona
//  Crea dinámicamente un objeto Persona e inicializa todos los campos
//  y punteros a valores seguros (NULL / "" según corresponda).
//--------------------------------------------------------------------
Persona* crearPersona(int id, const string& nombre, const string& fecha, const string& genero) {
    Persona* p = new Persona;
    p->id = id;
    p->nombre = nombre;
    p->fechaNacimiento = fecha;
    p->genero = genero;

    // Inicialización de punteros
    p->izq = NULL;
    p->der = NULL;
    p->padre = NULL;
    p->hijos = NULL;
    return p;
}

//--------------------------------------------------------------------
//  insertarPersona (BST)
//  Inserta recursivamente una nueva persona en el árbol binario de
//  búsqueda respetando la propiedad de orden por ID. Devuelve la raíz
//  (que puede cambiar si estaba vacía).
//--------------------------------------------------------------------
Persona* insertarPersona(Persona* raiz, Persona* nueva) {
    if (!raiz) return nueva; // Caso base: subárbol vacío

    if (nueva->id < raiz->id) {
        // Insertar en el subárbol izquierdo
        raiz->izq = insertarPersona(raiz->izq, nueva);
        raiz->izq->padre = raiz; // Actualizar relación padre
    } else {
        // Insertar en el subárbol derecho (ID >= raíz->id)
        raiz->der = insertarPersona(raiz->der, nueva);
        raiz->der->padre = raiz;
    }
    return raiz; // La raíz nunca cambia aquí
}

//--------------------------------------------------------------------
//  agregarHijo
//  Agrega un nodo a la lista de hijos de la persona "padre". No afecta
//  la posición del hijo dentro del BST; sólo construye la relación
//  genealógica.
//--------------------------------------------------------------------
void agregarHijo(Persona* padre, Persona* hijo) {
    hijo->padre = padre; // Establecer relación inversa

    // Crear nodo de lista
    Hijo* nuevo = new Hijo;
    nuevo->persona = hijo;
    nuevo->siguiente = NULL;

    // Insertar al final de la lista de hijos
    if (!padre->hijos) {
        padre->hijos = nuevo; // Primer hijo
    } else {
        Hijo* actual = padre->hijos;
        while (actual->siguiente) actual = actual->siguiente;
        actual->siguiente = nuevo;
    }
}

//--------------------------------------------------------------------
//  Buscar persona por ID (BST)
//--------------------------------------------------------------------
Persona* buscarPorID(Persona* raiz, int id) {
    if (!raiz || raiz->id == id) return raiz; // Encontrado o subárbol vacío
    return (id < raiz->id) ? buscarPorID(raiz->izq, id)  // Buscar en izq
                           : buscarPorID(raiz->der, id); // Buscar en der
}

//--------------------------------------------------------------------
//  Buscar persona por nombre (recorrido inorden completo)
//  Nota: Complejidad O(n) ya que no existe índice por nombre.
//--------------------------------------------------------------------
Persona* buscarPorNombre(Persona* raiz, const string& nombre) {
    if (!raiz) return NULL;
    if (raiz->nombre == nombre) return raiz;

    // Buscar primero en el subárbol izquierdo
    Persona* p = buscarPorNombre(raiz->izq, nombre);
    return p ? p : buscarPorNombre(raiz->der, nombre); // Si no, en el derecho
}
//--------------------------------------------------------------------
//  Recorridos clásicos del BST
//--------------------------------------------------------------------
void preorden(Persona* nodo) {
    if (!nodo) return;
    cout << nodo->nombre << " (ID: " << nodo->id << ")\n";
    preorden(nodo->izq);
    preorden(nodo->der);
}

void inorden(Persona* nodo) {
    if (!nodo) return;
    inorden(nodo->izq);
    cout << nodo->nombre << " (ID: " << nodo->id << ")\n";
    inorden(nodo->der);
}

void postorden(Persona* nodo) {
    if (!nodo) return;
    postorden(nodo->izq);
    postorden(nodo->der);
    cout << nodo->nombre << " (ID: " << nodo->id << ")\n";
}

//--------------------------------------------------------------------
//  mostrarAncestros
//  Imprime en consola la línea ascendente (padres, abuelos, etc.) hasta
//  la raíz del árbol BST.
//--------------------------------------------------------------------
void mostrarAncestros(Persona* persona) {
    while (persona->padre) {
        persona = persona->padre;
        cout << persona->nombre << " (ID: " << persona->id << ")\n";
    }
}

//--------------------------------------------------------------------
//  mostrarDescendientes
//  Recorrido recursivo preorden sobre la lista de hijos de "persona" y
//  sus sub-descendientes.
//--------------------------------------------------------------------
void mostrarDescendientes(Persona* persona) {
    Hijo* actual = persona->hijos;
    while (actual) {
        cout << actual->persona->nombre << " (ID: " << actual->persona->id << ")\n";
        mostrarDescendientes(actual->persona); // Recursión en nietos, etc.
        actual = actual->siguiente;
    }
}

//--------------------------------------------------------------------
//  Función principal (menú)
//--------------------------------------------------------------------
int main() {
    setlocale(LC_CTYPE,"Spanish");
    Persona* raiz = NULL; // Árbol inicialmente vacío
    int opcion;

    do {
        // Menú principal
        cout << "\n=== Gestor de Personas ===\n";
        cout << "1. Insertar persona\n";
        cout << "2. Buscar persona\n";
        cout << "3. Mostrar Preorden\n";
        cout << "4. Mostrar Inorden\n";
        cout << "5. Mostrar Postorden\n";
        cout << "6. Mostrar ancestros\n";
        cout << "7. Mostrar descendientes\n";
        cout << "8. Salir\n";
        cout << "Opción: ";
        cin >> opcion;
        cin.ignore(); // Limpiar buffer del salto de línea

        switch (opcion) {
            // ---------------------------------------------------------
            case 1: {
                // === Insertar persona ===
                int id;
                string nombre, fecha, genero;

                // ---- Validar que el ID sea positivo y único ----
                do {
                    cout << "ID (positivo): ";
                    cin >> id;
                    if (id < 0) {
                        cout << "El ID no puede ser negativo.\n";
                    } else if (buscarPorID(raiz, id)) {
                        cout << "Este ID ya existe. Ingrese uno diferente.\n";
                        id = -1; // Forzar repetición
                    }
                } while (id < 0);
                cin.ignore(); // Consumir salto de línea restante

                // ---- Solicitar nombre (no vacío) ----
                do {
                    cout << "Nombre: ";
                    getline(cin, nombre);
                    if (nombre.empty()) cout << "El nombre no puede estar vacío.\n";
                } while (nombre.empty());

                // ---- Solicitar fecha de nacimiento ----
                do {
                    cout << "Fecha de nacimiento (ej. 2000-01-01): ";
                    getline(cin, fecha);
                    if (fecha.empty()) cout << "La fecha no puede estar vacía.\n";
                } while (fecha.empty());

                // ---- Solicitar género ----
                do {
                    cout << "Género (M/F): ";
                    getline(cin, genero);
                    if (genero.empty()) cout << "El género no puede estar vacío.\n";
                } while (genero.empty());

                // Crear e insertar la nueva persona
                Persona* nueva = crearPersona(id, nombre, fecha, genero);
                raiz = insertarPersona(raiz, nueva);

                // Si la persona NO es la raíz (la raíz no tiene padre)
                if (raiz != nueva) {
                    int idPadre;
                    cout << "¿Tiene padre? (Ingrese ID o -1 si no): ";
                    cin >> idPadre;
                    if (idPadre != -1) {
                        Persona* padre = buscarPorID(raiz, idPadre);
                        if (padre) {
                            agregarHijo(padre, nueva);
                        } else {
                            cout << "Padre no encontrado. Se insertó sin asignar padre.\n";
                        }
                    }
                }

                break;
            }
            // ---------------------------------------------------------
            case 2: {
                // === Buscar persona ===
                cout << "Buscar por:\n1. ID\n2. Nombre\nOpción: ";
                int tipo; cin >> tipo; cin.ignore();
                if (tipo == 1) {
                    int id;
                    cout << "ID: "; cin >> id;
                    Persona* p = buscarPorID(raiz, id);
                    if (p) cout << "Encontrado: " << p->nombre << "\n";
                    else cout << "No encontrado.\n";
                } else {
                    string nombre;
                    cout << "Nombre: "; getline(cin, nombre);
                    Persona* p = buscarPorNombre(raiz, nombre);
                    if (p) cout << "Encontrado: " << p->nombre << "\n";
                    else cout << "No encontrado.\n";
                }
                break;
            }
            // ---------------------------------------------------------
            case 3: preorden(raiz); break;
            case 4: inorden(raiz); break;
            case 5: postorden(raiz); break;

            // ---------------------------------------------------------
            case 6: {
                // === Mostrar ancestros ===
                int id;
                cout << "ID de la persona: "; cin >> id;
                Persona* p = buscarPorID(raiz, id);
                if (p) mostrarAncestros(p);
                else cout << "No encontrado.\n";
                break;
            }
            // ---------------------------------------------------------
            case 7:
                // === Mostrar descendientes (desde la raíz) ===
                if (raiz) mostrarDescendientes(raiz);
                else cout << "Árbol vacío.\n";
                break;

            // ---------------------------------------------------------
            case 8: cout << "Saliendo...\n"; break;
            default: cout << "Opción inválida.\n";
        }

    } while (opcion != 8);

    // Nota: No se liberan los nodos creados (fuga de memoria). Para un
    // programa real, se debería implementar una función recursiva que
    // destruyera el BST y las listas de hijos antes de salir.

    return 0;
}