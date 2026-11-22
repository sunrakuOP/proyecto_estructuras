#ifndef COLAPRIORIDADPACIENTES_HPP
#define COLAPRIORIDADPACIENTES_HPP
#include <list>
#include "modelo/Enums.hpp"

class Paciente;

/* COLAS DE PRIORIDAD (TEORIA):

La cola normal solo saca el primer elemento en ella, FIFO
La cola de prioridad saca el elemento con mayor prioridad.

Como ???? 
Se puede hacer de varias formas:

1. primero organizando segun la prioridad al encolar O(n) al agregar y O(1) al desencolar
2. agregar normal (O(1)) y recorrer hasta encontrar O(n)

En mi caso:
COLA SOLO GUARDA Paciente*, entonces ellos tienen su propia prioridad

EL cast de aNumero(Gravedad) ya lo tenemos.

Como la implementamos? Una lista y yo manejo el orden de agregar y sacar



AL AGREGAR RECORREMOS Y COLOCAMOS DONDE VA (Segun prioridad) => O(n)
PERO EL DESENCOLAR Y FRONT ES => O(1)
*/

class ColaPrioridadPacientes {
private:
    std::list<Paciente*> colaPrioridad;
public:
    ColaPrioridadPacientes() {}

    ~ColaPrioridadPacientes() {}

    bool estaVacia() const;
    bool encolar(Paciente* p);
    Paciente* desencolar();

    // recorrer la lista, envejecer a todos y sacar los perdidos
    std::list<Paciente*> envejecer();

    int cantidadPorGravedad(Gravedad g) const;

    int totalPacientesCola() const;

    int sumaEsperaPorGravedad(Gravedad g) const;
    
    bool hayAlertaEnGravedad() const;
};

#endif
