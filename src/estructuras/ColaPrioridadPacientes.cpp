#include "estructuras/ColaPrioridadPacientes.hpp"
#include "modelo/Paciente.hpp"

bool ColaPrioridadPacientes::estaVacia() const {
    return colaPrioridad.empty();
}

bool ColaPrioridadPacientes::encolar(Paciente* p) {
    // chequear que el paciente no sea null
    if(p == nullptr) {
        return false;
    }
    int prioridad = aNumero(p->getGravedad());

    // recorrer en listas se hace con iterador
    for(auto it = colaPrioridad.begin(); it != colaPrioridad.end(); ++it) {
        if(prioridad < aNumero((*it)->getGravedad())) {
            colaPrioridad.insert(it, p);
            return true;
        }
    }
    // si llega al final, pues se hace push_back porque es el de menor priroidad
    colaPrioridad.push_back(p);
    return true;
}

Paciente* ColaPrioridadPacientes::desencolar() {
    if(estaVacia()) {
        return nullptr;
    }

    Paciente* p = colaPrioridad.front();
    colaPrioridad.pop_front();
    return p;
}

std::list<Paciente*> ColaPrioridadPacientes::envejecer() {
    std::list<Paciente*> listaPerdidos;

    for(auto it = colaPrioridad.begin(); it != colaPrioridad.end();) {
        (*it)->esperarUnTurno(); // esperar mientras el medico atiende
        if((*it)->estaPerdido()) {
            listaPerdidos.push_back(*it);
            it = colaPrioridad.erase(it);
        } else {
            ++it;
        }
    }

    return listaPerdidos;
}



int ColaPrioridadPacientes::cantidadPorGravedad(Gravedad g) const {
    int contador = 0;
    for(auto& p: colaPrioridad) {
        if(aNumero(g) == aNumero(p->getGravedad())) {
            contador++;
        }
    }
    return contador;
}
