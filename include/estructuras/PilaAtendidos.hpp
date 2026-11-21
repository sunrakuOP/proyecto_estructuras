#ifndef PILAATENDIDOS_HPP
#define PILAATENDIDOS_HPP
#include <stack>
#include <list>

class Paciente;

class PilaAtendidos {
private:
    /* Por que puntero y no copia ??
    Porque hay una sola copia real de los pacientes, aca solo es para usar el mismo sabiendo donde vive en memoria
    Si algo cambiara, quedaria desactualizado usando copia

    (solo acepta pacientes cuyo estado sea alta)
    */

    std::stack<const Paciente*> pilaAtendidos;
public:
    PilaAtendidos() {}

    bool apilar(const Paciente& p);
    bool estaVacia() const;
    void vaciar();
    int cantidad() const;

    // ahora el metodo mas dificil de la pila, uso una copia para no modificar la original
    std::list<const Paciente*> ultimos() const;
};

#endif
