//===================================================================
//  Gestor de Personas (Árbol Genealógico)
//-------------------------------------------------------------------
//  Autor: <Tu nombre aquí>
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