#include "estructuras/PilaAtendidos.hpp"
#include "modelo/Paciente.hpp"

bool PilaAtendidos::apilar(const Paciente& p)  {
    if(p.getEstadoPaciente() != EstadoPaciente::alta) {
        return false;
    }
    pilaAtendidos.push(&p); // recordar que agrega el puntero
    return true;
}

bool PilaAtendidos::estaVacia() const {
    return pilaAtendidos.empty();
}

void PilaAtendidos::vaciar() {
    while(!pilaAtendidos.empty()){
        pilaAtendidos.pop();
    }
}

int PilaAtendidos::cantidad() const {
    return (int)pilaAtendidos.size();
}

std::list<const Paciente*> PilaAtendidos::ultimos() const {
    std::list<const Paciente*> listaInvertida;
    std::stack<const Paciente*> copiaPila = pilaAtendidos;

    while(!copiaPila.empty()) {
        listaInvertida.push_back(copiaPila.top());
        copiaPila.pop();
    }

    return listaInvertida;
}
